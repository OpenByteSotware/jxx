#include "swing/jxx.swing.ProgressMonitor.h"
namespace jxx::swing
{
    ProgressMonitor::ProgressMonitor(const ::jxx::Ptr<::jxx::awt::Component>& parent, const ::jxx::Ptr<::jxx::lang::Object>& message, const ::jxx::Ptr<::jxx::lang::String>& note, ::jxx::lang::jint minimum, ::jxx::lang::jint maximum)
        : parentComponent_(parent), message_(message), note_(note), minimum_(minimum), maximum_(maximum), progress_(minimum) {}
    void ProgressMonitor::setProgress(::jxx::lang::jint v) { progress_=v; if (v >= maximum_) close(); }
    void ProgressMonitor::close() noexcept { closed_=true; }
    ::jxx::lang::jbool ProgressMonitor::isCanceled() const noexcept { return canceled_; }
    void ProgressMonitor::cancel() noexcept { canceled_=true; }
    void ProgressMonitor::setMinimum(::jxx::lang::jint v) noexcept { minimum_=v; } ::jxx::lang::jint ProgressMonitor::getMinimum() const noexcept { return minimum_; }
    void ProgressMonitor::setMaximum(::jxx::lang::jint v) noexcept { maximum_=v; } ::jxx::lang::jint ProgressMonitor::getMaximum() const noexcept { return maximum_; }
    void ProgressMonitor::setNote(const ::jxx::Ptr<::jxx::lang::String>& v) { note_=v; } ::jxx::Ptr<::jxx::lang::String> ProgressMonitor::getNote() const { return note_; }
    void ProgressMonitor::setMillisToDecideToPopup(::jxx::lang::jint v) noexcept { millisToDecideToPopup_=v; } ::jxx::lang::jint ProgressMonitor::getMillisToDecideToPopup() const noexcept { return millisToDecideToPopup_; }
    void ProgressMonitor::setMillisToPopup(::jxx::lang::jint v) noexcept { millisToPopup_=v; } ::jxx::lang::jint ProgressMonitor::getMillisToPopup() const noexcept { return millisToPopup_; }
}
