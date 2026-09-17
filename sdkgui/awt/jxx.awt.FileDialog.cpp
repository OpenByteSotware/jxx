#include "awt/jxx.awt.FileDialog.h"

#include <sstream>

#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    namespace
    {
        ::jxx::Ptr<::jxx::lang::String> emptyTitle()
        {
            return ::jxx::NEW<::jxx::lang::String>("");
        }
    }

    FileDialog::FileDialog(const ::jxx::Ptr<Frame>& parent)
        : FileDialog(parent, emptyTitle(), LOAD)
    {
    }

    FileDialog::FileDialog(const ::jxx::Ptr<Frame>& parent,
        const ::jxx::Ptr<::jxx::lang::String>& title)
        : FileDialog(parent, title, LOAD)
    {
    }

    FileDialog::FileDialog(const ::jxx::Ptr<Frame>& parent,
        const ::jxx::Ptr<::jxx::lang::String>& title,
        ::jxx::lang::jint mode)
        : Super(parent, title, true), mode_(LOAD), multipleMode_(false)
    {
        setMode(mode);
    }

    FileDialog::FileDialog(const ::jxx::Ptr<Dialog>& parent)
        : FileDialog(parent, emptyTitle(), LOAD)
    {
    }

    FileDialog::FileDialog(const ::jxx::Ptr<Dialog>& parent,
        const ::jxx::Ptr<::jxx::lang::String>& title)
        : FileDialog(parent, title, LOAD)
    {
    }

    FileDialog::FileDialog(const ::jxx::Ptr<Dialog>& parent,
        const ::jxx::Ptr<::jxx::lang::String>& title,
        ::jxx::lang::jint mode)
        : Super(parent, title, true), mode_(LOAD), multipleMode_(false)
    {
        setMode(mode);
    }

    void FileDialog::addNotify()
    {
        validate();
    }

    ::jxx::lang::jint FileDialog::getMode() const
    {
        return mode_;
    }

    void FileDialog::setMode(::jxx::lang::jint mode)
    {
        if (mode != LOAD && mode != SAVE)
            throw ::jxx::lang::IllegalArgumentException("mode");
        mode_ = mode;
    }

    ::jxx::Ptr<::jxx::lang::String> FileDialog::getDirectory() const
    {
        return directory_;
    }

    void FileDialog::setDirectory(
        const ::jxx::Ptr<::jxx::lang::String>& directory)
    {
        directory_ = directory;
    }

    ::jxx::Ptr<::jxx::lang::String> FileDialog::getFile() const
    {
        return file_;
    }

    void FileDialog::setFile(
        const ::jxx::Ptr<::jxx::lang::String>& file)
    {
        file_ = file;
    }

    ::jxx::Ptr<::jxx::io::FilenameFilter>
    FileDialog::getFilenameFilter() const
    {
        return filenameFilter_;
    }

    void FileDialog::setFilenameFilter(
        const ::jxx::Ptr<::jxx::io::FilenameFilter>& filter)
    {
        filenameFilter_ = filter;
    }

    ::jxx::lang::jbool FileDialog::isMultipleMode() const
    {
        return multipleMode_;
    }

    void FileDialog::setMultipleMode(::jxx::lang::jbool enable)
    {
        multipleMode_ = enable;
    }

    ::jxx::Ptr<::jxx::lang::String> FileDialog::paramString() const
    {
        std::ostringstream out;
        out << "mode=" << (mode_ == LOAD ? "load" : "save")
            << ",directory="
            << (directory_ != nullptr ? directory_->utf8() : "")
            << ",file=" << (file_ != nullptr ? file_->utf8() : "")
            << ",multipleMode=" << (multipleMode_ ? "true" : "false");
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
