#include "util/jxx.util.LocaleData.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.StringPool.h"

namespace jxx::util {
namespace {
    struct LangRow { const char* iso2; const char* iso3; const char* name; };
    struct CountryRow { const char* iso2; const char* iso3; const char* name; };

    static const LangRow LANG_ROWS[] = {
        {"ar", "ara", "Arabic"},
        {"de", "deu", "German"},
        {"en", "eng", "English"},
        {"es", "spa", "Spanish"},
        {"fr", "fra", "French"},
        {"it", "ita", "Italian"},
        {"ja", "jpn", "Japanese"},
        {"ko", "kor", "Korean"},
        {"pt", "por", "Portuguese"},
        {"ru", "rus", "Russian"},
        {"zh", "zho", "Chinese"}
    };

    static const CountryRow COUNTRY_ROWS[] = {
        {"BR", "BRA", "Brazil"},
        {"CA", "CAN", "Canada"},
        {"CN", "CHN", "China"},
        {"DE", "DEU", "Germany"},
        {"ES", "ESP", "Spain"},
        {"FR", "FRA", "France"},
        {"GB", "GBR", "United Kingdom"},
        {"IT", "ITA", "Italy"},
        {"JP", "JPN", "Japan"},
        {"KR", "KOR", "South Korea"},
        {"RU", "RUS", "Russia"},
        {"US", "USA", "United States"}
    };

    template <typename Row>
    jxx::Ptr<jxx::lang::String> findCode2(const Row* rows, int count, const jxx::Ptr<jxx::lang::String>& code, bool returnName) {
        if (!code) {
            return jxx::Ptr<jxx::lang::String>();
        }
        for (int i = 0; i < count; ++i) {
            if (code->equals(StringPool::intern(rows[i].iso2))) {
                return returnName ? StringPool::intern(rows[i].name)
                                  : StringPool::intern(rows[i].iso3);
            }
        }
        return code;
    }
}

jxx::Ptr<jxx::lang::String> LocaleData::getLanguageName(const jxx::Ptr<jxx::lang::String>& language) {
    return findCode2(LANG_ROWS, static_cast<int>(sizeof(LANG_ROWS) / sizeof(LANG_ROWS[0])), language, true);
}

jxx::Ptr<jxx::lang::String> LocaleData::getCountryName(const jxx::Ptr<jxx::lang::String>& country) {
    return findCode2(COUNTRY_ROWS, static_cast<int>(sizeof(COUNTRY_ROWS) / sizeof(COUNTRY_ROWS[0])), country, true);
}

jxx::Ptr<jxx::lang::String> LocaleData::getISO3Language(const jxx::Ptr<jxx::lang::String>& language) {
    return findCode2(LANG_ROWS, static_cast<int>(sizeof(LANG_ROWS) / sizeof(LANG_ROWS[0])), language, false);
}

jxx::Ptr<jxx::lang::String> LocaleData::getISO3Country(const jxx::Ptr<jxx::lang::String>& country) {
    return findCode2(COUNTRY_ROWS, static_cast<int>(sizeof(COUNTRY_ROWS) / sizeof(COUNTRY_ROWS[0])), country, false);
}

} // namespace jxx::util
