#pragma once
#include "ext/xml/transform/jxx.ext.xml.transform.Source.h"
#include "ext/xml/transform/jxx.ext.xml.transform.Transformer.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::ext::xml::transform {
class TransformerFactory : public ::jxx::lang::ClassBase<TransformerFactory,::jxx::lang::Object> {
public:
    virtual ~TransformerFactory() override=default;
    static ::jxx::Ptr<TransformerFactory> newInstance();
    virtual ::jxx::Ptr<Transformer> newTransformer()=0;
    virtual ::jxx::Ptr<Transformer> newTransformer(const ::jxx::Ptr<Source>& source)=0;
    virtual void setAttribute(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<::jxx::lang::Object>& value)=0;
    virtual ::jxx::Ptr<::jxx::lang::Object> getAttribute(const ::jxx::Ptr<::jxx::lang::String>& name) const=0;
    virtual void setFeature(const ::jxx::Ptr<::jxx::lang::String>& name,::jxx::lang::jbool value)=0;
    virtual ::jxx::lang::jbool getFeature(const ::jxx::Ptr<::jxx::lang::String>& name) const=0;
protected:
    TransformerFactory()=default;
};
}
