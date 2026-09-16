#include "awt/jxx.awt.HeadlessException.h"

namespace jxx::awt
{
    HeadlessException::HeadlessException()
        : Super()
    {
    }

    HeadlessException::HeadlessException(
        const ::jxx::Ptr<::jxx::lang::String>& message)
        : Super(message)
    {
    }

}
