#include "util/jxx.util.LocaleBuilder.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.StringPool.h"

namespace jxx::util {

LocaleBuilder::LocaleBuilder()
    : language_(StringPool::intern("")),
      script_(StringPool::intern("")),
      region_(StringPool::intern("")),
      variant_(StringPool::intern("")) {}

jxx::Ptr<LocaleBuilder> LocaleBuilder::setLanguage(const jxx::Ptr<jxx::lang::String>& language) {
    language_ = language ? language : StringPool::intern("");
    return getThis<LocaleBuilder>();
}

jxx::Ptr<LocaleBuilder> LocaleBuilder::setScript(const jxx::Ptr<jxx::lang::String>& script) {
    script_ = script ? script : StringPool::intern("");
    return getThis<LocaleBuilder>();
}

jxx::Ptr<LocaleBuilder> LocaleBuilder::setRegion(const jxx::Ptr<jxx::lang::String>& region) {
    region_ = region ? region : StringPool::intern("");
    return getThis<LocaleBuilder>();
}

jxx::Ptr<LocaleBuilder> LocaleBuilder::setVariant(const jxx::Ptr<jxx::lang::String>& variant) {
    variant_ = variant ? variant : StringPool::intern("");
    return getThis<LocaleBuilder>();
}

jxx::Ptr<LocaleBuilder> LocaleBuilder::clear() {
    language_ = StringPool::intern("");
    script_ = StringPool::intern("");
    region_ = StringPool::intern("");
    variant_ = StringPool::intern("");
    return getThis<LocaleBuilder>();
}

jxx::Ptr<Locale> LocaleBuilder::build() {
    return Locale::intern(language_, script_, region_, variant_);
}

} // namespace jxx::util
