#include "jxx.lang.String.h"
#include "jxx.lang.Throwable.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.IllegalArgumentException.h"

namespace jxx::lang {

void Throwable::addSuppressed(const jxx::Ptr<Throwable> ex) {
    if (!enableSuppression_) {
        return;
    }
    if (!ex) {
        throw NullPointerException("Cannot suppress null exception");
    }
    if (ex.get() == this) {
        throw IllegalArgumentException("Self-suppression not permitted");
    }
    suppressed_.push_back(ex);
}

} // namespace jxx::lang