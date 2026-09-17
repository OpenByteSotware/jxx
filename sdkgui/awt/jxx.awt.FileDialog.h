#pragma once

#include "awt/jxx.awt.Dialog.h"

namespace jxx::io
{
    class FilenameFilter;
}

namespace jxx::gui::internal
{
    class WxFileDialogPeer;
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
        ::jxx::lang::jint getMode() const;
        void setMode(::jxx::lang::jint mode);
        ::jxx::Ptr<::jxx::lang::String> getDirectory() const;
        void setDirectory(
            const ::jxx::Ptr<::jxx::lang::String>& directory);
        ::jxx::Ptr<::jxx::lang::String> getFile() const;
        void setFile(const ::jxx::Ptr<::jxx::lang::String>& file);
        ::jxx::Ptr<::jxx::io::FilenameFilter> getFilenameFilter() const;
        void setFilenameFilter(
            const ::jxx::Ptr<::jxx::io::FilenameFilter>& filter);
        ::jxx::lang::jbool isMultipleMode() const;
        void setMultipleMode(::jxx::lang::jbool enable);
        void setVisible(::jxx::lang::jbool visible) override;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const;

    private:
        ::jxx::lang::jint mode_;
        ::jxx::Ptr<::jxx::lang::String> directory_;
        ::jxx::Ptr<::jxx::lang::String> file_;
        ::jxx::Ptr<::jxx::io::FilenameFilter> filenameFilter_;
        ::jxx::lang::jbool multipleMode_;
        ::jxx::Ptr<::jxx::gui::internal::WxFileDialogPeer> filePeer_;
    };
}
