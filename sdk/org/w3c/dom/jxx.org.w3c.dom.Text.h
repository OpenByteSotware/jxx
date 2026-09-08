#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "org/w3c/dom/jxx.org.w3c.dom.CharacterData.h"
namespace jxx::org::w3c::dom {
class Text : public jxx::lang::InterfaceBase<Text, CharacterData> {
public:
    ~Text() override = default;
    virtual jxx::Ptr<Text> splitText(jxx::lang::jint offset) = 0;
    virtual jxx::lang::jbool isElementContentWhitespace() const = 0;
    virtual jxx::Ptr<jxx::lang::String> getWholeText() const = 0;
    virtual jxx::Ptr<Text> replaceWholeText(const jxx::Ptr<jxx::lang::String>& content) = 0;
};
}
