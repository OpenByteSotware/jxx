#include "gui/internal/jxx.gui.internal.GuiRuntime.h"
#include <wx/app.h>
#include <wx/init.h>
#include <wx/thread.h>
namespace jxx::gui::internal { ::jxx::lang::jint GuiRuntime::run(){ static wxInitializer initializer; if(!initializer.IsOk()) return -1; return wxTheApp ? static_cast<::jxx::lang::jint>(wxTheApp->OnRun()) : 0; } void GuiRuntime::exit(){if(wxTheApp) wxTheApp->ExitMainLoop();} ::jxx::lang::jbool GuiRuntime::isEventDispatchThread(){return wxIsMainThread();} }
