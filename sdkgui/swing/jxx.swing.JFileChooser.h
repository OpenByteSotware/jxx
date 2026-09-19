#pragma once
#include <vector>
#include "swing/jxx.swing.JComponent.h"
namespace jxx::io { class File; }
namespace jxx::swing::filechooser { class FileFilter; }
namespace jxx::swing
{
    class JFileChooser : public ::jxx::lang::ClassBase<JFileChooser, JComponent>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JFileChooser, JxxSuper>;
        static constexpr ::jxx::lang::jint OPEN_DIALOG = 0;
        static constexpr ::jxx::lang::jint SAVE_DIALOG = 1;
        static constexpr ::jxx::lang::jint CUSTOM_DIALOG = 2;
        static constexpr ::jxx::lang::jint CANCEL_OPTION = 1;
        static constexpr ::jxx::lang::jint APPROVE_OPTION = 0;
        static constexpr ::jxx::lang::jint ERROR_OPTION = -1;
        static constexpr ::jxx::lang::jint FILES_ONLY = 0;
        static constexpr ::jxx::lang::jint DIRECTORIES_ONLY = 1;
        static constexpr ::jxx::lang::jint FILES_AND_DIRECTORIES = 2;
        JFileChooser();
        explicit JFileChooser(const ::jxx::Ptr<::jxx::lang::String>& path);
        explicit JFileChooser(const ::jxx::Ptr<::jxx::io::File>& directory);
        void setCurrentDirectory(const ::jxx::Ptr<::jxx::io::File>& directory);
        ::jxx::Ptr<::jxx::io::File> getCurrentDirectory() const;
        void setSelectedFile(const ::jxx::Ptr<::jxx::io::File>& file);
        ::jxx::Ptr<::jxx::io::File> getSelectedFile() const;
        void setMultiSelectionEnabled(::jxx::lang::jbool enabled);
        ::jxx::lang::jbool isMultiSelectionEnabled() const;
        void setFileSelectionMode(::jxx::lang::jint mode);
        ::jxx::lang::jint getFileSelectionMode() const;
        void setDialogTitle(const ::jxx::Ptr<::jxx::lang::String>& title);
        ::jxx::Ptr<::jxx::lang::String> getDialogTitle() const;
        void setApproveButtonText(const ::jxx::Ptr<::jxx::lang::String>& text);
        ::jxx::Ptr<::jxx::lang::String> getApproveButtonText() const;
        void addChoosableFileFilter(
            const ::jxx::Ptr<::jxx::swing::filechooser::FileFilter>& filter);
        void removeChoosableFileFilter(
            const ::jxx::Ptr<::jxx::swing::filechooser::FileFilter>& filter);
        void setFileFilter(
            const ::jxx::Ptr<::jxx::swing::filechooser::FileFilter>& filter);
        ::jxx::Ptr<::jxx::swing::filechooser::FileFilter> getFileFilter() const;
        ::jxx::lang::jint showOpenDialog(
            const ::jxx::Ptr<::jxx::awt::Component>& parent);
        ::jxx::lang::jint showSaveDialog(
            const ::jxx::Ptr<::jxx::awt::Component>& parent);
        ::jxx::lang::jint showDialog(
            const ::jxx::Ptr<::jxx::awt::Component>& parent,
            const ::jxx::Ptr<::jxx::lang::String>& approveButtonText);
        void approveSelection();
        void cancelSelection();
    private:
        ::jxx::Ptr<::jxx::io::File> currentDirectory_;
        ::jxx::Ptr<::jxx::io::File> selectedFile_;
        ::jxx::Ptr<::jxx::lang::String> dialogTitle_;
        ::jxx::Ptr<::jxx::lang::String> approveButtonText_;
        std::vector<::jxx::Ptr<::jxx::swing::filechooser::FileFilter>> filters_;
        ::jxx::Ptr<::jxx::swing::filechooser::FileFilter> activeFilter_;
        ::jxx::lang::jint selectionMode_ = FILES_ONLY;
        ::jxx::lang::jint dialogType_ = OPEN_DIALOG;
        ::jxx::lang::jint result_ = CANCEL_OPTION;
        ::jxx::lang::jbool multiSelection_ = false;
    };
}
