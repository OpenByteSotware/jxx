#include <cstdlib>
#include <memory>
#include <gtest/gtest.h>
#include <wx/app.h>
#include <wx/init.h>
#include <wx/timer.h>
#include "awt/jxx.awt.Button.h"
#include "awt/jxx.awt.Dialog.h"
#include "awt/jxx.awt.FlowLayout.h"
#include "awt/jxx.awt.Frame.h"
#include "lang/jxx.lang.String.h"

class JxxGuiTestApplication final : public wxApp
{
public: bool OnInit() override
{
    return true;
}
};
wxIMPLEMENT_APP_NO_MAIN(JxxGuiTestApplication);

namespace
{
    ::jxx::Ptr<::jxx::lang::String> text(const char* value)
    {
        return ::jxx::NEW<::jxx::lang::String>(value);
    }
    class DialogCloseTimer final : public wxTimer
    {
    public:
        explicit DialogCloseTimer(const ::jxx::Ptr<::jxx::awt::Dialog>& dialog)
            : dialog_(dialog)
        {
        }
        void Notify() override
        {
            if (const auto dialog = dialog_.lock()) {
                closeRequested_ = true;
                dialog->setVisible(false);
            }
        }
        bool closeRequested() const noexcept
        {
            return closeRequested_;
        }
    private:
        std::weak_ptr<::jxx::awt::Dialog> dialog_;
        bool closeRequested_ = false;
    };
}

TEST(FrameDialogIntegrationTest, OpensFrameAndAutomaticallyClosesModalDialog)
{
    ASSERT_NE(wxTheApp, nullptr);
    ASSERT_TRUE(wxIsMainThread());

    auto frame = ::jxx::NEW<::jxx::awt::Frame>(text("JXX GUI Integration Test"));
    frame->setLayout(::jxx::NEW<::jxx::awt::FlowLayout>());
    frame->setBounds(100, 100, 420, 180);
    frame->add(::jxx::NEW<::jxx::awt::Button>(text("Dialog Test")));

    auto dialog = ::jxx::NEW<::jxx::awt::Dialog>(
        frame, text("Automatic Dialog Test"), true);
    dialog->setLayout(::jxx::NEW<::jxx::awt::FlowLayout>());
    dialog->setBounds(150, 150, 300, 140);
    dialog->add(::jxx::NEW<::jxx::awt::Button>(text("Close")));

    frame->setVisible(true);
    ASSERT_TRUE(frame->isVisible());

    DialogCloseTimer timer(dialog);
    ASSERT_TRUE(timer.StartOnce(250));
    dialog->setVisible(true);

    EXPECT_TRUE(timer.closeRequested());
    EXPECT_FALSE(dialog->isVisible());

    dialog->dispose();
    frame->dispose();
    wxTheApp->ProcessPendingEvents();
}
