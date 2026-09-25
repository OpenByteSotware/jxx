#pragma once
#include "ext/xml/transform/jxx.ext.xml.transform.Result.h"
#include "ext/xml/transform/jxx.ext.xml.transform.Source.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Properties.h"
namespace jxx::ext::xml::transform {
class Transformer : public ::jxx::lang::ClassBase<Transformer, ::jxx::lang::Object> {
public:
    virtual ~Transformer() override = default;
    virtual void transform(const ::jxx::Ptr<Source>& source,const ::jxx::Ptr<Result>& result)=0;
    virtual void setOutputProperty(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<::jxx::lang::String>& value)=0;
    virtual ::jxx::Ptr<::jxx::lang::String> getOutputProperty(const ::jxx::Ptr<::jxx::lang::String>& name) const=0;
    virtual void setOutputProperties(const ::jxx::Ptr<::jxx::util::Properties>& properties)=0;
    virtual ::jxx::Ptr<::jxx::util::Properties> getOutputProperties() const=0;
    virtual void setParameter(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<::jxx::lang::Object>& value)=0;
    virtual ::jxx::Ptr<::jxx::lang::Object> getParameter(const ::jxx::Ptr<::jxx::lang::String>& name) const=0;
    virtual void clearParameters()=0;
    virtual void reset();
protected:
    Transformer()=default;
};
}
