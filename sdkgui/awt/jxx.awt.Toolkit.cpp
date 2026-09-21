#include "awt/jxx.awt.Toolkit.h"

#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.EventQueue.h"
#include "awt/jxx.awt.FontMetrics.h"

namespace jxx::awt
{
    namespace
    {
        class PortableToolkit final : public Toolkit
        {
        public:
            ::jxx::Ptr<Dimension> getScreenSize()const override
            { return ::jxx::NEW<Dimension>(); }
            ::jxx::lang::jint getScreenResolution()const override{return 96;}
            ::jxx::Ptr<FontMetrics> getFontMetrics(
                const ::jxx::Ptr<Font>&)const override{return nullptr;}
            void sync() override{}
            void beep() override{}
        };
    }
    Toolkit::Toolkit():Super(){}
    ::jxx::Ptr<Toolkit> Toolkit::getDefaultToolkit()
    {static auto toolkit=::jxx::NEW<PortableToolkit>();return toolkit;}
    ::jxx::Ptr<::jxx::lang::String> Toolkit::getProperty(
        const ::jxx::Ptr<::jxx::lang::String>&,
        const ::jxx::Ptr<::jxx::lang::String>& value){return value;}
    ::jxx::Ptr<EventQueue> Toolkit::getSystemEventQueue()const
    {return EventQueue::getSystemEventQueue();}
    ::jxx::lang::jbool Toolkit::areExtraMouseButtonsEnabled()const{return true;}
}
