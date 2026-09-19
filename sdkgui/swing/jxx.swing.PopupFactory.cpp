#include <mutex>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/jxx.swing.PopupFactory.h"
namespace { std::mutex gPopupFactoryMutex; ::jxx::Ptr<::jxx::swing::PopupFactory> gPopupFactory; }
namespace jxx::swing
{
    void PopupFactory::setSharedInstance(const ::jxx::Ptr<PopupFactory>& factory)
    {
        if (!factory) throw ::jxx::lang::IllegalArgumentException("factory");
        std::lock_guard<std::mutex> lock(gPopupFactoryMutex); gPopupFactory = factory;
    }
    ::jxx::Ptr<PopupFactory> PopupFactory::getSharedInstance()
    {
        std::lock_guard<std::mutex> lock(gPopupFactoryMutex);
        if (!gPopupFactory) gPopupFactory = ::jxx::NEW<PopupFactory>();
        return gPopupFactory;
    }
    ::jxx::Ptr<Popup> PopupFactory::getPopup(const ::jxx::Ptr<::jxx::awt::Component>& owner,
        const ::jxx::Ptr<::jxx::awt::Component>& contents, ::jxx::lang::jint x, ::jxx::lang::jint y)
    {
        if (!contents) throw ::jxx::lang::IllegalArgumentException("contents");
        return ::jxx::NEW<Popup>(owner, contents, x, y);
    }
}
