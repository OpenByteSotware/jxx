#pragma once

#include <exception>
#include <type_traits>
#include <utility>

namespace jxx::lang
{
    template<typename Function>
    class Finally final
    {
    public:
        explicit Finally(Function&& function)
            noexcept(std::is_nothrow_move_constructible_v<Function>)
            : function_(std::forward<Function>(function)),
              active_(true),
              uncaughtOnCreation_(std::uncaught_exceptions())
        {
        }

        Finally(const Finally&) = delete;
        auto operator=(const Finally&) -> Finally& = delete;

        Finally(Finally&& other)
            noexcept(std::is_nothrow_move_constructible_v<Function>)
            : function_(std::move(other.function_)),
              active_(other.active_),
              uncaughtOnCreation_(other.uncaughtOnCreation_)
        {
            other.active_ = false;
        }

        auto operator=(Finally&&) -> Finally& = delete;

        ~Finally() noexcept(false)
        {
            if (!active_) {
                return;
            }

            // Generated code invokes runAndDismiss() from an enclosing
            // catch before rethrowing. Reaching this branch during unrelated
            // stack unwinding must never start a second exception.
            if (std::uncaught_exceptions() > uncaughtOnCreation_) {
                try
                {
                    function_();
                }
                catch (...)
                {
                    std::terminate();
                }
                return;
            }

            function_();
        }

        void runAndDismiss()
        {
            if (!active_) {
                return;
            }
            active_ = false;
            function_();
        }

        void dismiss() noexcept
        {
            active_ = false;
        }

    private:
        Function function_;
        bool active_;
        int uncaughtOnCreation_;
    };

    template<typename Function>
    auto makeFinally(Function&& function)
        -> Finally<std::decay_t<Function>>
    {
        return Finally<std::decay_t<Function>>(
            std::forward<Function>(function));
    }
}
