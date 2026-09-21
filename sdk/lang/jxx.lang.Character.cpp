#include "lang/jxx.lang.Character.h"

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

#include <memory>
#include <cctype>
#include <string>


#include <gsl/util>
#include <gsl/narrow>

#include "lang/jxx.lang.String.h"
#include "jxx.lang.IllegalArgumentException.h"
#include "jxx.unicode_bridge.h"

namespace
{
    [[noreturn]] void throwIAE_(const char* message)
    {
        throw jxx::lang::IllegalArgumentException(message);
    }
}

namespace jxx::lang
{
    namespace {
        std::string normalizedUnicodeName(const jxx::Ptr<String>& value) {
            if (value == nullptr) {
                throw NullPointerException();
            }
            auto name = value->utf8();
            for (auto& ch : name) {
                if (ch == ' ' || ch == '-') ch = '_';
                else ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
            }
            return name;
        }
    }

    Character::Subset::Subset(const jxx::Ptr<String>& name)
        : ClassBase<Subset, Object>(), name_(name) {
        if (name_ == nullptr) throw NullPointerException();
    }

    jxx::Ptr<String> Character::Subset::toString() const {
        return name_;
    }

    Character::UnicodeBlock::UnicodeBlock(
        const jxx::Ptr<String>& name, jint start, jint end)
        : ClassBase<UnicodeBlock, Subset>(name), start_(start), end_(end) {}

    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BASIC_LATIN(
        new Character::UnicodeBlock(jxx::NEW<String>("BASIC_LATIN"), 0x0000, 0x007F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LATIN_1_SUPPLEMENT(
        new Character::UnicodeBlock(jxx::NEW<String>("LATIN_1_SUPPLEMENT"), 0x0080, 0x00FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GREEK(
        new Character::UnicodeBlock(jxx::NEW<String>("GREEK"), 0x0370, 0x03FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CYRILLIC(
        new Character::UnicodeBlock(jxx::NEW<String>("CYRILLIC"), 0x0400, 0x04FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HEBREW(
        new Character::UnicodeBlock(jxx::NEW<String>("HEBREW"), 0x0590, 0x05FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ARABIC(
        new Character::UnicodeBlock(jxx::NEW<String>("ARABIC"), 0x0600, 0x06FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GENERAL_PUNCTUATION(
        new Character::UnicodeBlock(jxx::NEW<String>("GENERAL_PUNCTUATION"), 0x2000, 0x206F));

    jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::of(jint codePoint) {
        if (!Character::isValidCodePoint(codePoint)) throw IllegalArgumentException();
        const jxx::Ptr<UnicodeBlock> blocks[] = {
            BASIC_LATIN, LATIN_1_SUPPLEMENT, GREEK, CYRILLIC,
            HEBREW, ARABIC, GENERAL_PUNCTUATION
        };
        for (const auto& block : blocks) {
            if (codePoint >= block->start_ && codePoint <= block->end_) return block;
        }
        return nullptr;
    }

    jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::forName(
        const jxx::Ptr<String>& blockName) {
        const auto requested = normalizedUnicodeName(blockName);
        const jxx::Ptr<UnicodeBlock> blocks[] = {
            BASIC_LATIN, LATIN_1_SUPPLEMENT, GREEK, CYRILLIC,
            HEBREW, ARABIC, GENERAL_PUNCTUATION
        };
        for (const auto& block : blocks) {
            if (normalizedUnicodeName(block->toString()) == requested) return block;
        }
        throw IllegalArgumentException(blockName);
    }

    Character::UnicodeScript::UnicodeScript(
        const jxx::Ptr<String>& name, jint ordinal)
        : Enum<UnicodeScript>(name, ordinal) {}

    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::COMMON(
        new Character::UnicodeScript(jxx::NEW<String>("COMMON"), 0));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::LATIN(
        new Character::UnicodeScript(jxx::NEW<String>("LATIN"), 1));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::GREEK(
        new Character::UnicodeScript(jxx::NEW<String>("GREEK"), 2));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::CYRILLIC(
        new Character::UnicodeScript(jxx::NEW<String>("CYRILLIC"), 3));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::ARMENIAN(
        new Character::UnicodeScript(jxx::NEW<String>("ARMENIAN"), 4));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::HEBREW(
        new Character::UnicodeScript(jxx::NEW<String>("HEBREW"), 5));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::ARABIC(
        new Character::UnicodeScript(jxx::NEW<String>("ARABIC"), 6));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::SYRIAC(
        new Character::UnicodeScript(jxx::NEW<String>("SYRIAC"), 7));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::THAANA(
        new Character::UnicodeScript(jxx::NEW<String>("THAANA"), 8));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::DEVANAGARI(
        new Character::UnicodeScript(jxx::NEW<String>("DEVANAGARI"), 9));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::BENGALI(
        new Character::UnicodeScript(jxx::NEW<String>("BENGALI"), 10));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::GURMUKHI(
        new Character::UnicodeScript(jxx::NEW<String>("GURMUKHI"), 11));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::GUJARATI(
        new Character::UnicodeScript(jxx::NEW<String>("GUJARATI"), 12));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::ORIYA(
        new Character::UnicodeScript(jxx::NEW<String>("ORIYA"), 13));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::TAMIL(
        new Character::UnicodeScript(jxx::NEW<String>("TAMIL"), 14));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::TELUGU(
        new Character::UnicodeScript(jxx::NEW<String>("TELUGU"), 15));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::KANNADA(
        new Character::UnicodeScript(jxx::NEW<String>("KANNADA"), 16));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::MALAYALAM(
        new Character::UnicodeScript(jxx::NEW<String>("MALAYALAM"), 17));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::SINHALA(
        new Character::UnicodeScript(jxx::NEW<String>("SINHALA"), 18));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::THAI(
        new Character::UnicodeScript(jxx::NEW<String>("THAI"), 19));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::LAO(
        new Character::UnicodeScript(jxx::NEW<String>("LAO"), 20));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::TIBETAN(
        new Character::UnicodeScript(jxx::NEW<String>("TIBETAN"), 21));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::MYANMAR(
        new Character::UnicodeScript(jxx::NEW<String>("MYANMAR"), 22));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::GEORGIAN(
        new Character::UnicodeScript(jxx::NEW<String>("GEORGIAN"), 23));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::HANGUL(
        new Character::UnicodeScript(jxx::NEW<String>("HANGUL"), 24));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::ETHIOPIC(
        new Character::UnicodeScript(jxx::NEW<String>("ETHIOPIC"), 25));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::CHEROKEE(
        new Character::UnicodeScript(jxx::NEW<String>("CHEROKEE"), 26));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::CANADIAN_ABORIGINAL(
        new Character::UnicodeScript(jxx::NEW<String>("CANADIAN_ABORIGINAL"), 27));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::OGHAM(
        new Character::UnicodeScript(jxx::NEW<String>("OGHAM"), 28));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::RUNIC(
        new Character::UnicodeScript(jxx::NEW<String>("RUNIC"), 29));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::KHMER(
        new Character::UnicodeScript(jxx::NEW<String>("KHMER"), 30));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::MONGOLIAN(
        new Character::UnicodeScript(jxx::NEW<String>("MONGOLIAN"), 31));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::HIRAGANA(
        new Character::UnicodeScript(jxx::NEW<String>("HIRAGANA"), 32));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::KATAKANA(
        new Character::UnicodeScript(jxx::NEW<String>("KATAKANA"), 33));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::BOPOMOFO(
        new Character::UnicodeScript(jxx::NEW<String>("BOPOMOFO"), 34));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::HAN(
        new Character::UnicodeScript(jxx::NEW<String>("HAN"), 35));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::YI(
        new Character::UnicodeScript(jxx::NEW<String>("YI"), 36));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::OLD_ITALIC(
        new Character::UnicodeScript(jxx::NEW<String>("OLD_ITALIC"), 37));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::GOTHIC(
        new Character::UnicodeScript(jxx::NEW<String>("GOTHIC"), 38));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::DESERET(
        new Character::UnicodeScript(jxx::NEW<String>("DESERET"), 39));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::INHERITED(
        new Character::UnicodeScript(jxx::NEW<String>("INHERITED"), 40));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::TAGALOG(
        new Character::UnicodeScript(jxx::NEW<String>("TAGALOG"), 41));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::HANUNOO(
        new Character::UnicodeScript(jxx::NEW<String>("HANUNOO"), 42));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::BUHID(
        new Character::UnicodeScript(jxx::NEW<String>("BUHID"), 43));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::TAGBANWA(
        new Character::UnicodeScript(jxx::NEW<String>("TAGBANWA"), 44));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::LIMBU(
        new Character::UnicodeScript(jxx::NEW<String>("LIMBU"), 45));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::TAI_LE(
        new Character::UnicodeScript(jxx::NEW<String>("TAI_LE"), 46));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::LINEAR_B(
        new Character::UnicodeScript(jxx::NEW<String>("LINEAR_B"), 47));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::UGARITIC(
        new Character::UnicodeScript(jxx::NEW<String>("UGARITIC"), 48));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::SHAVIAN(
        new Character::UnicodeScript(jxx::NEW<String>("SHAVIAN"), 49));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::OSMANYA(
        new Character::UnicodeScript(jxx::NEW<String>("OSMANYA"), 50));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::CYPRIOT(
        new Character::UnicodeScript(jxx::NEW<String>("CYPRIOT"), 51));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::BRAILLE(
        new Character::UnicodeScript(jxx::NEW<String>("BRAILLE"), 52));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::BUGINESE(
        new Character::UnicodeScript(jxx::NEW<String>("BUGINESE"), 53));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::COPTIC(
        new Character::UnicodeScript(jxx::NEW<String>("COPTIC"), 54));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::NEW_TAI_LUE(
        new Character::UnicodeScript(jxx::NEW<String>("NEW_TAI_LUE"), 55));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::GLAGOLITIC(
        new Character::UnicodeScript(jxx::NEW<String>("GLAGOLITIC"), 56));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::TIFINAGH(
        new Character::UnicodeScript(jxx::NEW<String>("TIFINAGH"), 57));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::SYLOTI_NAGRI(
        new Character::UnicodeScript(jxx::NEW<String>("SYLOTI_NAGRI"), 58));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::OLD_PERSIAN(
        new Character::UnicodeScript(jxx::NEW<String>("OLD_PERSIAN"), 59));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::KHAROSHTHI(
        new Character::UnicodeScript(jxx::NEW<String>("KHAROSHTHI"), 60));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::BALINESE(
        new Character::UnicodeScript(jxx::NEW<String>("BALINESE"), 61));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::CUNEIFORM(
        new Character::UnicodeScript(jxx::NEW<String>("CUNEIFORM"), 62));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::PHOENICIAN(
        new Character::UnicodeScript(jxx::NEW<String>("PHOENICIAN"), 63));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::PHAGS_PA(
        new Character::UnicodeScript(jxx::NEW<String>("PHAGS_PA"), 64));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::NKO(
        new Character::UnicodeScript(jxx::NEW<String>("NKO"), 65));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::SUNDANESE(
        new Character::UnicodeScript(jxx::NEW<String>("SUNDANESE"), 66));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::LEPCHA(
        new Character::UnicodeScript(jxx::NEW<String>("LEPCHA"), 67));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::OL_CHIKI(
        new Character::UnicodeScript(jxx::NEW<String>("OL_CHIKI"), 68));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::VAI(
        new Character::UnicodeScript(jxx::NEW<String>("VAI"), 69));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::SAURASHTRA(
        new Character::UnicodeScript(jxx::NEW<String>("SAURASHTRA"), 70));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::KAYAH_LI(
        new Character::UnicodeScript(jxx::NEW<String>("KAYAH_LI"), 71));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::REJANG(
        new Character::UnicodeScript(jxx::NEW<String>("REJANG"), 72));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::LYCIAN(
        new Character::UnicodeScript(jxx::NEW<String>("LYCIAN"), 73));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::CARIAN(
        new Character::UnicodeScript(jxx::NEW<String>("CARIAN"), 74));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::LYDIAN(
        new Character::UnicodeScript(jxx::NEW<String>("LYDIAN"), 75));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::CHAM(
        new Character::UnicodeScript(jxx::NEW<String>("CHAM"), 76));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::TAI_THAM(
        new Character::UnicodeScript(jxx::NEW<String>("TAI_THAM"), 77));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::TAI_VIET(
        new Character::UnicodeScript(jxx::NEW<String>("TAI_VIET"), 78));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::AVESTAN(
        new Character::UnicodeScript(jxx::NEW<String>("AVESTAN"), 79));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::EGYPTIAN_HIEROGLYPHS(
        new Character::UnicodeScript(jxx::NEW<String>("EGYPTIAN_HIEROGLYPHS"), 80));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::SAMARITAN(
        new Character::UnicodeScript(jxx::NEW<String>("SAMARITAN"), 81));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::MANDAIC(
        new Character::UnicodeScript(jxx::NEW<String>("MANDAIC"), 82));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::LISU(
        new Character::UnicodeScript(jxx::NEW<String>("LISU"), 83));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::BAMUM(
        new Character::UnicodeScript(jxx::NEW<String>("BAMUM"), 84));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::JAVANESE(
        new Character::UnicodeScript(jxx::NEW<String>("JAVANESE"), 85));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::MEETEI_MAYEK(
        new Character::UnicodeScript(jxx::NEW<String>("MEETEI_MAYEK"), 86));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::IMPERIAL_ARAMAIC(
        new Character::UnicodeScript(jxx::NEW<String>("IMPERIAL_ARAMAIC"), 87));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::OLD_SOUTH_ARABIAN(
        new Character::UnicodeScript(jxx::NEW<String>("OLD_SOUTH_ARABIAN"), 88));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::INSCRIPTIONAL_PARTHIAN(
        new Character::UnicodeScript(jxx::NEW<String>("INSCRIPTIONAL_PARTHIAN"), 89));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::INSCRIPTIONAL_PAHLAVI(
        new Character::UnicodeScript(jxx::NEW<String>("INSCRIPTIONAL_PAHLAVI"), 90));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::OLD_TURKIC(
        new Character::UnicodeScript(jxx::NEW<String>("OLD_TURKIC"), 91));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::BRAHMI(
        new Character::UnicodeScript(jxx::NEW<String>("BRAHMI"), 92));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::KAITHI(
        new Character::UnicodeScript(jxx::NEW<String>("KAITHI"), 93));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::MEROITIC_HIEROGLYPHS(
        new Character::UnicodeScript(jxx::NEW<String>("MEROITIC_HIEROGLYPHS"), 94));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::MEROITIC_CURSIVE(
        new Character::UnicodeScript(jxx::NEW<String>("MEROITIC_CURSIVE"), 95));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::SORA_SOMPENG(
        new Character::UnicodeScript(jxx::NEW<String>("SORA_SOMPENG"), 96));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::CHAKMA(
        new Character::UnicodeScript(jxx::NEW<String>("CHAKMA"), 97));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::SHARADA(
        new Character::UnicodeScript(jxx::NEW<String>("SHARADA"), 98));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::TAKRI(
        new Character::UnicodeScript(jxx::NEW<String>("TAKRI"), 99));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::MIAO(
        new Character::UnicodeScript(jxx::NEW<String>("MIAO"), 100));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::UNKNOWN(
        new Character::UnicodeScript(jxx::NEW<String>("UNKNOWN"), 101));

    jxx::Ptr<ClassAny> Character::UnicodeScript::Class() {
        return ClassInfo<UnicodeScript, Enum<UnicodeScript>>::Class();
    }

    jxx::Ptr<JxxArray<jxx::Ptr<Character::UnicodeScript>, 1>>
    Character::UnicodeScript::values() {
        auto result = jxx::NEW<JxxArray<jxx::Ptr<UnicodeScript>, 1>>(102);
        (*result)[0] = COMMON;
        (*result)[1] = LATIN;
        (*result)[2] = GREEK;
        (*result)[3] = CYRILLIC;
        (*result)[4] = ARMENIAN;
        (*result)[5] = HEBREW;
        (*result)[6] = ARABIC;
        (*result)[7] = SYRIAC;
        (*result)[8] = THAANA;
        (*result)[9] = DEVANAGARI;
        (*result)[10] = BENGALI;
        (*result)[11] = GURMUKHI;
        (*result)[12] = GUJARATI;
        (*result)[13] = ORIYA;
        (*result)[14] = TAMIL;
        (*result)[15] = TELUGU;
        (*result)[16] = KANNADA;
        (*result)[17] = MALAYALAM;
        (*result)[18] = SINHALA;
        (*result)[19] = THAI;
        (*result)[20] = LAO;
        (*result)[21] = TIBETAN;
        (*result)[22] = MYANMAR;
        (*result)[23] = GEORGIAN;
        (*result)[24] = HANGUL;
        (*result)[25] = ETHIOPIC;
        (*result)[26] = CHEROKEE;
        (*result)[27] = CANADIAN_ABORIGINAL;
        (*result)[28] = OGHAM;
        (*result)[29] = RUNIC;
        (*result)[30] = KHMER;
        (*result)[31] = MONGOLIAN;
        (*result)[32] = HIRAGANA;
        (*result)[33] = KATAKANA;
        (*result)[34] = BOPOMOFO;
        (*result)[35] = HAN;
        (*result)[36] = YI;
        (*result)[37] = OLD_ITALIC;
        (*result)[38] = GOTHIC;
        (*result)[39] = DESERET;
        (*result)[40] = INHERITED;
        (*result)[41] = TAGALOG;
        (*result)[42] = HANUNOO;
        (*result)[43] = BUHID;
        (*result)[44] = TAGBANWA;
        (*result)[45] = LIMBU;
        (*result)[46] = TAI_LE;
        (*result)[47] = LINEAR_B;
        (*result)[48] = UGARITIC;
        (*result)[49] = SHAVIAN;
        (*result)[50] = OSMANYA;
        (*result)[51] = CYPRIOT;
        (*result)[52] = BRAILLE;
        (*result)[53] = BUGINESE;
        (*result)[54] = COPTIC;
        (*result)[55] = NEW_TAI_LUE;
        (*result)[56] = GLAGOLITIC;
        (*result)[57] = TIFINAGH;
        (*result)[58] = SYLOTI_NAGRI;
        (*result)[59] = OLD_PERSIAN;
        (*result)[60] = KHAROSHTHI;
        (*result)[61] = BALINESE;
        (*result)[62] = CUNEIFORM;
        (*result)[63] = PHOENICIAN;
        (*result)[64] = PHAGS_PA;
        (*result)[65] = NKO;
        (*result)[66] = SUNDANESE;
        (*result)[67] = LEPCHA;
        (*result)[68] = OL_CHIKI;
        (*result)[69] = VAI;
        (*result)[70] = SAURASHTRA;
        (*result)[71] = KAYAH_LI;
        (*result)[72] = REJANG;
        (*result)[73] = LYCIAN;
        (*result)[74] = CARIAN;
        (*result)[75] = LYDIAN;
        (*result)[76] = CHAM;
        (*result)[77] = TAI_THAM;
        (*result)[78] = TAI_VIET;
        (*result)[79] = AVESTAN;
        (*result)[80] = EGYPTIAN_HIEROGLYPHS;
        (*result)[81] = SAMARITAN;
        (*result)[82] = MANDAIC;
        (*result)[83] = LISU;
        (*result)[84] = BAMUM;
        (*result)[85] = JAVANESE;
        (*result)[86] = MEETEI_MAYEK;
        (*result)[87] = IMPERIAL_ARAMAIC;
        (*result)[88] = OLD_SOUTH_ARABIAN;
        (*result)[89] = INSCRIPTIONAL_PARTHIAN;
        (*result)[90] = INSCRIPTIONAL_PAHLAVI;
        (*result)[91] = OLD_TURKIC;
        (*result)[92] = BRAHMI;
        (*result)[93] = KAITHI;
        (*result)[94] = MEROITIC_HIEROGLYPHS;
        (*result)[95] = MEROITIC_CURSIVE;
        (*result)[96] = SORA_SOMPENG;
        (*result)[97] = CHAKMA;
        (*result)[98] = SHARADA;
        (*result)[99] = TAKRI;
        (*result)[100] = MIAO;
        (*result)[101] = UNKNOWN;
        return result;
    }

    jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::valueOf(
        const jxx::Ptr<String>& name) {
        return Enum<UnicodeScript>::valueOf(Class(), name);
    }

    jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::forName(
        const jxx::Ptr<String>& scriptName) {
        const auto requested = normalizedUnicodeName(scriptName);
        static const char* aliases[] = {
            "ZYYY",
            "LATN",
            "GREK",
            "CYRL",
            "ARMN",
            "HEBR",
            "ARAB",
            "SYRC",
            "THAA",
            "DEVA",
            "BENG",
            "GURU",
            "GUJR",
            "ORYA",
            "TAML",
            "TELU",
            "KNDA",
            "MLYM",
            "SINH",
            "THAI",
            "LAOO",
            "TIBT",
            "MYMR",
            "GEOR",
            "HANG",
            "ETHI",
            "CHER",
            "CANS",
            "OGAM",
            "RUNR",
            "KHMR",
            "MONG",
            "HIRA",
            "KANA",
            "BOPO",
            "HANI",
            "YIII",
            "ITAL",
            "GOTH",
            "DSRT",
            "ZINH",
            "TGLG",
            "HANO",
            "BUHD",
            "TAGB",
            "LIMB",
            "TALE",
            "LINB",
            "UGAR",
            "SHAW",
            "OSMA",
            "CPRT",
            "BRAI",
            "BUGI",
            "COPT",
            "TALU",
            "GLAG",
            "TFNG",
            "SYLO",
            "XPEO",
            "KHAR",
            "BALI",
            "XSUX",
            "PHNX",
            "PHAG",
            "NKOO",
            "SUND",
            "LEPC",
            "OLCK",
            "VAII",
            "SAUR",
            "KALI",
            "RJNG",
            "LYCI",
            "CARI",
            "LYDI",
            "CHAM",
            "LANA",
            "TAVT",
            "AVST",
            "EGYP",
            "SAMR",
            "MAND",
            "LISU",
            "BAMU",
            "JAVA",
            "MTEI",
            "ARMI",
            "SARB",
            "PRTI",
            "PHLI",
            "ORKH",
            "BRAH",
            "KTHI",
            "MERO",
            "MERC",
            "SORA",
            "CAKM",
            "SHRD",
            "TAKR",
            "PLRD",
            "ZZZZ",
        };
        const auto all = values();
        for (std::uint32_t i = 0; i < all->length; ++i) {
            if (requested == aliases[i]) return (*all)[i];
        }
        for (std::uint32_t i=0; i<all->length; ++i) {
            const auto& script = (*all)[i];
            if (normalizedUnicodeName(script->name()) == requested) return script;
        }
        throw IllegalArgumentException(scriptName);
    }

    jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::of(jint codePoint) {
        if (!Character::isValidCodePoint(codePoint)) throw IllegalArgumentException();
        using Key = jxx::unicode_bridge::UnicodeScriptKey;
        switch (jxx::unicode_bridge::getUnicodeScript(static_cast<char32_t>(codePoint))) {
            case Key::Common: return COMMON;
            case Key::Latin: return LATIN;
            case Key::Greek: return GREEK;
            case Key::Cyrillic: return CYRILLIC;
            case Key::Armenian: return ARMENIAN;
            case Key::Hebrew: return HEBREW;
            case Key::Arabic: return ARABIC;
            case Key::Syriac: return SYRIAC;
            case Key::Thaana: return THAANA;
            case Key::Devanagari: return DEVANAGARI;
            case Key::Bengali: return BENGALI;
            case Key::Gurmukhi: return GURMUKHI;
            case Key::Gujarati: return GUJARATI;
            case Key::Oriya: return ORIYA;
            case Key::Tamil: return TAMIL;
            case Key::Telugu: return TELUGU;
            case Key::Kannada: return KANNADA;
            case Key::Malayalam: return MALAYALAM;
            case Key::Sinhala: return SINHALA;
            case Key::Thai: return THAI;
            case Key::Lao: return LAO;
            case Key::Tibetan: return TIBETAN;
            case Key::Myanmar: return MYANMAR;
            case Key::Georgian: return GEORGIAN;
            case Key::Hangul: return HANGUL;
            case Key::Ethiopic: return ETHIOPIC;
            case Key::Cherokee: return CHEROKEE;
            case Key::CanadianAboriginal: return CANADIAN_ABORIGINAL;
            case Key::Ogham: return OGHAM;
            case Key::Runic: return RUNIC;
            case Key::Khmer: return KHMER;
            case Key::Mongolian: return MONGOLIAN;
            case Key::Hiragana: return HIRAGANA;
            case Key::Katakana: return KATAKANA;
            case Key::Bopomofo: return BOPOMOFO;
            case Key::Han: return HAN;
            case Key::Yi: return YI;
            case Key::OldItalic: return OLD_ITALIC;
            case Key::Gothic: return GOTHIC;
            case Key::Deseret: return DESERET;
            case Key::Inherited: return INHERITED;
            case Key::Tagalog: return TAGALOG;
            case Key::Hanunoo: return HANUNOO;
            case Key::Buhid: return BUHID;
            case Key::Tagbanwa: return TAGBANWA;
            case Key::Limbu: return LIMBU;
            case Key::TaiLe: return TAI_LE;
            case Key::LinearB: return LINEAR_B;
            case Key::Ugaritic: return UGARITIC;
            case Key::Shavian: return SHAVIAN;
            case Key::Osmanya: return OSMANYA;
            case Key::Cypriot: return CYPRIOT;
            case Key::Braille: return BRAILLE;
            case Key::Buginese: return BUGINESE;
            case Key::Coptic: return COPTIC;
            case Key::NewTaiLue: return NEW_TAI_LUE;
            case Key::Glagolitic: return GLAGOLITIC;
            case Key::Tifinagh: return TIFINAGH;
            case Key::SylotiNagri: return SYLOTI_NAGRI;
            case Key::OldPersian: return OLD_PERSIAN;
            case Key::Kharoshthi: return KHAROSHTHI;
            case Key::Balinese: return BALINESE;
            case Key::Cuneiform: return CUNEIFORM;
            case Key::Phoenician: return PHOENICIAN;
            case Key::PhagsPa: return PHAGS_PA;
            case Key::Nko: return NKO;
            case Key::Sundanese: return SUNDANESE;
            case Key::Lepcha: return LEPCHA;
            case Key::OlChiki: return OL_CHIKI;
            case Key::Vai: return VAI;
            case Key::Saurashtra: return SAURASHTRA;
            case Key::KayahLi: return KAYAH_LI;
            case Key::Rejang: return REJANG;
            case Key::Lycian: return LYCIAN;
            case Key::Carian: return CARIAN;
            case Key::Lydian: return LYDIAN;
            case Key::Cham: return CHAM;
            case Key::TaiTham: return TAI_THAM;
            case Key::TaiViet: return TAI_VIET;
            case Key::Avestan: return AVESTAN;
            case Key::EgyptianHieroglyphs: return EGYPTIAN_HIEROGLYPHS;
            case Key::Samaritan: return SAMARITAN;
            case Key::Mandaic: return MANDAIC;
            case Key::Lisu: return LISU;
            case Key::Bamum: return BAMUM;
            case Key::Javanese: return JAVANESE;
            case Key::MeeteiMayek: return MEETEI_MAYEK;
            case Key::ImperialAramaic: return IMPERIAL_ARAMAIC;
            case Key::OldSouthArabian: return OLD_SOUTH_ARABIAN;
            case Key::InscriptionalParthian: return INSCRIPTIONAL_PARTHIAN;
            case Key::InscriptionalPahlavi: return INSCRIPTIONAL_PAHLAVI;
            case Key::OldTurkic: return OLD_TURKIC;
            case Key::Brahmi: return BRAHMI;
            case Key::Kaithi: return KAITHI;
            case Key::MeroiticHieroglyphs: return MEROITIC_HIEROGLYPHS;
            case Key::MeroiticCursive: return MEROITIC_CURSIVE;
            case Key::SoraSompeng: return SORA_SOMPENG;
            case Key::Chakma: return CHAKMA;
            case Key::Sharada: return SHARADA;
            case Key::Takri: return TAKRI;
            case Key::Miao: return MIAO;
            default: return UNKNOWN;
        }
    }
    namespace {
    jxx::Ptr<ClassAny> registerCharacterPrimitive()
    {
        ClassAny::Meta metadata;
        metadata.binaryName = "char";
        metadata.typeId = std::type_index(typeid(jchar));
        metadata.isPrimitive = true;
        metadata.modifiers = 0x0001 | 0x0010 | 0x0400;
        return ClassAny::registerClass(metadata);
    }
    }

    const jxx::Ptr<ClassAny> Character::TYPE = registerCharacterPrimitive();

    jxx::Ptr<ClassAny> Character::Class()
    {
        return JxxClassInfoMarker::Class();
    }


    Character::Character(jchar value)
        : Super()
        , value_(value)
    {}

    jchar Character::charValue() const noexcept
    {
        return value_;
    }

    jint Character::compareTo(const jxx::Ptr<Character>& anotherCharacter) const
    {
        if (!anotherCharacter)
            return 1;

        if (value_ < anotherCharacter->value_)
            return -1;
        if (value_ > anotherCharacter->value_)
            return 1;
        return 0;
    }

    jbool Character::equals(const jxx::Ptr<Object>& obj) const
    {
        auto other = std::dynamic_pointer_cast<Character>(obj);
        return other && other->value_ == value_;
    }

    jint Character::hashCode() const
    {
        return hashCode(value_);
    }

    jint Character::hashCode(jchar value) noexcept
    {
        return static_cast<jint>(value);
    }

    jint Character::compare(jchar x, jchar y) noexcept
    {
        return static_cast<jint>(x) - static_cast<jint>(y);
    }

    jxx::Ptr<String> Character::toString(jchar value)
    {
        return jxx::NEW<String>(std::u16string(1, static_cast<char16_t>(value)));
    }

    jchar Character::reverseBytes(jchar value) noexcept
    {
        return static_cast<jchar>((value << 8) | (value >> 8));
    }

    jxx::Ptr<String> Character::toString() const
    {
        return String::valueOf(value_);
    }

    jxx::Ptr<Character> Character::valueOf(jchar c)
    {
        static const std::array<jxx::Ptr<Character>, 128> cache = [] {
            std::array<jxx::Ptr<Character>, 128> values{};
            for (std::size_t index = 0; index < values.size(); ++index)
            {
                values[index] = jxx::NEW<Character>(static_cast<jchar>(index));
            }
            return values;
        }();
        return c <= 127 ? cache[static_cast<std::size_t>(c)] : jxx::NEW<Character>(c);
    }

    jbool Character::isValidCodePoint(jint codePoint) noexcept
    {
        return codePoint >= MIN_CODE_POINT && codePoint <= MAX_CODE_POINT;
    }

    jbool Character::isBmpCodePoint(jint codePoint) noexcept
    {
        return codePoint >= 0x0000 && codePoint <= 0xFFFF;
    }

    jbool Character::isSupplementaryCodePoint(jint codePoint) noexcept
    {
        return codePoint >= MIN_SUPPLEMENTARY_CODE_POINT && codePoint <= MAX_CODE_POINT;
    }

    jbool Character::isHighSurrogate(jchar ch) noexcept
    {
        return ch >= MIN_HIGH_SURROGATE && ch <= MAX_HIGH_SURROGATE;
    }

    jbool Character::isLowSurrogate(jchar ch) noexcept
    {
        return ch >= MIN_LOW_SURROGATE && ch <= MAX_LOW_SURROGATE;
    }

    jbool Character::isSurrogate(jchar ch) noexcept
    {
        return isHighSurrogate(ch) || isLowSurrogate(ch);
    }

    jbool Character::isSurrogatePair(jchar high, jchar low) noexcept
    {
        return isHighSurrogate(high) && isLowSurrogate(low);
    }

    jint Character::charCount(jint codePoint) noexcept
    {
        return isSupplementaryCodePoint(codePoint) ? 2 : 1;
    }

    jint Character::toCodePoint(jchar high, jchar low) noexcept
    {
        return ((static_cast<jint>(high) - 0xD800) << 10)
            + (static_cast<jint>(low) - 0xDC00)
            + 0x10000;
    }

    jchar Character::highSurrogate(jint codePoint) noexcept
    {
        return static_cast<jchar>(((codePoint - 0x10000) >> 10) + 0xD800);
    }

    jchar Character::lowSurrogate(jint codePoint) noexcept
    {
        return static_cast<jchar>(((codePoint - 0x10000) & 0x3FF) + 0xDC00);
    }

    jbool Character::isLowerCase(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isLowerCase(static_cast<char32_t>(codePoint));
    }

    jbool Character::isUpperCase(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isUpperCase(static_cast<char32_t>(codePoint));
    }

    jbool Character::isTitleCase(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isTitleCase(static_cast<char32_t>(codePoint));
    }

    jbool Character::isDigit(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isDigit(static_cast<char32_t>(codePoint));
    }

    jbool Character::isLetter(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isLetter(static_cast<char32_t>(codePoint));
    }

    jbool Character::isLetterOrDigit(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isLetterOrDigit(static_cast<char32_t>(codePoint));
    }

    jbool Character::isWhitespace(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isWhitespace(static_cast<char32_t>(codePoint));
    }

    jbool Character::isSpaceChar(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isSpaceChar(static_cast<char32_t>(codePoint));
    }

    jbool Character::isISOControl(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isISOControl(static_cast<char32_t>(codePoint));
    }

    jbool Character::isDefined(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isDefined(static_cast<char32_t>(codePoint));
    }

    jbool Character::isMirrored(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isMirrored(static_cast<char32_t>(codePoint));
    }

    jint Character::getType(jint codePoint) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return UNASSIGNED;

        return jxx::unicode_bridge::getType(static_cast<char32_t>(codePoint));
    }

    jint Character::getNumericValue(jint codePoint) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return -1;

        return jxx::unicode_bridge::getNumericValue(static_cast<char32_t>(codePoint));
    }

    jint Character::digit(jint codePoint, jint radix) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return -1;

        if (radix < MIN_RADIX || radix > MAX_RADIX)
            return -1;

        return jxx::unicode_bridge::digit(static_cast<char32_t>(codePoint), radix);
    }

    jint Character::toLowerCase(jint codePoint) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return codePoint;

        return gsl::narrow<jint>(
            jxx::unicode_bridge::toLowerCase(static_cast<char32_t>(codePoint)));
    }

    jint Character::toUpperCase(jint codePoint) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return codePoint;

        return gsl::narrow<jint>(
            jxx::unicode_bridge::toUpperCase(static_cast<char32_t>(codePoint)));
    }

    jint Character::toTitleCase(jint codePoint) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return codePoint;

        return gsl::narrow<jint>(
            jxx::unicode_bridge::toTitleCase(static_cast<char32_t>(codePoint)));
    }

    CharArray Character::toChars(jint codePoint)
    {
        if (!isValidCodePoint(codePoint))
            throwIAE_("invalid Unicode code point");

        if (isBmpCodePoint(codePoint))
        {
            CharArray result = jxx::NEW<JxxArray<jchar, 1U>>(1U);
            (*result)[0] = static_cast<jchar>(codePoint);
            return result;
        }

        CharArray result = jxx::NEW<JxxArray<jchar, 1U>>(2U);
        (*result)[0] = highSurrogate(codePoint);
        (*result)[1] = lowSurrogate(codePoint);
        return result;
    }




    void Character::writeObject(
        const jxx::Ptr<jxx::io::ObjectOutputStream>& out)
    {
        (void)out;
    }

    void Character::readObject(
        const jxx::Ptr<jxx::io::ObjectInputStream>& in)
    {
        (void)in;
    }

    void Character::readObjectNoData()
    {
    }

}