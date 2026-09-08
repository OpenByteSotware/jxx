#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
namespace jxx::org::w3c::dom {
class CharacterData : public jxx::lang::InterfaceBase<CharacterData, Node> {
public:
    ~CharacterData() override = default;
    virtual jxx::Ptr<jxx::lang::String> getData() const = 0;
    virtual void setData(const jxx::Ptr<jxx::lang::String>& data) = 0;
    virtual jxx::lang::jint getLength() const = 0;
    virtual jxx::Ptr<jxx::lang::String> substringData(jxx::lang::jint offset, jxx::lang::jint count) const = 0;
    virtual void appendData(const jxx::Ptr<jxx::lang::String>& data) = 0;
    virtual void insertData(jxx::lang::jint offset, const jxx::Ptr<jxx::lang::String>& data) = 0;
    virtual void deleteData(jxx::lang::jint offset, jxx::lang::jint count) = 0;
    virtual void replaceData(jxx::lang::jint offset, jxx::lang::jint count, const jxx::Ptr<jxx::lang::String>& data) = 0;
};
}
