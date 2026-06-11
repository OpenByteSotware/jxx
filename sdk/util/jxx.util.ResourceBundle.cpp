#include "util/jxx.util.ResourceBundle.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.StringPool.h"
#include "examples/messages/Messages_en_US.h"
#include "examples/messages/Messages_fr_FR.h"

namespace jxx::util {

jxx::Ptr<ResourceBundle> ResourceBundle::getBundle(const jxx::Ptr<jxx::lang::String>& baseName,
                                                   const jxx::Ptr<Locale>& locale) {
    if (!baseName || !baseName->equals(StringPool::intern("messages"))) {
        return jxx::Ptr<ResourceBundle>();
    }

    auto loc = locale ? locale : Locale::getDefault();
    if (loc->getLanguage()->equals(StringPool::intern("fr")) &&
        loc->getCountry()->equals(StringPool::intern("FR"))) {
        return std::static_pointer_cast<ResourceBundle>(jxx::NEW<jxx::examples::messages::Messages_fr_FR>());
    }
    return std::static_pointer_cast<ResourceBundle>(jxx::NEW<jxx::examples::messages::Messages_en_US>());
}

} // namespace jxx::util
