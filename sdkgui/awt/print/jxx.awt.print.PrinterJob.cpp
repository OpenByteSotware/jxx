#include "awt/print/jxx.awt.print.PrinterJob.h"

#include "awt/print/jxx.awt.print.PageFormat.h"

namespace jxx::awt::print
{
    namespace
    {
        class DefaultPrinterJob final : public PrinterJob
        {
        public:
            void setPrintable(const ::jxx::Ptr<Printable>& painter) override
            { painter_ = painter; format_.reset(); document_.reset(); }
            void setPrintable(const ::jxx::Ptr<Printable>& painter,
                const ::jxx::Ptr<PageFormat>& format) override
            { painter_=painter;format_=format;document_.reset(); }
            void setPageable(const ::jxx::Ptr<Pageable>& document) override
            { document_=document;painter_.reset();format_.reset(); }
            ::jxx::lang::jbool printDialog() override{return true;}
            ::jxx::Ptr<PageFormat> pageDialog(
                const ::jxx::Ptr<PageFormat>& page) override
            { return page == nullptr ? PrinterJob::defaultPage() : page; }
            ::jxx::Ptr<PageFormat> defaultPage(
                const ::jxx::Ptr<PageFormat>& page) override
            { return page==nullptr?::jxx::NEW<PageFormat>():
                ::jxx::CAST<PageFormat>(page->clone()); }
            ::jxx::Ptr<PageFormat> validatePage(
                const ::jxx::Ptr<PageFormat>& page) override
            { return defaultPage(page); }
            void print() override{}
            void setCopies(::jxx::lang::jint value) override
            { copies_=value>0?value:1; }
            ::jxx::lang::jint getCopies() const override{return copies_;}
            ::jxx::Ptr<::jxx::lang::String> getUserName() const override
            { return ::jxx::NEW<::jxx::lang::String>(""); }
            void setJobName(
                const ::jxx::Ptr<::jxx::lang::String>& value) override
            { jobName_=value; }
            ::jxx::Ptr<::jxx::lang::String> getJobName() const override
            { return jobName_; }
            void cancel() override{cancelled_=true;}
            ::jxx::lang::jbool isCancelled() const override
            { return cancelled_; }
        private:
            ::jxx::Ptr<Printable> painter_;
            ::jxx::Ptr<PageFormat> format_;
            ::jxx::Ptr<Pageable> document_;
            ::jxx::Ptr<::jxx::lang::String> jobName_ =
                ::jxx::NEW<::jxx::lang::String>("JXX print job");
            ::jxx::lang::jint copies_=1;
            ::jxx::lang::jbool cancelled_=false;
        };
    }

    PrinterJob::PrinterJob() : Super()
    {
    }

    ::jxx::Ptr<PrinterJob> PrinterJob::getPrinterJob()
    {
        return ::jxx::NEW<DefaultPrinterJob>();
    }

    ::jxx::Ptr<PageFormat> PrinterJob::defaultPage()
    {
        return defaultPage(::jxx::NEW<PageFormat>());
    }
}
