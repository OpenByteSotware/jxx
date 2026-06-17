#pragma once

namespace jxx {
    namespace util {
        namespace function {

            template <typename T>
            class ConsumerSuper {
            public:
                virtual ~ConsumerSuper() = default;

                // Java: void accept(T t)
                // Changed from jxx::Ptr<T> to T to match Java semantics
                virtual void accept(T value) = 0;
            };

            template <typename T>
            class SelfRef {
            public:
                static jxx::Ptr<T> get(T* ptr) {
                    // Attempt to cast to enable_shared_from_this if possible
                    if (auto eshared = dynamic_cast<std::enable_shared_from_this<T>*>(ptr)) {
                        try {
                            return std::static_pointer_cast<T>(eshared->shared_from_this());
                        }
                        catch (const std::bad_weak_ptr&) {
                            // Fall through to nullptr
                        }
                    }
                    return nullptr;
                }
            };

        } // namespace function
    } // namespace util
} // namespace jxx
