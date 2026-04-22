#include "jxx.lang.Throwable.h"

#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.IllegalArgumentException.h"

namespace jxx::lang {

void Throwable::addSuppressed(const Ptr& ex) {
    if (!enableSuppression_) {
        return;
    }
    if (!ex) {
        throw NullPointerException(String("Cannot suppress null exception"));
    }
    if (ex.get() == this) {
        throw IllegalArgumentException(String("Self-suppression not permitted"));
    }
    suppressed_.push_back(ex);
}

} // namespace jxx::lang