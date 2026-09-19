#include "swing/jxx.swing.JFileChooser.h"
#include <algorithm>
#include "io/jxx.io.File.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/filechooser/jxx.swing.filechooser.FileFilter.h"
namespace jxx::swing
{
    JFileChooser::JFileChooser() : Super() {}
    JFileChooser::JFileChooser(const ::jxx::Ptr<::jxx::lang::String>& path)
        : JFileChooser(path ? ::jxx::NEW<::jxx::io::File>(path) : nullptr) {}
    JFileChooser::JFileChooser(const ::jxx::Ptr<::jxx::io::File>& directory)
        : Super(), currentDirectory_(directory) {}
    void JFileChooser::setCurrentDirectory(
        const ::jxx::Ptr<::jxx::io::File>& value) { currentDirectory_ = value; }
    ::jxx::Ptr<::jxx::io::File> JFileChooser::getCurrentDirectory() const
    { return currentDirectory_; }
    void JFileChooser::setSelectedFile(const ::jxx::Ptr<::jxx::io::File>& value)
    { selectedFile_ = value; }
    ::jxx::Ptr<::jxx::io::File> JFileChooser::getSelectedFile() const
    { return selectedFile_; }
    void JFileChooser::setMultiSelectionEnabled(::jxx::lang::jbool value)
    { multiSelection_ = value; }
    ::jxx::lang::jbool JFileChooser::isMultiSelectionEnabled() const
    { return multiSelection_; }
    void JFileChooser::setFileSelectionMode(::jxx::lang::jint value)
    {
        if (value < FILES_ONLY || value > FILES_AND_DIRECTORIES)
            throw ::jxx::lang::IllegalArgumentException("mode");
        selectionMode_ = value;
    }
    ::jxx::lang::jint JFileChooser::getFileSelectionMode() const
    { return selectionMode_; }
    void JFileChooser::setDialogTitle(
        const ::jxx::Ptr<::jxx::lang::String>& value) { dialogTitle_ = value; }
    ::jxx::Ptr<::jxx::lang::String> JFileChooser::getDialogTitle() const
    { return dialogTitle_; }
    void JFileChooser::setApproveButtonText(
        const ::jxx::Ptr<::jxx::lang::String>& value) { approveButtonText_ = value; }
    ::jxx::Ptr<::jxx::lang::String> JFileChooser::getApproveButtonText() const
    { return approveButtonText_; }
    void JFileChooser::addChoosableFileFilter(
        const ::jxx::Ptr<::jxx::swing::filechooser::FileFilter>& filter)
    {
        if (filter && std::find(filters_.begin(), filters_.end(), filter)
            == filters_.end()) filters_.push_back(filter);
    }
    void JFileChooser::removeChoosableFileFilter(
        const ::jxx::Ptr<::jxx::swing::filechooser::FileFilter>& filter)
    {
        filters_.erase(std::remove(filters_.begin(), filters_.end(), filter),
            filters_.end());
        if (activeFilter_ == filter) activeFilter_.reset();
    }
    void JFileChooser::setFileFilter(
        const ::jxx::Ptr<::jxx::swing::filechooser::FileFilter>& filter)
    { activeFilter_ = filter; addChoosableFileFilter(filter); }
    ::jxx::Ptr<::jxx::swing::filechooser::FileFilter>
    JFileChooser::getFileFilter() const { return activeFilter_; }
    ::jxx::lang::jint JFileChooser::showOpenDialog(
        const ::jxx::Ptr<::jxx::awt::Component>& parent)
    { dialogType_ = OPEN_DIALOG; return showDialog(parent, nullptr); }
    ::jxx::lang::jint JFileChooser::showSaveDialog(
        const ::jxx::Ptr<::jxx::awt::Component>& parent)
    { dialogType_ = SAVE_DIALOG; return showDialog(parent, nullptr); }
    ::jxx::lang::jint JFileChooser::showDialog(
        const ::jxx::Ptr<::jxx::awt::Component>&,
        const ::jxx::Ptr<::jxx::lang::String>& approveText)
    {
        if (approveText) approveButtonText_ = approveText;
        result_ = selectedFile_ ? APPROVE_OPTION : CANCEL_OPTION;
        return result_;
    }
    void JFileChooser::approveSelection() { result_ = APPROVE_OPTION; }
    void JFileChooser::cancelSelection() { result_ = CANCEL_OPTION; }
}
