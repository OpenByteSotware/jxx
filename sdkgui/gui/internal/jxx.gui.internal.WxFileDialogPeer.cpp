#include "gui/internal/jxx.gui.internal.WxFileDialogPeer.h"

#include <wx/filedlg.h>
#include <wx/string.h>

#include "gui/internal/jxx.gui.internal.GuiRuntime.h"

namespace jxx::gui::internal
{
    namespace
    {
        wxString nativeText(const ::jxx::Ptr<::jxx::lang::String>& value)
        {
            return value != nullptr
                ? wxString::FromUTF8(value->utf8().c_str())
                : wxString();
        }

        ::jxx::Ptr<::jxx::lang::String> text(const wxString& value)
        {
            return ::jxx::NEW<::jxx::lang::String>(value.ToUTF8().data());
        }
    }

    WxFileDialogPeer::WxFileDialogPeer(
        const ::jxx::Ptr<::jxx::lang::String>& title,
        ::jxx::lang::jint mode,
        const ::jxx::Ptr<::jxx::lang::String>& directory,
        const ::jxx::Ptr<::jxx::lang::String>& file,
        ::jxx::lang::jbool multipleMode)
        : title_(title), directory_(directory), file_(file), mode_(mode),
          multipleMode_(multipleMode)
    {
    }

    ::jxx::lang::jbool WxFileDialogPeer::showModal()
    {
        if (!GuiRuntime::ensureInitialized()) return false;

        long style = mode_ == 0
            ? wxFD_OPEN | wxFD_FILE_MUST_EXIST
            : wxFD_SAVE | wxFD_OVERWRITE_PROMPT;
        if (mode_ == 0 && multipleMode_) style |= wxFD_MULTIPLE;

        wxFileDialog dialog(nullptr, nativeText(title_),
            nativeText(directory_), nativeText(file_),
            wxFileSelectorDefaultWildcardStr, style);

        if (dialog.ShowModal() != wxID_OK)
        {
            file_.reset();
            return false;
        }

        directory_ = text(dialog.GetDirectory());
        file_ = text(dialog.GetFilename());
        return true;
    }

    ::jxx::Ptr<::jxx::lang::String>
    WxFileDialogPeer::getDirectory() const
    {
        return directory_;
    }

    ::jxx::Ptr<::jxx::lang::String> WxFileDialogPeer::getFile() const
    {
        return file_;
    }
}
