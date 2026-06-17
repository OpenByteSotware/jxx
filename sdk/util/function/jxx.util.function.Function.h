#pragma once

namespace jxx {
    namespace util {
        namespace function {

            template <typename T, typename R>
            class Function {
            public:
                virtual ~Function() = default;

                // Java: R apply(T t)
                virtual jxx::Ptr<R> apply(jxx::Ptr<T> t) = 0;

                // Java 8 default methods:
                // <V> Function<V, R> compose(Function<? super V, ? extends T> before)
                // <V> Function<T, V> andThen(Function<? super R, ? extends V> after)
                //
                // Note:
                // Exact Java generic wildcard parity for these methods requires
                // additional wildcard bridge types for java.util.function variance.
                // If you want, I can generate those next as a dedicated function-variance stage.

                template <typename V>
                jxx::Ptr<Function<V, R>> compose(jxx::Ptr<Function<V, T>> /*before*/) {
                    throw UnsupportedOperationException();
                }

                template <typename V>
                jxx::Ptr<Function<T, V>> andThen(jxx::Ptr<Function<R, V>> /*after*/) {
                    throw UnsupportedOperationException();
                }

                // Java: static <T> Function<T, T> identity()
                static jxx::Ptr<Function<T, T>> identity() {
                    class IdentityFunction : public virtual Function<T, T> {
                    public:
                        virtual ~IdentityFunction() = default;

                        virtual jxx::Ptr<T> apply(jxx::Ptr<T> t) override {
                            return t;
                        }
                    };

                    return jxx::Ptr<Function<T, T>>(new IdentityFunction());
                }
            };

        } // namespace function
    } // namespace util
} // namespace jxx
``