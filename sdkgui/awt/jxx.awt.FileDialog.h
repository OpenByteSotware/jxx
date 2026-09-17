#pragma once

#include "awt/jxx.awt.Dialog.h"

namespace jxx::io
{
    class FilenameFilter;
}

namespace jxx::awt
{
    class FileDialog : public ::jxx::lang::ClassBase<FileDialog, Dialog>
    {
    public:
        using JxxSuper = Dialog;
        using Super = ::jxx::lang::ClassBase<FileDialog, JxxSuper>;

        static constexpr ::jxx::lang::jint LOAD = 0;
        static constexpr ::jxx::lang::jint SAVE = 1;

        explicit FileDialog(const ::jxx::Ptr<Frame>& parent);
        FileDialog(const ::jxx::Ptr<Frame>& parent,
            const ::jxx::Ptr<::jxx::lang::String>& title);
        FileDialog(const ::jxx::Ptr<Frame>& parent,
            const ::jxx::Ptr<::jxx::lang::String>& title,
            ::jxx::lang::jint mode);
        explicit FileDialog(const ::jxx::Ptr<Dialog>& parent);
        FileDialog(const ::jxx::Ptr<Dialog>& parent,
            const ::jxx::Ptr<::jxx::lang::String>& title);
        FileDialog(const ::jxx::Ptr<Dialog>& parent,
            const ::jxx::Ptr<::jxx::lang::String>& title,
            ::jxx::lang::jint mode);
        ~FileDialog() override = default;

        virtual void addNotify();
        virtual ::jxx::lang::jint getMode() const;
        virtual void setMode(::jxx::lang::jint mode);
        virtual ::jxx::Ptr<::jxx::lang::String> getDirectory() const;
        virtual void setDirectory(
            const ::jxx::Ptr<::jxx::lang::String>& directory);
        virtual ::jxx::Ptr<::jxx::lang::String> getFile() const;
        virtual void setFile(
            const ::jxx::Ptr<::jxx::lang::String>& file);
        virtual ::jxx::Ptr<::jxx::io::FilenameFilter>
            getFilenameFilter() const;
        virtual void setFilenameFilter(
            const ::jxx::Ptr<::jxx::io::FilenameFilter>& filter);
        virtual ::jxx::lang::jbool isMultipleMode() const;
        virtual void setMultipleMode(::jxx::lang::jbool enable);

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const;

    private:
        ::jxx::lang::jint mode_;
        ::jxx::Ptr<::jxx::lang::String> directory_;
        ::jxx::Ptr<::jxx::lang::String> file_;
        ::jxx::Ptr<::jxx::io::FilenameFilter> filenameFilter_;
        ::jxx::lang::jbool multipleMode_;
    };
}
