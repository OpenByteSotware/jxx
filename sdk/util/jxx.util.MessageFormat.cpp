#include "util/jxx.util.MessageFormat.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.StringPool.h"

#include <sstream>
#include <string>

namespace jxx::util {

MessageFormat::MessageFormat(const jxx::Ptr<jxx::lang::String>& pattern)
    : pattern_(pattern ? pattern : StringPool::intern("")) {}

jxx::Ptr<MessageFormat> MessageFormat::of(const jxx::Ptr<jxx::lang::String>& pattern) {
    return jxx::NEW<MessageFormat>(pattern);
}

jxx::Ptr<jxx::lang::String> MessageFormat::format(jxx::Ptr<jxx::lang::Object>* args, jxx::lang::jint count) const {
    std::string input = pattern_->utf8();
    std::ostringstream out;

    for (std::size_t i = 0; i < input.size();) {
        if (input[i] == '{') {
            std::size_t end = input.find('}', i + 1U);
            if (end != std::string::npos) {
                std::string token = input.substr(i + 1U, end - i - 1U);
                jxx::lang::jint index = -1;
                try { index = std::stoi(token); } catch (...) { index = -1; }
                if (index >= 0 && index < count && args[index]) {
                    out << args[index]->toString()->utf8();
                    i = end + 1U;
                    continue;
                }
            }
        }
        out << input[i];
        ++i;
    }

    return jxx::lang::String::valueOf(out.str().c_str());
}

} // namespace jxx::util
