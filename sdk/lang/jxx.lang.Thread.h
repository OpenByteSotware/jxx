#pragma once
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

// -----------------------------------------------------------------------------
// Optional integration with jxx_exception.hpp for InterruptedException
// -----------------------------------------------------------------------------
#if defined(__has_include)
#if __has_include("jxx_exception.hpp")
#include "jxx_exception.hpp"
namespace jxx_thr_detail {
    using InterruptedBase = jxx::ex::RuntimeException;
    inline InterruptedBase make_interrupted(const char* msg,
        const char* file,
        int line,
        const char* func) {
        return InterruptedBase{ msg, {}, jxx::ex::make_source_loc(file, line, func) };
    }
}
#define JXX_THROW_INTERRUPTED(MSG) \
      throw jxx_thr_detail::make_interrupted((MSG), __FILE__, __LINE__, __func__)
#else
#include <stdexcept>
namespace jxx_thr_detail {
    struct InterruptedException : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}
#define JXX_THROW_INTERRUPTED(MSG) \
      throw jxx_thr_detail::InterruptedException(MSG)
#endif
#else
#include <stdexcept>
namespace jxx_thr_detail {
    struct InterruptedException : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}
#define JXX_THROW_INTERRUPTED(MSG) \
    throw jxx_thr_detail::InterruptedException(MSG)
#endif

namespace jxx::lang {

    // -----------------------------------------------------------------------------
    // Runnable interface (Java-like)
    // -----------------------------------------------------------------------------
    struct Runnable {
        virtual ~Runnable() = default;
        virtual void run() = 0;
    };

    // -----------------------------------------------------------------------------
    // Thread (Java-like) built on top of std::thread
    // -----------------------------------------------------------------------------
    class Thread {
    public:
        using Target = std::function<void()>;
        using UncaughtHandler = std::function<void(const std::exception&)>;

    private:
        // Shared state between owner thread and worker thread
        struct State {
            std::atomic<bool> started{ false };
            std::atomic<bool> running{ false };
            std::atomic<bool> finished{ false };
            std::atomic<bool> interrupted{ false };

            std::string name{ "Thread" };
            bool daemon{ false };
            int  priority{ 0 }; // hint only; no portable effect in standard C++

            // One mutex for coordination of finished/interrupt waits
            std::mutex m;
            std::condition_variable cv_finished;
            std::condition_variable cv_interrupt; // used to wake interruptible waits

            // Uncaught exception handler
            UncaughtHandler handler{};

            // Optional targets; if none are provided, virtual run() is called.
            Target target{};
            std::shared_ptr<Runnable> runnable{};

            State() = default;
        };

        std::shared_ptr<State> st_ = std::make_shared<State>();
        std::thread th_{};

        // Per-thread pointer to current State (only set for threads started via this wrapper)
        static thread_local State* tls_current_;

        // Optional process-wide default handler (used if per-thread handler is empty)
        static UncaughtHandler& defaultHandlerRef() {
            static UncaughtHandler h{};
            return h;
        }

        // Worker trampoline: calls lambda, Runnable::run(), or virtual run() on 'self'
        static void entry(Thread* self, std::shared_ptr<State> st) noexcept {
            tls_current_ = st.get();
            st->running.store(true, std::memory_order_release);

            try {
                if (st->target) {
                    st->target();
                }
                else if (st->runnable) {
                    st->runnable->run();
                }
                else {
                    // Dispatch to derived override
                    self->run();
                }
            }
            catch (const std::exception& e) {
                if (st->handler) {
                    try { st->handler(e); }
                    catch (...) {/* swallow handler failure */ }
                }
                else if (defaultHandlerRef()) {
                    try { defaultHandlerRef()(e); }
                    catch (...) {}
                }
                else {
                    // No handler: swallow to avoid terminate() (Java's default prints to stderr).
                    // You can switch this policy to std::terminate() if you prefer fail-fast.
                }
            }
            catch (...) {
                // Non-std exceptions: route to handler if present
                struct Unknown : std::exception { const char* what() const noexcept override { return "unknown exception"; } } u;
                if (st->handler) {
                    try { st->handler(u); }
                    catch (...) {}
                }
                else if (defaultHandlerRef()) {
                    try { defaultHandlerRef()(u); }
                    catch (...) {}
                }
            }

            st->running.store(false, std::memory_order_release);
            st->finished.store(true, std::memory_order_release);
            {
                std::lock_guard<std::mutex> lk(st->m);
            }
            st->cv_finished.notify_all();
            st->cv_interrupt.notify_all();
            tls_current_ = nullptr;
        }

    public:
        // -------------------- Construction --------------------
        Thread() = default; // override run() and call start()

        explicit Thread(Target t, std::string name = {}) {
            st_->target = std::move(t);
            if (!name.empty()) st_->name = std::move(name);
        }

        explicit Thread(std::shared_ptr<Runnable> r, std::string name = {}) {
            st_->runnable = std::move(r);
            if (!name.empty()) st_->name = std::move(name);
        }

        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        Thread(Thread&& other) noexcept
            : st_(std::move(other.st_))
            , th_(std::move(other.th_)) {}

