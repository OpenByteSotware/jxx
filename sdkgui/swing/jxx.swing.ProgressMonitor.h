#pragma once
#include "awt/jxx.awt.Component.h"
namespace jxx::swing
{
    class ProgressMonitor : public ::jxx::lang::ClassBase<ProgressMonitor, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<ProgressMonitor, JxxSuper>;
        ProgressMonitor(const ::jxx::Ptr<::jxx::awt::Component>& parentComponent,
            const ::jxx::Ptr<::jxx::lang::Object>& message,
            const ::jxx::Ptr<::jxx::lang::String>& note,
            ::jxx::lang::jint minimum, ::jxx::lang::jint maximum);
        void setProgress(::jxx::lang::jint value);
        void close() noexcept;
        ::jxx::lang::jbool isCanceled() const noexcept;
        void cancel() noexcept;
        void setMinimum(::jxx::lang::jint value) noexcept;
        ::jxx::lang::jint getMinimum() const noexcept;
        void setMaximum(::jxx::lang::jint value) noexcept;
        ::jxx::lang::jint getMaximum() const noexcept;
        void setNote(const ::jxx::Ptr<::jxx::lang::String>& note);
        ::jxx::Ptr<::jxx::lang::String> getNote() const;
        void setMillisToDecideToPopup(::jxx::lang::jint millis) noexcept;
        ::jxx::lang::jint getMillisToDecideToPopup() const noexcept;
        void setMillisToPopup(::jxx::lang::jint millis) noexcept;
        ::jxx::lang::jint getMillisToPopup() const noexcept;
    private:
        ::jxx::Ptr<::jxx::awt::Component> parentComponent_;
        ::jxx::Ptr<::jxx::lang::Object> message_;
        ::jxx::Ptr<::jxx::lang::String> note_;
        ::jxx::lang::jint minimum_, maximum_, progress_;
        ::jxx::lang::jint millisToDecideToPopup_ = 500, millisToPopup_ = 2000;
        ::jxx::lang::jbool canceled_ = false, closed_ = false;
    };
}
