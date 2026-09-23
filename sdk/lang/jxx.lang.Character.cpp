#include <memory>
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include <gsl/util>
#include <gsl/narrow>

#include "lang/jxx.lang.Character.h"

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"


#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
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
                if (ch == ' ' || ch == '-' || ch == '_') ch = '\0';
                else ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
            }
            name.erase(std::remove(name.begin(), name.end(), '\0'), name.end());
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

    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BASIC_LATIN(new Character::UnicodeBlock(jxx::NEW<String>("BASIC_LATIN"), 0x0, 0x7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LATIN_1_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("LATIN_1_SUPPLEMENT"), 0x80, 0xFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LATIN_EXTENDED_A(new Character::UnicodeBlock(jxx::NEW<String>("LATIN_EXTENDED_A"), 0x100, 0x17F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LATIN_EXTENDED_B(new Character::UnicodeBlock(jxx::NEW<String>("LATIN_EXTENDED_B"), 0x180, 0x24F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::IPA_EXTENSIONS(new Character::UnicodeBlock(jxx::NEW<String>("IPA_EXTENSIONS"), 0x250, 0x2AF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SPACING_MODIFIER_LETTERS(new Character::UnicodeBlock(jxx::NEW<String>("SPACING_MODIFIER_LETTERS"), 0x2B0, 0x2FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::COMBINING_DIACRITICAL_MARKS(new Character::UnicodeBlock(jxx::NEW<String>("COMBINING_DIACRITICAL_MARKS"), 0x300, 0x36F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GREEK(new Character::UnicodeBlock(jxx::NEW<String>("GREEK"), 0x370, 0x3FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CYRILLIC(new Character::UnicodeBlock(jxx::NEW<String>("CYRILLIC"), 0x400, 0x4FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ARMENIAN(new Character::UnicodeBlock(jxx::NEW<String>("ARMENIAN"), 0x530, 0x58F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HEBREW(new Character::UnicodeBlock(jxx::NEW<String>("HEBREW"), 0x590, 0x5FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ARABIC(new Character::UnicodeBlock(jxx::NEW<String>("ARABIC"), 0x600, 0x6FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::DEVANAGARI(new Character::UnicodeBlock(jxx::NEW<String>("DEVANAGARI"), 0x900, 0x97F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BENGALI(new Character::UnicodeBlock(jxx::NEW<String>("BENGALI"), 0x980, 0x9FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GURMUKHI(new Character::UnicodeBlock(jxx::NEW<String>("GURMUKHI"), 0xA00, 0xA7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GUJARATI(new Character::UnicodeBlock(jxx::NEW<String>("GUJARATI"), 0xA80, 0xAFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ORIYA(new Character::UnicodeBlock(jxx::NEW<String>("ORIYA"), 0xB00, 0xB7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TAMIL(new Character::UnicodeBlock(jxx::NEW<String>("TAMIL"), 0xB80, 0xBFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TELUGU(new Character::UnicodeBlock(jxx::NEW<String>("TELUGU"), 0xC00, 0xC7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::KANNADA(new Character::UnicodeBlock(jxx::NEW<String>("KANNADA"), 0xC80, 0xCFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MALAYALAM(new Character::UnicodeBlock(jxx::NEW<String>("MALAYALAM"), 0xD00, 0xD7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::THAI(new Character::UnicodeBlock(jxx::NEW<String>("THAI"), 0xE00, 0xE7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LAO(new Character::UnicodeBlock(jxx::NEW<String>("LAO"), 0xE80, 0xEFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TIBETAN(new Character::UnicodeBlock(jxx::NEW<String>("TIBETAN"), 0xF00, 0xFFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GEORGIAN(new Character::UnicodeBlock(jxx::NEW<String>("GEORGIAN"), 0x10A0, 0x10FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HANGUL_JAMO(new Character::UnicodeBlock(jxx::NEW<String>("HANGUL_JAMO"), 0x1100, 0x11FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LATIN_EXTENDED_ADDITIONAL(new Character::UnicodeBlock(jxx::NEW<String>("LATIN_EXTENDED_ADDITIONAL"), 0x1E00, 0x1EFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GREEK_EXTENDED(new Character::UnicodeBlock(jxx::NEW<String>("GREEK_EXTENDED"), 0x1F00, 0x1FFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GENERAL_PUNCTUATION(new Character::UnicodeBlock(jxx::NEW<String>("GENERAL_PUNCTUATION"), 0x2000, 0x206F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SUPERSCRIPTS_AND_SUBSCRIPTS(new Character::UnicodeBlock(jxx::NEW<String>("SUPERSCRIPTS_AND_SUBSCRIPTS"), 0x2070, 0x209F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CURRENCY_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("CURRENCY_SYMBOLS"), 0x20A0, 0x20CF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::COMBINING_MARKS_FOR_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("COMBINING_MARKS_FOR_SYMBOLS"), 0x20D0, 0x20FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LETTERLIKE_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("LETTERLIKE_SYMBOLS"), 0x2100, 0x214F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::NUMBER_FORMS(new Character::UnicodeBlock(jxx::NEW<String>("NUMBER_FORMS"), 0x2150, 0x218F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ARROWS(new Character::UnicodeBlock(jxx::NEW<String>("ARROWS"), 0x2190, 0x21FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MATHEMATICAL_OPERATORS(new Character::UnicodeBlock(jxx::NEW<String>("MATHEMATICAL_OPERATORS"), 0x2200, 0x22FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MISCELLANEOUS_TECHNICAL(new Character::UnicodeBlock(jxx::NEW<String>("MISCELLANEOUS_TECHNICAL"), 0x2300, 0x23FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CONTROL_PICTURES(new Character::UnicodeBlock(jxx::NEW<String>("CONTROL_PICTURES"), 0x2400, 0x243F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::OPTICAL_CHARACTER_RECOGNITION(new Character::UnicodeBlock(jxx::NEW<String>("OPTICAL_CHARACTER_RECOGNITION"), 0x2440, 0x245F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ENCLOSED_ALPHANUMERICS(new Character::UnicodeBlock(jxx::NEW<String>("ENCLOSED_ALPHANUMERICS"), 0x2460, 0x24FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BOX_DRAWING(new Character::UnicodeBlock(jxx::NEW<String>("BOX_DRAWING"), 0x2500, 0x257F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BLOCK_ELEMENTS(new Character::UnicodeBlock(jxx::NEW<String>("BLOCK_ELEMENTS"), 0x2580, 0x259F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GEOMETRIC_SHAPES(new Character::UnicodeBlock(jxx::NEW<String>("GEOMETRIC_SHAPES"), 0x25A0, 0x25FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MISCELLANEOUS_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("MISCELLANEOUS_SYMBOLS"), 0x2600, 0x26FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::DINGBATS(new Character::UnicodeBlock(jxx::NEW<String>("DINGBATS"), 0x2700, 0x27BF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_SYMBOLS_AND_PUNCTUATION(new Character::UnicodeBlock(jxx::NEW<String>("CJK_SYMBOLS_AND_PUNCTUATION"), 0x3000, 0x303F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HIRAGANA(new Character::UnicodeBlock(jxx::NEW<String>("HIRAGANA"), 0x3040, 0x309F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::KATAKANA(new Character::UnicodeBlock(jxx::NEW<String>("KATAKANA"), 0x30A0, 0x30FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BOPOMOFO(new Character::UnicodeBlock(jxx::NEW<String>("BOPOMOFO"), 0x3100, 0x312F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HANGUL_COMPATIBILITY_JAMO(new Character::UnicodeBlock(jxx::NEW<String>("HANGUL_COMPATIBILITY_JAMO"), 0x3130, 0x318F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::KANBUN(new Character::UnicodeBlock(jxx::NEW<String>("KANBUN"), 0x3190, 0x319F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ENCLOSED_CJK_LETTERS_AND_MONTHS(new Character::UnicodeBlock(jxx::NEW<String>("ENCLOSED_CJK_LETTERS_AND_MONTHS"), 0x3200, 0x32FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_COMPATIBILITY(new Character::UnicodeBlock(jxx::NEW<String>("CJK_COMPATIBILITY"), 0x3300, 0x33FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS(new Character::UnicodeBlock(jxx::NEW<String>("CJK_UNIFIED_IDEOGRAPHS"), 0x4E00, 0x9FFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HANGUL_SYLLABLES(new Character::UnicodeBlock(jxx::NEW<String>("HANGUL_SYLLABLES"), 0xAC00, 0xD7AF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::PRIVATE_USE_AREA(new Character::UnicodeBlock(jxx::NEW<String>("PRIVATE_USE_AREA"), 0xE000, 0xF8FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS(new Character::UnicodeBlock(jxx::NEW<String>("CJK_COMPATIBILITY_IDEOGRAPHS"), 0xF900, 0xFAFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ALPHABETIC_PRESENTATION_FORMS(new Character::UnicodeBlock(jxx::NEW<String>("ALPHABETIC_PRESENTATION_FORMS"), 0xFB00, 0xFB4F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ARABIC_PRESENTATION_FORMS_A(new Character::UnicodeBlock(jxx::NEW<String>("ARABIC_PRESENTATION_FORMS_A"), 0xFB50, 0xFDFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::COMBINING_HALF_MARKS(new Character::UnicodeBlock(jxx::NEW<String>("COMBINING_HALF_MARKS"), 0xFE20, 0xFE2F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_COMPATIBILITY_FORMS(new Character::UnicodeBlock(jxx::NEW<String>("CJK_COMPATIBILITY_FORMS"), 0xFE30, 0xFE4F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SMALL_FORM_VARIANTS(new Character::UnicodeBlock(jxx::NEW<String>("SMALL_FORM_VARIANTS"), 0xFE50, 0xFE6F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ARABIC_PRESENTATION_FORMS_B(new Character::UnicodeBlock(jxx::NEW<String>("ARABIC_PRESENTATION_FORMS_B"), 0xFE70, 0xFEFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HALFWIDTH_AND_FULLWIDTH_FORMS(new Character::UnicodeBlock(jxx::NEW<String>("HALFWIDTH_AND_FULLWIDTH_FORMS"), 0xFF00, 0xFFEF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SPECIALS(new Character::UnicodeBlock(jxx::NEW<String>("SPECIALS"), 0xFFF0, 0xFFFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SURROGATES_AREA(new Character::UnicodeBlock(jxx::NEW<String>("SURROGATES_AREA"), -1, -1));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SYRIAC(new Character::UnicodeBlock(jxx::NEW<String>("SYRIAC"), 0x700, 0x74F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::THAANA(new Character::UnicodeBlock(jxx::NEW<String>("THAANA"), 0x780, 0x7BF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SINHALA(new Character::UnicodeBlock(jxx::NEW<String>("SINHALA"), 0xD80, 0xDFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MYANMAR(new Character::UnicodeBlock(jxx::NEW<String>("MYANMAR"), 0x1000, 0x109F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ETHIOPIC(new Character::UnicodeBlock(jxx::NEW<String>("ETHIOPIC"), 0x1200, 0x137F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CHEROKEE(new Character::UnicodeBlock(jxx::NEW<String>("CHEROKEE"), 0x13A0, 0x13FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS(new Character::UnicodeBlock(jxx::NEW<String>("UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS"), 0x1400, 0x167F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::OGHAM(new Character::UnicodeBlock(jxx::NEW<String>("OGHAM"), 0x1680, 0x169F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::RUNIC(new Character::UnicodeBlock(jxx::NEW<String>("RUNIC"), 0x16A0, 0x16FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::KHMER(new Character::UnicodeBlock(jxx::NEW<String>("KHMER"), 0x1780, 0x17FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MONGOLIAN(new Character::UnicodeBlock(jxx::NEW<String>("MONGOLIAN"), 0x1800, 0x18AF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BRAILLE_PATTERNS(new Character::UnicodeBlock(jxx::NEW<String>("BRAILLE_PATTERNS"), 0x2800, 0x28FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_RADICALS_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("CJK_RADICALS_SUPPLEMENT"), 0x2E80, 0x2EFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::KANGXI_RADICALS(new Character::UnicodeBlock(jxx::NEW<String>("KANGXI_RADICALS"), 0x2F00, 0x2FDF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::IDEOGRAPHIC_DESCRIPTION_CHARACTERS(new Character::UnicodeBlock(jxx::NEW<String>("IDEOGRAPHIC_DESCRIPTION_CHARACTERS"), 0x2FF0, 0x2FFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BOPOMOFO_EXTENDED(new Character::UnicodeBlock(jxx::NEW<String>("BOPOMOFO_EXTENDED"), 0x31A0, 0x31BF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A(new Character::UnicodeBlock(jxx::NEW<String>("CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A"), 0x3400, 0x4DBF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::YI_SYLLABLES(new Character::UnicodeBlock(jxx::NEW<String>("YI_SYLLABLES"), 0xA000, 0xA48F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::YI_RADICALS(new Character::UnicodeBlock(jxx::NEW<String>("YI_RADICALS"), 0xA490, 0xA4CF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CYRILLIC_SUPPLEMENTARY(new Character::UnicodeBlock(jxx::NEW<String>("CYRILLIC_SUPPLEMENTARY"), 0x500, 0x52F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TAGALOG(new Character::UnicodeBlock(jxx::NEW<String>("TAGALOG"), 0x1700, 0x171F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HANUNOO(new Character::UnicodeBlock(jxx::NEW<String>("HANUNOO"), 0x1720, 0x173F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BUHID(new Character::UnicodeBlock(jxx::NEW<String>("BUHID"), 0x1740, 0x175F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TAGBANWA(new Character::UnicodeBlock(jxx::NEW<String>("TAGBANWA"), 0x1760, 0x177F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LIMBU(new Character::UnicodeBlock(jxx::NEW<String>("LIMBU"), 0x1900, 0x194F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TAI_LE(new Character::UnicodeBlock(jxx::NEW<String>("TAI_LE"), 0x1950, 0x197F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::KHMER_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("KHMER_SYMBOLS"), 0x19E0, 0x19FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::PHONETIC_EXTENSIONS(new Character::UnicodeBlock(jxx::NEW<String>("PHONETIC_EXTENSIONS"), 0x1D00, 0x1D7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A(new Character::UnicodeBlock(jxx::NEW<String>("MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A"), 0x27C0, 0x27EF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SUPPLEMENTAL_ARROWS_A(new Character::UnicodeBlock(jxx::NEW<String>("SUPPLEMENTAL_ARROWS_A"), 0x27F0, 0x27FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SUPPLEMENTAL_ARROWS_B(new Character::UnicodeBlock(jxx::NEW<String>("SUPPLEMENTAL_ARROWS_B"), 0x2900, 0x297F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B(new Character::UnicodeBlock(jxx::NEW<String>("MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B"), 0x2980, 0x29FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SUPPLEMENTAL_MATHEMATICAL_OPERATORS(new Character::UnicodeBlock(jxx::NEW<String>("SUPPLEMENTAL_MATHEMATICAL_OPERATORS"), 0x2A00, 0x2AFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MISCELLANEOUS_SYMBOLS_AND_ARROWS(new Character::UnicodeBlock(jxx::NEW<String>("MISCELLANEOUS_SYMBOLS_AND_ARROWS"), 0x2B00, 0x2BFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::KATAKANA_PHONETIC_EXTENSIONS(new Character::UnicodeBlock(jxx::NEW<String>("KATAKANA_PHONETIC_EXTENSIONS"), 0x31F0, 0x31FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::YIJING_HEXAGRAM_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("YIJING_HEXAGRAM_SYMBOLS"), 0x4DC0, 0x4DFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::VARIATION_SELECTORS(new Character::UnicodeBlock(jxx::NEW<String>("VARIATION_SELECTORS"), 0xFE00, 0xFE0F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LINEAR_B_SYLLABARY(new Character::UnicodeBlock(jxx::NEW<String>("LINEAR_B_SYLLABARY"), 0x10000, 0x1007F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LINEAR_B_IDEOGRAMS(new Character::UnicodeBlock(jxx::NEW<String>("LINEAR_B_IDEOGRAMS"), 0x10080, 0x100FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::AEGEAN_NUMBERS(new Character::UnicodeBlock(jxx::NEW<String>("AEGEAN_NUMBERS"), 0x10100, 0x1013F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::OLD_ITALIC(new Character::UnicodeBlock(jxx::NEW<String>("OLD_ITALIC"), 0x10300, 0x1032F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GOTHIC(new Character::UnicodeBlock(jxx::NEW<String>("GOTHIC"), 0x10330, 0x1034F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::UGARITIC(new Character::UnicodeBlock(jxx::NEW<String>("UGARITIC"), 0x10380, 0x1039F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::DESERET(new Character::UnicodeBlock(jxx::NEW<String>("DESERET"), 0x10400, 0x1044F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SHAVIAN(new Character::UnicodeBlock(jxx::NEW<String>("SHAVIAN"), 0x10450, 0x1047F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::OSMANYA(new Character::UnicodeBlock(jxx::NEW<String>("OSMANYA"), 0x10480, 0x104AF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CYPRIOT_SYLLABARY(new Character::UnicodeBlock(jxx::NEW<String>("CYPRIOT_SYLLABARY"), 0x10800, 0x1083F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BYZANTINE_MUSICAL_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("BYZANTINE_MUSICAL_SYMBOLS"), 0x1D000, 0x1D0FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MUSICAL_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("MUSICAL_SYMBOLS"), 0x1D100, 0x1D1FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TAI_XUAN_JING_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("TAI_XUAN_JING_SYMBOLS"), 0x1D300, 0x1D35F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MATHEMATICAL_ALPHANUMERIC_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("MATHEMATICAL_ALPHANUMERIC_SYMBOLS"), 0x1D400, 0x1D7FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B(new Character::UnicodeBlock(jxx::NEW<String>("CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B"), 0x20000, 0x2A6DF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT"), 0x2F800, 0x2FA1F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TAGS(new Character::UnicodeBlock(jxx::NEW<String>("TAGS"), 0xE0000, 0xE007F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::VARIATION_SELECTORS_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("VARIATION_SELECTORS_SUPPLEMENT"), 0xE0100, 0xE01EF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SUPPLEMENTARY_PRIVATE_USE_AREA_A(new Character::UnicodeBlock(jxx::NEW<String>("SUPPLEMENTARY_PRIVATE_USE_AREA_A"), 0xF0000, 0xFFFFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SUPPLEMENTARY_PRIVATE_USE_AREA_B(new Character::UnicodeBlock(jxx::NEW<String>("SUPPLEMENTARY_PRIVATE_USE_AREA_B"), 0x100000, 0x10FFFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HIGH_SURROGATES(new Character::UnicodeBlock(jxx::NEW<String>("HIGH_SURROGATES"), 0xD800, 0xDB7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HIGH_PRIVATE_USE_SURROGATES(new Character::UnicodeBlock(jxx::NEW<String>("HIGH_PRIVATE_USE_SURROGATES"), 0xDB80, 0xDBFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LOW_SURROGATES(new Character::UnicodeBlock(jxx::NEW<String>("LOW_SURROGATES"), 0xDC00, 0xDFFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ARABIC_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("ARABIC_SUPPLEMENT"), 0x750, 0x77F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::NKO(new Character::UnicodeBlock(jxx::NEW<String>("NKO"), 0x7C0, 0x7FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SAMARITAN(new Character::UnicodeBlock(jxx::NEW<String>("SAMARITAN"), 0x800, 0x83F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MANDAIC(new Character::UnicodeBlock(jxx::NEW<String>("MANDAIC"), 0x840, 0x85F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ETHIOPIC_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("ETHIOPIC_SUPPLEMENT"), 0x1380, 0x139F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED(new Character::UnicodeBlock(jxx::NEW<String>("UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED"), 0x18B0, 0x18FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::NEW_TAI_LUE(new Character::UnicodeBlock(jxx::NEW<String>("NEW_TAI_LUE"), 0x1980, 0x19DF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BUGINESE(new Character::UnicodeBlock(jxx::NEW<String>("BUGINESE"), 0x1A00, 0x1A1F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TAI_THAM(new Character::UnicodeBlock(jxx::NEW<String>("TAI_THAM"), 0x1A20, 0x1AAF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BALINESE(new Character::UnicodeBlock(jxx::NEW<String>("BALINESE"), 0x1B00, 0x1B7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SUNDANESE(new Character::UnicodeBlock(jxx::NEW<String>("SUNDANESE"), 0x1B80, 0x1BBF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BATAK(new Character::UnicodeBlock(jxx::NEW<String>("BATAK"), 0x1BC0, 0x1BFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LEPCHA(new Character::UnicodeBlock(jxx::NEW<String>("LEPCHA"), 0x1C00, 0x1C4F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::OL_CHIKI(new Character::UnicodeBlock(jxx::NEW<String>("OL_CHIKI"), 0x1C50, 0x1C7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::VEDIC_EXTENSIONS(new Character::UnicodeBlock(jxx::NEW<String>("VEDIC_EXTENSIONS"), 0x1CD0, 0x1CFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::PHONETIC_EXTENSIONS_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("PHONETIC_EXTENSIONS_SUPPLEMENT"), 0x1D80, 0x1DBF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::COMBINING_DIACRITICAL_MARKS_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("COMBINING_DIACRITICAL_MARKS_SUPPLEMENT"), 0x1DC0, 0x1DFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GLAGOLITIC(new Character::UnicodeBlock(jxx::NEW<String>("GLAGOLITIC"), 0x2C00, 0x2C5F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LATIN_EXTENDED_C(new Character::UnicodeBlock(jxx::NEW<String>("LATIN_EXTENDED_C"), 0x2C60, 0x2C7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::COPTIC(new Character::UnicodeBlock(jxx::NEW<String>("COPTIC"), 0x2C80, 0x2CFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GEORGIAN_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("GEORGIAN_SUPPLEMENT"), 0x2D00, 0x2D2F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TIFINAGH(new Character::UnicodeBlock(jxx::NEW<String>("TIFINAGH"), 0x2D30, 0x2D7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ETHIOPIC_EXTENDED(new Character::UnicodeBlock(jxx::NEW<String>("ETHIOPIC_EXTENDED"), 0x2D80, 0x2DDF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CYRILLIC_EXTENDED_A(new Character::UnicodeBlock(jxx::NEW<String>("CYRILLIC_EXTENDED_A"), 0x2DE0, 0x2DFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SUPPLEMENTAL_PUNCTUATION(new Character::UnicodeBlock(jxx::NEW<String>("SUPPLEMENTAL_PUNCTUATION"), 0x2E00, 0x2E7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_STROKES(new Character::UnicodeBlock(jxx::NEW<String>("CJK_STROKES"), 0x31C0, 0x31EF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LISU(new Character::UnicodeBlock(jxx::NEW<String>("LISU"), 0xA4D0, 0xA4FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::VAI(new Character::UnicodeBlock(jxx::NEW<String>("VAI"), 0xA500, 0xA63F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CYRILLIC_EXTENDED_B(new Character::UnicodeBlock(jxx::NEW<String>("CYRILLIC_EXTENDED_B"), 0xA640, 0xA69F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BAMUM(new Character::UnicodeBlock(jxx::NEW<String>("BAMUM"), 0xA6A0, 0xA6FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MODIFIER_TONE_LETTERS(new Character::UnicodeBlock(jxx::NEW<String>("MODIFIER_TONE_LETTERS"), 0xA700, 0xA71F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LATIN_EXTENDED_D(new Character::UnicodeBlock(jxx::NEW<String>("LATIN_EXTENDED_D"), 0xA720, 0xA7FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SYLOTI_NAGRI(new Character::UnicodeBlock(jxx::NEW<String>("SYLOTI_NAGRI"), 0xA800, 0xA82F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::COMMON_INDIC_NUMBER_FORMS(new Character::UnicodeBlock(jxx::NEW<String>("COMMON_INDIC_NUMBER_FORMS"), 0xA830, 0xA83F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::PHAGS_PA(new Character::UnicodeBlock(jxx::NEW<String>("PHAGS_PA"), 0xA840, 0xA87F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SAURASHTRA(new Character::UnicodeBlock(jxx::NEW<String>("SAURASHTRA"), 0xA880, 0xA8DF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::DEVANAGARI_EXTENDED(new Character::UnicodeBlock(jxx::NEW<String>("DEVANAGARI_EXTENDED"), 0xA8E0, 0xA8FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::KAYAH_LI(new Character::UnicodeBlock(jxx::NEW<String>("KAYAH_LI"), 0xA900, 0xA92F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::REJANG(new Character::UnicodeBlock(jxx::NEW<String>("REJANG"), 0xA930, 0xA95F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HANGUL_JAMO_EXTENDED_A(new Character::UnicodeBlock(jxx::NEW<String>("HANGUL_JAMO_EXTENDED_A"), 0xA960, 0xA97F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::JAVANESE(new Character::UnicodeBlock(jxx::NEW<String>("JAVANESE"), 0xA980, 0xA9DF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CHAM(new Character::UnicodeBlock(jxx::NEW<String>("CHAM"), 0xAA00, 0xAA5F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MYANMAR_EXTENDED_A(new Character::UnicodeBlock(jxx::NEW<String>("MYANMAR_EXTENDED_A"), 0xAA60, 0xAA7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TAI_VIET(new Character::UnicodeBlock(jxx::NEW<String>("TAI_VIET"), 0xAA80, 0xAADF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ETHIOPIC_EXTENDED_A(new Character::UnicodeBlock(jxx::NEW<String>("ETHIOPIC_EXTENDED_A"), 0xAB00, 0xAB2F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MEETEI_MAYEK(new Character::UnicodeBlock(jxx::NEW<String>("MEETEI_MAYEK"), 0xABC0, 0xABFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HANGUL_JAMO_EXTENDED_B(new Character::UnicodeBlock(jxx::NEW<String>("HANGUL_JAMO_EXTENDED_B"), 0xD7B0, 0xD7FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::VERTICAL_FORMS(new Character::UnicodeBlock(jxx::NEW<String>("VERTICAL_FORMS"), 0xFE10, 0xFE1F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ANCIENT_GREEK_NUMBERS(new Character::UnicodeBlock(jxx::NEW<String>("ANCIENT_GREEK_NUMBERS"), 0x10140, 0x1018F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ANCIENT_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("ANCIENT_SYMBOLS"), 0x10190, 0x101CF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::PHAISTOS_DISC(new Character::UnicodeBlock(jxx::NEW<String>("PHAISTOS_DISC"), 0x101D0, 0x101FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LYCIAN(new Character::UnicodeBlock(jxx::NEW<String>("LYCIAN"), 0x10280, 0x1029F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CARIAN(new Character::UnicodeBlock(jxx::NEW<String>("CARIAN"), 0x102A0, 0x102DF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::OLD_PERSIAN(new Character::UnicodeBlock(jxx::NEW<String>("OLD_PERSIAN"), 0x103A0, 0x103DF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::IMPERIAL_ARAMAIC(new Character::UnicodeBlock(jxx::NEW<String>("IMPERIAL_ARAMAIC"), 0x10840, 0x1085F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::PHOENICIAN(new Character::UnicodeBlock(jxx::NEW<String>("PHOENICIAN"), 0x10900, 0x1091F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LYDIAN(new Character::UnicodeBlock(jxx::NEW<String>("LYDIAN"), 0x10920, 0x1093F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::KHAROSHTHI(new Character::UnicodeBlock(jxx::NEW<String>("KHAROSHTHI"), 0x10A00, 0x10A5F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::OLD_SOUTH_ARABIAN(new Character::UnicodeBlock(jxx::NEW<String>("OLD_SOUTH_ARABIAN"), 0x10A60, 0x10A7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::AVESTAN(new Character::UnicodeBlock(jxx::NEW<String>("AVESTAN"), 0x10B00, 0x10B3F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::INSCRIPTIONAL_PARTHIAN(new Character::UnicodeBlock(jxx::NEW<String>("INSCRIPTIONAL_PARTHIAN"), 0x10B40, 0x10B5F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::INSCRIPTIONAL_PAHLAVI(new Character::UnicodeBlock(jxx::NEW<String>("INSCRIPTIONAL_PAHLAVI"), 0x10B60, 0x10B7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::OLD_TURKIC(new Character::UnicodeBlock(jxx::NEW<String>("OLD_TURKIC"), 0x10C00, 0x10C4F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::RUMI_NUMERAL_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("RUMI_NUMERAL_SYMBOLS"), 0x10E60, 0x10E7F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BRAHMI(new Character::UnicodeBlock(jxx::NEW<String>("BRAHMI"), 0x11000, 0x1107F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::KAITHI(new Character::UnicodeBlock(jxx::NEW<String>("KAITHI"), 0x11080, 0x110CF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CUNEIFORM(new Character::UnicodeBlock(jxx::NEW<String>("CUNEIFORM"), 0x12000, 0x123FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CUNEIFORM_NUMBERS_AND_PUNCTUATION(new Character::UnicodeBlock(jxx::NEW<String>("CUNEIFORM_NUMBERS_AND_PUNCTUATION"), 0x12400, 0x1247F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::EGYPTIAN_HIEROGLYPHS(new Character::UnicodeBlock(jxx::NEW<String>("EGYPTIAN_HIEROGLYPHS"), 0x13000, 0x1342F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BAMUM_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("BAMUM_SUPPLEMENT"), 0x16800, 0x16A3F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::KANA_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("KANA_SUPPLEMENT"), 0x1B000, 0x1B0FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ANCIENT_GREEK_MUSICAL_NOTATION(new Character::UnicodeBlock(jxx::NEW<String>("ANCIENT_GREEK_MUSICAL_NOTATION"), 0x1D200, 0x1D24F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::COUNTING_ROD_NUMERALS(new Character::UnicodeBlock(jxx::NEW<String>("COUNTING_ROD_NUMERALS"), 0x1D360, 0x1D37F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MAHJONG_TILES(new Character::UnicodeBlock(jxx::NEW<String>("MAHJONG_TILES"), 0x1F000, 0x1F02F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::DOMINO_TILES(new Character::UnicodeBlock(jxx::NEW<String>("DOMINO_TILES"), 0x1F030, 0x1F09F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::PLAYING_CARDS(new Character::UnicodeBlock(jxx::NEW<String>("PLAYING_CARDS"), 0x1F0A0, 0x1F0FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ENCLOSED_ALPHANUMERIC_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("ENCLOSED_ALPHANUMERIC_SUPPLEMENT"), 0x1F100, 0x1F1FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ENCLOSED_IDEOGRAPHIC_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("ENCLOSED_IDEOGRAPHIC_SUPPLEMENT"), 0x1F200, 0x1F2FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS(new Character::UnicodeBlock(jxx::NEW<String>("MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS"), 0x1F300, 0x1F5FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::EMOTICONS(new Character::UnicodeBlock(jxx::NEW<String>("EMOTICONS"), 0x1F600, 0x1F64F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TRANSPORT_AND_MAP_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("TRANSPORT_AND_MAP_SYMBOLS"), 0x1F680, 0x1F6FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ALCHEMICAL_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("ALCHEMICAL_SYMBOLS"), 0x1F700, 0x1F77F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C(new Character::UnicodeBlock(jxx::NEW<String>("CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C"), 0x2A700, 0x2B73F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D(new Character::UnicodeBlock(jxx::NEW<String>("CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D"), 0x2B740, 0x2B81F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ARABIC_EXTENDED_A(new Character::UnicodeBlock(jxx::NEW<String>("ARABIC_EXTENDED_A"), 0x8A0, 0x8FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SUNDANESE_SUPPLEMENT(new Character::UnicodeBlock(jxx::NEW<String>("SUNDANESE_SUPPLEMENT"), 0x1CC0, 0x1CCF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MEETEI_MAYEK_EXTENSIONS(new Character::UnicodeBlock(jxx::NEW<String>("MEETEI_MAYEK_EXTENSIONS"), 0xAAE0, 0xAAFF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MEROITIC_HIEROGLYPHS(new Character::UnicodeBlock(jxx::NEW<String>("MEROITIC_HIEROGLYPHS"), 0x10980, 0x1099F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MEROITIC_CURSIVE(new Character::UnicodeBlock(jxx::NEW<String>("MEROITIC_CURSIVE"), 0x109A0, 0x109FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SORA_SOMPENG(new Character::UnicodeBlock(jxx::NEW<String>("SORA_SOMPENG"), 0x110D0, 0x110FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CHAKMA(new Character::UnicodeBlock(jxx::NEW<String>("CHAKMA"), 0x11100, 0x1114F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::SHARADA(new Character::UnicodeBlock(jxx::NEW<String>("SHARADA"), 0x11180, 0x111DF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::TAKRI(new Character::UnicodeBlock(jxx::NEW<String>("TAKRI"), 0x11680, 0x116CF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::MIAO(new Character::UnicodeBlock(jxx::NEW<String>("MIAO"), 0x16F00, 0x16F9F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS(new Character::UnicodeBlock(jxx::NEW<String>("ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS"), 0x1EE00, 0x1EEFF));

    namespace {
        const std::vector<jxx::Ptr<Character::UnicodeBlock>>& unicodeBlocks() {
            static const std::vector<jxx::Ptr<Character::UnicodeBlock>> values = {
                Character::UnicodeBlock::BASIC_LATIN,
                Character::UnicodeBlock::LATIN_1_SUPPLEMENT,
                Character::UnicodeBlock::LATIN_EXTENDED_A,
                Character::UnicodeBlock::LATIN_EXTENDED_B,
                Character::UnicodeBlock::IPA_EXTENSIONS,
                Character::UnicodeBlock::SPACING_MODIFIER_LETTERS,
                Character::UnicodeBlock::COMBINING_DIACRITICAL_MARKS,
                Character::UnicodeBlock::GREEK,
                Character::UnicodeBlock::CYRILLIC,
                Character::UnicodeBlock::ARMENIAN,
                Character::UnicodeBlock::HEBREW,
                Character::UnicodeBlock::ARABIC,
                Character::UnicodeBlock::DEVANAGARI,
                Character::UnicodeBlock::BENGALI,
                Character::UnicodeBlock::GURMUKHI,
                Character::UnicodeBlock::GUJARATI,
                Character::UnicodeBlock::ORIYA,
                Character::UnicodeBlock::TAMIL,
                Character::UnicodeBlock::TELUGU,
                Character::UnicodeBlock::KANNADA,
                Character::UnicodeBlock::MALAYALAM,
                Character::UnicodeBlock::THAI,
                Character::UnicodeBlock::LAO,
                Character::UnicodeBlock::TIBETAN,
                Character::UnicodeBlock::GEORGIAN,
                Character::UnicodeBlock::HANGUL_JAMO,
                Character::UnicodeBlock::LATIN_EXTENDED_ADDITIONAL,
                Character::UnicodeBlock::GREEK_EXTENDED,
                Character::UnicodeBlock::GENERAL_PUNCTUATION,
                Character::UnicodeBlock::SUPERSCRIPTS_AND_SUBSCRIPTS,
                Character::UnicodeBlock::CURRENCY_SYMBOLS,
                Character::UnicodeBlock::COMBINING_MARKS_FOR_SYMBOLS,
                Character::UnicodeBlock::LETTERLIKE_SYMBOLS,
                Character::UnicodeBlock::NUMBER_FORMS,
                Character::UnicodeBlock::ARROWS,
                Character::UnicodeBlock::MATHEMATICAL_OPERATORS,
                Character::UnicodeBlock::MISCELLANEOUS_TECHNICAL,
                Character::UnicodeBlock::CONTROL_PICTURES,
                Character::UnicodeBlock::OPTICAL_CHARACTER_RECOGNITION,
                Character::UnicodeBlock::ENCLOSED_ALPHANUMERICS,
                Character::UnicodeBlock::BOX_DRAWING,
                Character::UnicodeBlock::BLOCK_ELEMENTS,
                Character::UnicodeBlock::GEOMETRIC_SHAPES,
                Character::UnicodeBlock::MISCELLANEOUS_SYMBOLS,
                Character::UnicodeBlock::DINGBATS,
                Character::UnicodeBlock::CJK_SYMBOLS_AND_PUNCTUATION,
                Character::UnicodeBlock::HIRAGANA,
                Character::UnicodeBlock::KATAKANA,
                Character::UnicodeBlock::BOPOMOFO,
                Character::UnicodeBlock::HANGUL_COMPATIBILITY_JAMO,
                Character::UnicodeBlock::KANBUN,
                Character::UnicodeBlock::ENCLOSED_CJK_LETTERS_AND_MONTHS,
                Character::UnicodeBlock::CJK_COMPATIBILITY,
                Character::UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS,
                Character::UnicodeBlock::HANGUL_SYLLABLES,
                Character::UnicodeBlock::PRIVATE_USE_AREA,
                Character::UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS,
                Character::UnicodeBlock::ALPHABETIC_PRESENTATION_FORMS,
                Character::UnicodeBlock::ARABIC_PRESENTATION_FORMS_A,
                Character::UnicodeBlock::COMBINING_HALF_MARKS,
                Character::UnicodeBlock::CJK_COMPATIBILITY_FORMS,
                Character::UnicodeBlock::SMALL_FORM_VARIANTS,
                Character::UnicodeBlock::ARABIC_PRESENTATION_FORMS_B,
                Character::UnicodeBlock::HALFWIDTH_AND_FULLWIDTH_FORMS,
                Character::UnicodeBlock::SPECIALS,
                Character::UnicodeBlock::SYRIAC,
                Character::UnicodeBlock::THAANA,
                Character::UnicodeBlock::SINHALA,
                Character::UnicodeBlock::MYANMAR,
                Character::UnicodeBlock::ETHIOPIC,
                Character::UnicodeBlock::CHEROKEE,
                Character::UnicodeBlock::UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS,
                Character::UnicodeBlock::OGHAM,
                Character::UnicodeBlock::RUNIC,
                Character::UnicodeBlock::KHMER,
                Character::UnicodeBlock::MONGOLIAN,
                Character::UnicodeBlock::BRAILLE_PATTERNS,
                Character::UnicodeBlock::CJK_RADICALS_SUPPLEMENT,
                Character::UnicodeBlock::KANGXI_RADICALS,
                Character::UnicodeBlock::IDEOGRAPHIC_DESCRIPTION_CHARACTERS,
                Character::UnicodeBlock::BOPOMOFO_EXTENDED,
                Character::UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A,
                Character::UnicodeBlock::YI_SYLLABLES,
                Character::UnicodeBlock::YI_RADICALS,
                Character::UnicodeBlock::CYRILLIC_SUPPLEMENTARY,
                Character::UnicodeBlock::TAGALOG,
                Character::UnicodeBlock::HANUNOO,
                Character::UnicodeBlock::BUHID,
                Character::UnicodeBlock::TAGBANWA,
                Character::UnicodeBlock::LIMBU,
                Character::UnicodeBlock::TAI_LE,
                Character::UnicodeBlock::KHMER_SYMBOLS,
                Character::UnicodeBlock::PHONETIC_EXTENSIONS,
                Character::UnicodeBlock::MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A,
                Character::UnicodeBlock::SUPPLEMENTAL_ARROWS_A,
                Character::UnicodeBlock::SUPPLEMENTAL_ARROWS_B,
                Character::UnicodeBlock::MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B,
                Character::UnicodeBlock::SUPPLEMENTAL_MATHEMATICAL_OPERATORS,
                Character::UnicodeBlock::MISCELLANEOUS_SYMBOLS_AND_ARROWS,
                Character::UnicodeBlock::KATAKANA_PHONETIC_EXTENSIONS,
                Character::UnicodeBlock::YIJING_HEXAGRAM_SYMBOLS,
                Character::UnicodeBlock::VARIATION_SELECTORS,
                Character::UnicodeBlock::LINEAR_B_SYLLABARY,
                Character::UnicodeBlock::LINEAR_B_IDEOGRAMS,
                Character::UnicodeBlock::AEGEAN_NUMBERS,
                Character::UnicodeBlock::OLD_ITALIC,
                Character::UnicodeBlock::GOTHIC,
                Character::UnicodeBlock::UGARITIC,
                Character::UnicodeBlock::DESERET,
                Character::UnicodeBlock::SHAVIAN,
                Character::UnicodeBlock::OSMANYA,
                Character::UnicodeBlock::CYPRIOT_SYLLABARY,
                Character::UnicodeBlock::BYZANTINE_MUSICAL_SYMBOLS,
                Character::UnicodeBlock::MUSICAL_SYMBOLS,
                Character::UnicodeBlock::TAI_XUAN_JING_SYMBOLS,
                Character::UnicodeBlock::MATHEMATICAL_ALPHANUMERIC_SYMBOLS,
                Character::UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B,
                Character::UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT,
                Character::UnicodeBlock::TAGS,
                Character::UnicodeBlock::VARIATION_SELECTORS_SUPPLEMENT,
                Character::UnicodeBlock::SUPPLEMENTARY_PRIVATE_USE_AREA_A,
                Character::UnicodeBlock::SUPPLEMENTARY_PRIVATE_USE_AREA_B,
                Character::UnicodeBlock::HIGH_SURROGATES,
                Character::UnicodeBlock::HIGH_PRIVATE_USE_SURROGATES,
                Character::UnicodeBlock::LOW_SURROGATES,
                Character::UnicodeBlock::ARABIC_SUPPLEMENT,
                Character::UnicodeBlock::NKO,
                Character::UnicodeBlock::SAMARITAN,
                Character::UnicodeBlock::MANDAIC,
                Character::UnicodeBlock::ETHIOPIC_SUPPLEMENT,
                Character::UnicodeBlock::UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED,
                Character::UnicodeBlock::NEW_TAI_LUE,
                Character::UnicodeBlock::BUGINESE,
                Character::UnicodeBlock::TAI_THAM,
                Character::UnicodeBlock::BALINESE,
                Character::UnicodeBlock::SUNDANESE,
                Character::UnicodeBlock::BATAK,
                Character::UnicodeBlock::LEPCHA,
                Character::UnicodeBlock::OL_CHIKI,
                Character::UnicodeBlock::VEDIC_EXTENSIONS,
                Character::UnicodeBlock::PHONETIC_EXTENSIONS_SUPPLEMENT,
                Character::UnicodeBlock::COMBINING_DIACRITICAL_MARKS_SUPPLEMENT,
                Character::UnicodeBlock::GLAGOLITIC,
                Character::UnicodeBlock::LATIN_EXTENDED_C,
                Character::UnicodeBlock::COPTIC,
                Character::UnicodeBlock::GEORGIAN_SUPPLEMENT,
                Character::UnicodeBlock::TIFINAGH,
                Character::UnicodeBlock::ETHIOPIC_EXTENDED,
                Character::UnicodeBlock::CYRILLIC_EXTENDED_A,
                Character::UnicodeBlock::SUPPLEMENTAL_PUNCTUATION,
                Character::UnicodeBlock::CJK_STROKES,
                Character::UnicodeBlock::LISU,
                Character::UnicodeBlock::VAI,
                Character::UnicodeBlock::CYRILLIC_EXTENDED_B,
                Character::UnicodeBlock::BAMUM,
                Character::UnicodeBlock::MODIFIER_TONE_LETTERS,
                Character::UnicodeBlock::LATIN_EXTENDED_D,
                Character::UnicodeBlock::SYLOTI_NAGRI,
                Character::UnicodeBlock::COMMON_INDIC_NUMBER_FORMS,
                Character::UnicodeBlock::PHAGS_PA,
                Character::UnicodeBlock::SAURASHTRA,
                Character::UnicodeBlock::DEVANAGARI_EXTENDED,
                Character::UnicodeBlock::KAYAH_LI,
                Character::UnicodeBlock::REJANG,
                Character::UnicodeBlock::HANGUL_JAMO_EXTENDED_A,
                Character::UnicodeBlock::JAVANESE,
                Character::UnicodeBlock::CHAM,
                Character::UnicodeBlock::MYANMAR_EXTENDED_A,
                Character::UnicodeBlock::TAI_VIET,
                Character::UnicodeBlock::ETHIOPIC_EXTENDED_A,
                Character::UnicodeBlock::MEETEI_MAYEK,
                Character::UnicodeBlock::HANGUL_JAMO_EXTENDED_B,
                Character::UnicodeBlock::VERTICAL_FORMS,
                Character::UnicodeBlock::ANCIENT_GREEK_NUMBERS,
                Character::UnicodeBlock::ANCIENT_SYMBOLS,
                Character::UnicodeBlock::PHAISTOS_DISC,
                Character::UnicodeBlock::LYCIAN,
                Character::UnicodeBlock::CARIAN,
                Character::UnicodeBlock::OLD_PERSIAN,
                Character::UnicodeBlock::IMPERIAL_ARAMAIC,
                Character::UnicodeBlock::PHOENICIAN,
                Character::UnicodeBlock::LYDIAN,
                Character::UnicodeBlock::KHAROSHTHI,
                Character::UnicodeBlock::OLD_SOUTH_ARABIAN,
                Character::UnicodeBlock::AVESTAN,
                Character::UnicodeBlock::INSCRIPTIONAL_PARTHIAN,
                Character::UnicodeBlock::INSCRIPTIONAL_PAHLAVI,
                Character::UnicodeBlock::OLD_TURKIC,
                Character::UnicodeBlock::RUMI_NUMERAL_SYMBOLS,
                Character::UnicodeBlock::BRAHMI,
                Character::UnicodeBlock::KAITHI,
                Character::UnicodeBlock::CUNEIFORM,
                Character::UnicodeBlock::CUNEIFORM_NUMBERS_AND_PUNCTUATION,
                Character::UnicodeBlock::EGYPTIAN_HIEROGLYPHS,
                Character::UnicodeBlock::BAMUM_SUPPLEMENT,
                Character::UnicodeBlock::KANA_SUPPLEMENT,
                Character::UnicodeBlock::ANCIENT_GREEK_MUSICAL_NOTATION,
                Character::UnicodeBlock::COUNTING_ROD_NUMERALS,
                Character::UnicodeBlock::MAHJONG_TILES,
                Character::UnicodeBlock::DOMINO_TILES,
                Character::UnicodeBlock::PLAYING_CARDS,
                Character::UnicodeBlock::ENCLOSED_ALPHANUMERIC_SUPPLEMENT,
                Character::UnicodeBlock::ENCLOSED_IDEOGRAPHIC_SUPPLEMENT,
                Character::UnicodeBlock::MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS,
                Character::UnicodeBlock::EMOTICONS,
                Character::UnicodeBlock::TRANSPORT_AND_MAP_SYMBOLS,
                Character::UnicodeBlock::ALCHEMICAL_SYMBOLS,
                Character::UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C,
                Character::UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D,
                Character::UnicodeBlock::ARABIC_EXTENDED_A,
                Character::UnicodeBlock::SUNDANESE_SUPPLEMENT,
                Character::UnicodeBlock::MEETEI_MAYEK_EXTENSIONS,
                Character::UnicodeBlock::MEROITIC_HIEROGLYPHS,
                Character::UnicodeBlock::MEROITIC_CURSIVE,
                Character::UnicodeBlock::SORA_SOMPENG,
                Character::UnicodeBlock::CHAKMA,
                Character::UnicodeBlock::SHARADA,
                Character::UnicodeBlock::TAKRI,
                Character::UnicodeBlock::MIAO,
                Character::UnicodeBlock::ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS,
            };
            return values;
        }
    }

    jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::of(jchar codeUnit) { return of(static_cast<jint>(codeUnit)); }

    jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::of(jint codePoint) {
        if (!Character::isValidCodePoint(codePoint)) throw IllegalArgumentException();
        for (const auto& block : unicodeBlocks()) {
            if (codePoint >= block->start_ && codePoint <= block->end_) return block;
        }
        return nullptr;
    }

    jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::forName(const jxx::Ptr<String>& blockName) {
        const auto requested = normalizedUnicodeName(blockName);
        for (const auto& block : unicodeBlocks()) {
            if (normalizedUnicodeName(block->toString()) == requested) return block;
        }
        if (requested == "GREEKANDCOPTIC") return GREEK;
        if (requested == "COMBININGDIACRITICALMARKSFORSYMBOLS") return COMBINING_MARKS_FOR_SYMBOLS;
        if (requested == "CYRILLICSUPPLEMENT") return CYRILLIC_SUPPLEMENTARY;
        if (requested == "SURROGATESAREA") return SURROGATES_AREA;
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

    ::jxx::lang::jint Character::codePointAt(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& seq,
        ::jxx::lang::jint index)
    {
        if (seq == nullptr) throw ::jxx::lang::NullPointerException();
        const auto length = seq->length();
        if (index < 0 || index >= length) throw ::jxx::lang::IndexOutOfBoundsException();
        const auto high = seq->charAt(index);
        if (isHighSurrogate(high) && index + 1 < length) {
            const auto low = seq->charAt(index + 1);
            if (isLowSurrogate(low)) return toCodePoint(high, low);
        }
        return static_cast<::jxx::lang::jint>(high);
    }

    ::jxx::lang::jint Character::codePointAt(
        const ::jxx::lang::CharArray& a,
        ::jxx::lang::jint index)
    {
        if (a == nullptr) throw ::jxx::lang::NullPointerException();
        return codePointAt(a, index, static_cast<::jxx::lang::jint>(a->length));
    }

    ::jxx::lang::jint Character::codePointAt(
        const ::jxx::lang::CharArray& a,
        ::jxx::lang::jint index,
        ::jxx::lang::jint limit)
    {
        if (a == nullptr) throw ::jxx::lang::NullPointerException();
        const auto length = static_cast<::jxx::lang::jint>(a->length);
        if (index < 0 || index >= limit || limit < 0 || limit > length) {
            throw ::jxx::lang::IndexOutOfBoundsException();
        }
        const auto high = (*a)[index];
        if (isHighSurrogate(high) && index + 1 < limit) {
            const auto low = (*a)[index + 1];
            if (isLowSurrogate(low)) return toCodePoint(high, low);
        }
        return static_cast<::jxx::lang::jint>(high);
    }

    ::jxx::lang::jint Character::codePointBefore(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& seq,
        ::jxx::lang::jint index)
    {
        if (seq == nullptr) throw ::jxx::lang::NullPointerException();
        if (index < 1 || index > seq->length()) throw ::jxx::lang::IndexOutOfBoundsException();
        const auto low = seq->charAt(index - 1);
        if (isLowSurrogate(low) && index - 2 >= 0) {
            const auto high = seq->charAt(index - 2);
            if (isHighSurrogate(high)) return toCodePoint(high, low);
        }
        return static_cast<::jxx::lang::jint>(low);
    }

    ::jxx::lang::jint Character::codePointBefore(
        const ::jxx::lang::CharArray& a,
        ::jxx::lang::jint index)
    {
        if (a == nullptr) throw ::jxx::lang::NullPointerException();
        return codePointBefore(a, index, 0);
    }

    ::jxx::lang::jint Character::codePointBefore(
        const ::jxx::lang::CharArray& a,
        ::jxx::lang::jint index,
        ::jxx::lang::jint start)
    {
        if (a == nullptr) throw ::jxx::lang::NullPointerException();
        const auto length = static_cast<::jxx::lang::jint>(a->length);
        if (start < 0 || start >= index || index > length) throw ::jxx::lang::IndexOutOfBoundsException();
        const auto low = (*a)[index - 1];
        if (isLowSurrogate(low) && index - 2 >= start) {
            const auto high = (*a)[index - 2];
            if (isHighSurrogate(high)) return toCodePoint(high, low);
        }
        return static_cast<::jxx::lang::jint>(low);
    }

    ::jxx::lang::jint Character::codePointCount(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& seq,
        ::jxx::lang::jint beginIndex,
        ::jxx::lang::jint endIndex)
    {
        if (seq == nullptr) throw ::jxx::lang::NullPointerException();
        const auto length = seq->length();
        if (beginIndex < 0 || endIndex > length || beginIndex > endIndex) {
            throw ::jxx::lang::IndexOutOfBoundsException();
        }
        ::jxx::lang::jint result = 0;
        for (auto i = beginIndex; i < endIndex; ++i, ++result) {
            const auto high = seq->charAt(i);
            if (isHighSurrogate(high) && i + 1 < endIndex && isLowSurrogate(seq->charAt(i + 1))) ++i;
        }
        return result;
    }

    ::jxx::lang::jint Character::codePointCount(
        const ::jxx::lang::CharArray& a,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint count)
    {
        if (a == nullptr) throw ::jxx::lang::NullPointerException();
        const auto length = static_cast<::jxx::lang::jint>(a->length);
        if (offset < 0 || count < 0 || offset > length - count) throw ::jxx::lang::IndexOutOfBoundsException();
        const auto limit = offset + count;
        ::jxx::lang::jint result = 0;
        for (auto i = offset; i < limit; ++i, ++result) {
            const auto high = (*a)[i];
            if (isHighSurrogate(high) && i + 1 < limit && isLowSurrogate((*a)[i + 1])) ++i;
        }
        return result;
    }

    ::jxx::lang::jint Character::offsetByCodePoints(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& seq,
        ::jxx::lang::jint index,
        ::jxx::lang::jint codePointOffset)
    {
        if (seq == nullptr) throw ::jxx::lang::NullPointerException();
        const auto length = seq->length();
        if (index < 0 || index > length) throw ::jxx::lang::IndexOutOfBoundsException();
        auto result = index;
        if (codePointOffset >= 0) {
            for (auto remaining = codePointOffset; remaining > 0; --remaining) {
                if (result >= length) throw ::jxx::lang::IndexOutOfBoundsException();
                const auto high = seq->charAt(result++);
                if (isHighSurrogate(high) && result < length && isLowSurrogate(seq->charAt(result))) ++result;
            }
        } else {
            for (auto remaining = codePointOffset; remaining < 0; ++remaining) {
                if (result <= 0) throw ::jxx::lang::IndexOutOfBoundsException();
                const auto low = seq->charAt(--result);
                if (isLowSurrogate(low) && result > 0 && isHighSurrogate(seq->charAt(result - 1))) --result;
            }
        }
        return result;
    }

    ::jxx::lang::jint Character::offsetByCodePoints(
        const ::jxx::lang::CharArray& a,
        ::jxx::lang::jint start,
        ::jxx::lang::jint count,
        ::jxx::lang::jint index,
        ::jxx::lang::jint codePointOffset)
    {
        if (a == nullptr) throw ::jxx::lang::NullPointerException();
        const auto length = static_cast<::jxx::lang::jint>(a->length);
        if (start < 0 || count < 0 || start > length - count || index < start || index > start + count) {
            throw ::jxx::lang::IndexOutOfBoundsException();
        }
        const auto limit = start + count;
        auto result = index;
        if (codePointOffset >= 0) {
            for (auto remaining = codePointOffset; remaining > 0; --remaining) {
                if (result >= limit) throw ::jxx::lang::IndexOutOfBoundsException();
                const auto high = (*a)[result++];
                if (isHighSurrogate(high) && result < limit && isLowSurrogate((*a)[result])) ++result;
            }
        } else {
            for (auto remaining = codePointOffset; remaining < 0; ++remaining) {
                if (result <= start) throw ::jxx::lang::IndexOutOfBoundsException();
                const auto low = (*a)[--result];
                if (isLowSurrogate(low) && result > start && isHighSurrogate((*a)[result - 1])) --result;
            }
        }
        return result;
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




    ::jxx::lang::jint Character::toChars(
        ::jxx::lang::jint codePoint,
        const ::jxx::lang::CharArray& dst,
        ::jxx::lang::jint dstIndex)
    {
        if (!isValidCodePoint(codePoint)) throwIAE_("invalid Unicode code point");
        if (dst == nullptr) throw ::jxx::lang::NullPointerException();
        const auto required = charCount(codePoint);
        const auto length = static_cast<::jxx::lang::jint>(dst->length);
        if (dstIndex < 0 || dstIndex > length - required) throw ::jxx::lang::IndexOutOfBoundsException();
        if (required == 1) {
            (*dst)[dstIndex] = static_cast<::jxx::lang::jchar>(codePoint);
        } else {
            (*dst)[dstIndex] = highSurrogate(codePoint);
            (*dst)[dstIndex + 1] = lowSurrogate(codePoint);
        }
        return required;
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