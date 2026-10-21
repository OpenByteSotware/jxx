#include "ext/xml/transform/jxx.ext.xml.transform.ClassInitialization.h"

#include <mutex>

#include "ext/xml/transform/jxx.ext.xml.transform.OutputKeys.h"
#include "ext/xml/transform/jxx.ext.xml.transform.Result.h"
#include "ext/xml/transform/jxx.ext.xml.transform.Source.h"
#include "ext/xml/transform/jxx.ext.xml.transform.Transformer.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerConfigurationException.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerException.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerFactory.h"

namespace jxx::ext::xml::transform {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            OutputKeys,
            ::jxx::lang::Object>::Class();
        (void)::jxx::lang::InterfaceBase<Result>::Class();
        (void)::jxx::lang::InterfaceBase<Source>::Class();
        (void)::jxx::lang::ClassBase<
            Transformer,
            ::jxx::lang::Object>::Class();
        (void)::jxx::lang::ClassBase<
            TransformerException,
            ::jxx::lang::Exception>::Class();
        (void)::jxx::lang::ClassBase<
            TransformerConfigurationException,
            TransformerException>::Class();
        (void)::jxx::lang::ClassBase<
            TransformerFactory,
            ::jxx::lang::Object>::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::ext::xml::transform
