#include "awt/jxx.awt.IllegalComponentStateException.h"

namespace jxx::awt
{
    IllegalComponentStateException::IllegalComponentStateException()
        : Super()
    {
    }

    IllegalComponentStateException::IllegalComponentStateException(
        const ::jxx::Ptr<::jxx::lang::String>& message)
        : Super(message)
    {
    }

}
