#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt::print
{
    class Pageable;
    class PageFormat;
    class Printable;

    class PrinterJob : public ::jxx::lang::ClassBase<PrinterJob,
        ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<PrinterJob, JxxSuper>;

        ~PrinterJob() override = default;

        static ::jxx::Ptr<PrinterJob> getPrinterJob();

        virtual void setPrintable(
            const ::jxx::Ptr<Printable>& painter) = 0;
        virtual void setPrintable(const ::jxx::Ptr<Printable>& painter,
            const ::jxx::Ptr<PageFormat>& format) = 0;
        virtual void setPageable(
            const ::jxx::Ptr<Pageable>& document) = 0;
        virtual ::jxx::lang::jbool printDialog() = 0;
        virtual ::jxx::Ptr<PageFormat> pageDialog(
            const ::jxx::Ptr<PageFormat>& page) = 0;
        virtual ::jxx::Ptr<PageFormat> defaultPage(
            const ::jxx::Ptr<PageFormat>& page) = 0;
        ::jxx::Ptr<PageFormat> defaultPage();
        virtual ::jxx::Ptr<PageFormat> validatePage(
            const ::jxx::Ptr<PageFormat>& page) = 0;
        virtual void print() = 0;
        virtual void setCopies(::jxx::lang::jint copies) = 0;
        virtual ::jxx::lang::jint getCopies() const = 0;
        virtual ::jxx::Ptr<::jxx::lang::String> getUserName() const = 0;
        virtual void setJobName(
            const ::jxx::Ptr<::jxx::lang::String>& jobName) = 0;
        virtual ::jxx::Ptr<::jxx::lang::String> getJobName() const = 0;
        virtual void cancel() = 0;
        virtual ::jxx::lang::jbool isCancelled() const = 0;

    protected:
        PrinterJob();
    };
}
