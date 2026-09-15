#include "swing/jxx.swing.SwingUtilities.h"
#include "gui/internal/jxx.gui.internal.GuiRuntime.h"
namespace jxx::swing
{
	::jxx::lang::jint SwingUtilities::run()
	{
		return ::jxx::gui::internal::GuiRuntime::run();
	} ::jxx::lang::jbool SwingUtilities::isEventDispatchThread()
	{
		return ::jxx::gui::internal::GuiRuntime::isEventDispatchThread();
	}
}
