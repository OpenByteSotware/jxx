#include "io/jxx.util.InputMismatchException.h"

namespace jxx {
namespace util {

InputMismatchException::InputMismatchException()
    : NoSuchElementException() {
}

InputMismatchException::InputMismatchException(jxx::Ptr<jxx::lang::String> message)
    : NoSuchElementException(message) {
}

} // namespace util
} // namespace jxx
