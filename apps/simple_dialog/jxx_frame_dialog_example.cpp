#include "awt/event/jxx.awt.event.ActionEvent.h"
#include "awt/event/jxx.awt.event.ActionListener.h"
#include "awt/jxx.awt.Button.h"
#include "awt/jxx.awt.Dialog.h"
#include "awt/jxx.awt.FlowLayout.h"
#include "awt/jxx.awt.Frame.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "swing/jxx.swing.SwingUtilities.h"

namespace
{
    ::jxx::Ptr<::jxx::lang::String> text(const char* value)
    {
        return ::jxx::NEW<::jxx::lang::String>(value);
    }

    class CloseListener final : public ::jxx::lang::ClassBase<
        CloseListener, ::jxx::lang::Object,
        ::jxx::awt::event::ActionListener>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        explicit CloseListener(const ::jxx::Ptr<::jxx::awt::Dialog>& dialog)
            : dialog_(dialog) {}
        void actionPerformed(
            const ::jxx::Ptr<::jxx::awt::event::ActionEvent>&) override
        {
            if (const auto dialog = dialog_.lock()) dialog->setVisible(false);
        }
    private:
        std::weak_ptr<::jxx::awt::Dialog> dialog_;
    };

    class OpenListener final : public ::jxx::lang::ClassBase<
        OpenListener, ::jxx::lang::Object,
        ::jxx::awt::event::ActionListener>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        explicit OpenListener(const ::jxx::Ptr<::jxx::awt::Dialog>& dialog)
            : dialog_(dialog) {}
        void actionPerformed(
            const ::jxx::Ptr<::jxx::awt::event::ActionEvent>&) override
        {
            if (const auto dialog = dialog_.lock()) dialog->setVisible(true);
        }
    private:
        std::weak_ptr<::jxx::awt::Dialog> dialog_;
    };
}

int main()
{
    auto frame = ::jxx::NEW<::jxx::awt::Frame>(text("JXX Frame"));
    frame->setLayout(::jxx::NEW<::jxx::awt::FlowLayout>());
    frame->setBounds(100, 100, 420, 180);

    auto dialog = ::jxx::NEW<::jxx::awt::Dialog>(
        frame, text("JXX Modal Dialog"), true);
    dialog->setLayout(::jxx::NEW<::jxx::awt::FlowLayout>());
    dialog->setBounds(150, 150, 300, 140);

    auto openButton = ::jxx::NEW<::jxx::awt::Button>(text("Open Dialog"));
    openButton->setSize(140, 32);
    openButton->addActionListener(::jxx::NEW<OpenListener>(dialog));
    frame->add(openButton);

    auto closeButton = ::jxx::NEW<::jxx::awt::Button>(text("Close"));
    closeButton->setSize(100, 32);
    closeButton->addActionListener(::jxx::NEW<CloseListener>(dialog));
    dialog->add(closeButton);

    frame->setVisible(true);
    return ::jxx::swing::SwingUtilities::run();
}
