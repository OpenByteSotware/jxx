#pragma once
#include <mutex>
#include <vector>
#include "awt/jxx.awt.KeyEventDispatcher.h"
#include "awt/jxx.awt.KeyEventPostProcessor.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::awt::event { class KeyEvent; }
namespace jxx::awt {
class AWTEvent; class Component; class Container; class FocusTraversalPolicy; class Window;
class KeyboardFocusManager : public ::jxx::lang::ClassBase<KeyboardFocusManager,::jxx::lang::Object,KeyEventDispatcher,KeyEventPostProcessor> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<KeyboardFocusManager,JxxSuper,KeyEventDispatcher,KeyEventPostProcessor>;
 static constexpr ::jxx::lang::jint FORWARD_TRAVERSAL_KEYS=0,BACKWARD_TRAVERSAL_KEYS=1,UP_CYCLE_TRAVERSAL_KEYS=2,DOWN_CYCLE_TRAVERSAL_KEYS=3;
 ~KeyboardFocusManager() override=default;
 static ::jxx::Ptr<KeyboardFocusManager> getCurrentKeyboardFocusManager(); static void setCurrentKeyboardFocusManager(const ::jxx::Ptr<KeyboardFocusManager>& manager);
 ::jxx::Ptr<Component> getFocusOwner() const; ::jxx::Ptr<Component> getPermanentFocusOwner() const; ::jxx::Ptr<Window> getFocusedWindow() const; ::jxx::Ptr<Window> getActiveWindow() const; ::jxx::Ptr<Container> getCurrentFocusCycleRoot() const; void clearFocusOwner(); void clearGlobalFocusOwner();
 void setDefaultFocusTraversalPolicy(const ::jxx::Ptr<FocusTraversalPolicy>& policy); ::jxx::Ptr<FocusTraversalPolicy> getDefaultFocusTraversalPolicy() const;
 void addKeyEventDispatcher(const ::jxx::Ptr<KeyEventDispatcher>& dispatcher); void removeKeyEventDispatcher(const ::jxx::Ptr<KeyEventDispatcher>& dispatcher); void addKeyEventPostProcessor(const ::jxx::Ptr<KeyEventPostProcessor>& processor); void removeKeyEventPostProcessor(const ::jxx::Ptr<KeyEventPostProcessor>& processor);
 virtual ::jxx::lang::jbool dispatchEvent(const ::jxx::Ptr<AWTEvent>& event)=0; virtual void processKeyEvent(const ::jxx::Ptr<Component>& focusedComponent,const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event)=0; virtual void focusNextComponent(const ::jxx::Ptr<Component>& component)=0; virtual void focusPreviousComponent(const ::jxx::Ptr<Component>& component)=0; virtual void upFocusCycle(const ::jxx::Ptr<Component>& component)=0; virtual void downFocusCycle(const ::jxx::Ptr<Container>& container)=0;
protected:
 KeyboardFocusManager(); void setGlobalFocusOwner(const ::jxx::Ptr<Component>& component); void setGlobalPermanentFocusOwner(const ::jxx::Ptr<Component>& component); void setGlobalCurrentFocusCycleRoot(const ::jxx::Ptr<Container>& container); ::jxx::lang::jbool dispatchThroughChain(const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event); ::jxx::lang::jbool postProcessThroughChain(const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event);
private:
 static std::mutex currentMutex_; static ::jxx::Ptr<KeyboardFocusManager> currentManager_; ::jxx::Ptr<Component> focusOwner_,permanentFocusOwner_; ::jxx::Ptr<Window> focusedWindow_,activeWindow_; ::jxx::Ptr<Container> currentFocusCycleRoot_; ::jxx::Ptr<FocusTraversalPolicy> defaultFocusTraversalPolicy_; std::vector<::jxx::Ptr<KeyEventDispatcher>> dispatchers_; std::vector<::jxx::Ptr<KeyEventPostProcessor>> postProcessors_;
}; }
