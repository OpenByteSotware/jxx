#pragma once

#include "lang/jxx_types.h"

namespace jxx {
    namespace util {
        namespace function {

            template <typename Base, typename Self>
            struct SelfRef {
                static jxx::Ptr<Base> get(Self* self) {
                    return jxx::Ptr<Base>(self);
                }
            };


            template <typename T>
            class ConsumerSuper : virtual public jxx::lang::Object {
            public:
                virtual ~ConsumerSuper() = default;

                virtual void accept(jxx::Ptr<T> value) = 0;
            };

        } // namespace function
    } // namespace util
} // namespace jxx