        Thread& operator=(Thread&& other) noexcept {
            if (this != &other) {
                if (th_.joinable()) th_.detach(); // best-effort to avoid blocking in move-assign
                st_ = std::move(other.st_);
                th_ = std::move(other.th_);
            }
            return *this;
        }

        ~Thread() {
            // Java's Thread doesn't auto-join in finalize; we avoid blocking here.
            if (th_.joinable()) th_.detach();
        }

        // -------------------- Lifecycle --------------------
        void start() {
            bool expected = false;
            if (!st_->started.compare_exchange_strong(expected, true)) {
                throw std::logic_error("Thread already started");
            }
            // Pass 'this' so the worker can dispatch virtual run() when no target is set.
            th_ = std::thread(&Thread::entry, this, st_);
        }

        void join() {
            if (th_.joinable()) th_.join();
        }

        template <typename Rep, typename Period>
        bool joinFor(const std::chrono::duration<Rep, Period>& d) {
            if (!st_) return true;
            if (st_->finished.load(std::memory_order_acquire)) {
                if (th_.joinable()) th_.join();
                return true;
            }
            std::unique_lock<std::mutex> lk(st_->m);
            bool done = st_->cv_finished.wait_for(lk, d, [&] { return st_->finished.load(std::memory_order_acquire); });
            if (done && th_.joinable()) th_.join();
            return done;
        }

        // -------------------- Status & metadata --------------------
        bool isAlive() const noexcept {
            return st_ && st_->running.load(std::memory_order_acquire);
        }

        void setName(std::string s) { st_->name = std::move(s); }
        const std::string& getName() const noexcept { return st_->name; }

        std::thread::id getId() const noexcept { return th_.get_id(); }

        void setPriority(int p) { st_->priority = p; } // no portable effect
        int  getPriority() const noexcept { return st_->priority; }

        void setDaemon(bool d) { st_->daemon = d; }    // no portable effect
        bool isDaemon() const noexcept { return st_->daemon; }

        // -------------------- Uncaught exception handlers --------------------
        void setUncaughtExceptionHandler(UncaughtHandler h) { st_->handler = std::move(h); }
        static void setDefaultUncaughtExceptionHandler(UncaughtHandler h) { defaultHandlerRef() = std::move(h); }

        // -------------------- Interruption API --------------------
        void interrupt() noexcept {
            if (!st_) return;
            st_->interrupted.store(true, std::memory_order_release);
            // Wake any waits that are using the thread's interrupt CV
            st_->cv_interrupt.notify_all();
            // Also wake joiners if they're waiting (harmless)
            st_->cv_finished.notify_all();
        }

        bool isInterrupted() const noexcept {
            return st_ && st_->interrupted.load(std::memory_order_acquire);
        }

        // Static: check & clear current thread's interrupted status
        static bool interrupted() noexcept {
            if (tls_current_) {
                bool was = tls_current_->interrupted.exchange(false, std::memory_order_acq_rel);
                return was;
            }
            return false; // foreign threads not started by jxx::Thread
        }

        // Throw an InterruptedException (or fallback) if the flag is set (and clear it)
        static void checkInterrupted() {
            if (interrupted()) {
                JXX_THROW_INTERRUPTED("Thread interrupted");
            }
        }

        // Interruptible sleep: returns false if interrupted (and DOES NOT clear the flag)
        template <typename Rep, typename Period>
        static bool sleepForInterruptible(const std::chrono::duration<Rep, Period>& d) {
            // If not a jxx::Thread-managed worker, we can't observe a per-thread flag; do normal sleep.
            if (!tls_current_) {
                std::this_thread::sleep_for(d);
                return true;
            }
            auto* st = tls_current_;
            // Fast path: already interrupted
            if (st->interrupted.load(std::memory_order_acquire)) return false;

            std::unique_lock<std::mutex> lk(st->m);
            // Predicate wakes immediately if interrupted was set during wait
            bool wokeByInterrupt = st->cv_interrupt.wait_for(lk, d, [&] {
                return st->interrupted.load(std::memory_order_acquire);
                });
            // If woke by interrupt, return false. If timeout, return true.
            return !wokeByInterrupt;
        }

        static void sleep(unsigned int milliseconds) {
            Thread::sleep_for(milliseconds);
        }

        static void sleep_for(unsigned int milliseconds) {
            std::chrono::milliseconds time_ms(milliseconds);
            std::this_thread::sleep_for(time_ms);
        }

        static void yield() noexcept { std::this_thread::yield(); }

        // Pointer to current Thread wrapper is not exposed (cannot reconstruct safely).
        // Use static APIs above to interact with the current thread.
        static Thread* currentThread() noexcept { return nullptr; }

    protected:
        // Override this in subclasses if you want Java-style subclassing
        // and do NOT pass a lambda/Runnable in the constructor.
        virtual void run() {}
    };

    // Define the thread_local
    inline thread_local Thread::State* Thread::tls_current_ = nullptr;

} // namespace jxx
