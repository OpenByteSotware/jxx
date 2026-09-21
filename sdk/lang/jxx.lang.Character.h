#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.buildin_array.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Enum.h"

namespace jxx::lang
{
    class ClassAny;
    class String;
    class CharSequence;

    class Character final
		: public ClassBase<Character, Object, Comparable<Character>, jxx::io::SerializableI>
    {
    public:
        class Subset : public ClassBase<Subset, Object> {
        public:
            jxx::Ptr<String> toString() const override;

        protected:
            explicit Subset(const jxx::Ptr<String>& name);

        private:
            jxx::Ptr<String> name_;
        };

        class UnicodeBlock final : public ClassBase<UnicodeBlock, Subset> {
        public:
            static jxx::Ptr<UnicodeBlock> of(jchar codeUnit);
            static jxx::Ptr<UnicodeBlock> of(jint codePoint);
            static jxx::Ptr<UnicodeBlock> forName(const jxx::Ptr<String>& blockName);

            static const jxx::Ptr<UnicodeBlock> BASIC_LATIN;
            static const jxx::Ptr<UnicodeBlock> LATIN_1_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> LATIN_EXTENDED_A;
            static const jxx::Ptr<UnicodeBlock> LATIN_EXTENDED_B;
            static const jxx::Ptr<UnicodeBlock> IPA_EXTENSIONS;
            static const jxx::Ptr<UnicodeBlock> SPACING_MODIFIER_LETTERS;
            static const jxx::Ptr<UnicodeBlock> COMBINING_DIACRITICAL_MARKS;
            static const jxx::Ptr<UnicodeBlock> GREEK;
            static const jxx::Ptr<UnicodeBlock> CYRILLIC;
            static const jxx::Ptr<UnicodeBlock> ARMENIAN;
            static const jxx::Ptr<UnicodeBlock> HEBREW;
            static const jxx::Ptr<UnicodeBlock> ARABIC;
            static const jxx::Ptr<UnicodeBlock> DEVANAGARI;
            static const jxx::Ptr<UnicodeBlock> BENGALI;
            static const jxx::Ptr<UnicodeBlock> GURMUKHI;
            static const jxx::Ptr<UnicodeBlock> GUJARATI;
            static const jxx::Ptr<UnicodeBlock> ORIYA;
            static const jxx::Ptr<UnicodeBlock> TAMIL;
            static const jxx::Ptr<UnicodeBlock> TELUGU;
            static const jxx::Ptr<UnicodeBlock> KANNADA;
            static const jxx::Ptr<UnicodeBlock> MALAYALAM;
            static const jxx::Ptr<UnicodeBlock> THAI;
            static const jxx::Ptr<UnicodeBlock> LAO;
            static const jxx::Ptr<UnicodeBlock> TIBETAN;
            static const jxx::Ptr<UnicodeBlock> GEORGIAN;
            static const jxx::Ptr<UnicodeBlock> HANGUL_JAMO;
            static const jxx::Ptr<UnicodeBlock> LATIN_EXTENDED_ADDITIONAL;
            static const jxx::Ptr<UnicodeBlock> GREEK_EXTENDED;
            static const jxx::Ptr<UnicodeBlock> GENERAL_PUNCTUATION;
            static const jxx::Ptr<UnicodeBlock> SUPERSCRIPTS_AND_SUBSCRIPTS;
            static const jxx::Ptr<UnicodeBlock> CURRENCY_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> COMBINING_MARKS_FOR_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> LETTERLIKE_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> NUMBER_FORMS;
            static const jxx::Ptr<UnicodeBlock> ARROWS;
            static const jxx::Ptr<UnicodeBlock> MATHEMATICAL_OPERATORS;
            static const jxx::Ptr<UnicodeBlock> MISCELLANEOUS_TECHNICAL;
            static const jxx::Ptr<UnicodeBlock> CONTROL_PICTURES;
            static const jxx::Ptr<UnicodeBlock> OPTICAL_CHARACTER_RECOGNITION;
            static const jxx::Ptr<UnicodeBlock> ENCLOSED_ALPHANUMERICS;
            static const jxx::Ptr<UnicodeBlock> BOX_DRAWING;
            static const jxx::Ptr<UnicodeBlock> BLOCK_ELEMENTS;
            static const jxx::Ptr<UnicodeBlock> GEOMETRIC_SHAPES;
            static const jxx::Ptr<UnicodeBlock> MISCELLANEOUS_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> DINGBATS;
            static const jxx::Ptr<UnicodeBlock> CJK_SYMBOLS_AND_PUNCTUATION;
            static const jxx::Ptr<UnicodeBlock> HIRAGANA;
            static const jxx::Ptr<UnicodeBlock> KATAKANA;
            static const jxx::Ptr<UnicodeBlock> BOPOMOFO;
            static const jxx::Ptr<UnicodeBlock> HANGUL_COMPATIBILITY_JAMO;
            static const jxx::Ptr<UnicodeBlock> KANBUN;
            static const jxx::Ptr<UnicodeBlock> ENCLOSED_CJK_LETTERS_AND_MONTHS;
            static const jxx::Ptr<UnicodeBlock> CJK_COMPATIBILITY;
            static const jxx::Ptr<UnicodeBlock> CJK_UNIFIED_IDEOGRAPHS;
            static const jxx::Ptr<UnicodeBlock> HANGUL_SYLLABLES;
            static const jxx::Ptr<UnicodeBlock> PRIVATE_USE_AREA;
            static const jxx::Ptr<UnicodeBlock> CJK_COMPATIBILITY_IDEOGRAPHS;
            static const jxx::Ptr<UnicodeBlock> ALPHABETIC_PRESENTATION_FORMS;
            static const jxx::Ptr<UnicodeBlock> ARABIC_PRESENTATION_FORMS_A;
            static const jxx::Ptr<UnicodeBlock> COMBINING_HALF_MARKS;
            static const jxx::Ptr<UnicodeBlock> CJK_COMPATIBILITY_FORMS;
            static const jxx::Ptr<UnicodeBlock> SMALL_FORM_VARIANTS;
            static const jxx::Ptr<UnicodeBlock> ARABIC_PRESENTATION_FORMS_B;
            static const jxx::Ptr<UnicodeBlock> HALFWIDTH_AND_FULLWIDTH_FORMS;
            static const jxx::Ptr<UnicodeBlock> SPECIALS;
            static const jxx::Ptr<UnicodeBlock> SURROGATES_AREA;
            static const jxx::Ptr<UnicodeBlock> SYRIAC;
            static const jxx::Ptr<UnicodeBlock> THAANA;
            static const jxx::Ptr<UnicodeBlock> SINHALA;
            static const jxx::Ptr<UnicodeBlock> MYANMAR;
            static const jxx::Ptr<UnicodeBlock> ETHIOPIC;
            static const jxx::Ptr<UnicodeBlock> CHEROKEE;
            static const jxx::Ptr<UnicodeBlock> UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS;
            static const jxx::Ptr<UnicodeBlock> OGHAM;
            static const jxx::Ptr<UnicodeBlock> RUNIC;
            static const jxx::Ptr<UnicodeBlock> KHMER;
            static const jxx::Ptr<UnicodeBlock> MONGOLIAN;
            static const jxx::Ptr<UnicodeBlock> BRAILLE_PATTERNS;
            static const jxx::Ptr<UnicodeBlock> CJK_RADICALS_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> KANGXI_RADICALS;
            static const jxx::Ptr<UnicodeBlock> IDEOGRAPHIC_DESCRIPTION_CHARACTERS;
            static const jxx::Ptr<UnicodeBlock> BOPOMOFO_EXTENDED;
            static const jxx::Ptr<UnicodeBlock> CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A;
            static const jxx::Ptr<UnicodeBlock> YI_SYLLABLES;
            static const jxx::Ptr<UnicodeBlock> YI_RADICALS;
            static const jxx::Ptr<UnicodeBlock> CYRILLIC_SUPPLEMENTARY;
            static const jxx::Ptr<UnicodeBlock> TAGALOG;
            static const jxx::Ptr<UnicodeBlock> HANUNOO;
            static const jxx::Ptr<UnicodeBlock> BUHID;
            static const jxx::Ptr<UnicodeBlock> TAGBANWA;
            static const jxx::Ptr<UnicodeBlock> LIMBU;
            static const jxx::Ptr<UnicodeBlock> TAI_LE;
            static const jxx::Ptr<UnicodeBlock> KHMER_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> PHONETIC_EXTENSIONS;
            static const jxx::Ptr<UnicodeBlock> MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A;
            static const jxx::Ptr<UnicodeBlock> SUPPLEMENTAL_ARROWS_A;
            static const jxx::Ptr<UnicodeBlock> SUPPLEMENTAL_ARROWS_B;
            static const jxx::Ptr<UnicodeBlock> MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B;
            static const jxx::Ptr<UnicodeBlock> SUPPLEMENTAL_MATHEMATICAL_OPERATORS;
            static const jxx::Ptr<UnicodeBlock> MISCELLANEOUS_SYMBOLS_AND_ARROWS;
            static const jxx::Ptr<UnicodeBlock> KATAKANA_PHONETIC_EXTENSIONS;
            static const jxx::Ptr<UnicodeBlock> YIJING_HEXAGRAM_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> VARIATION_SELECTORS;
            static const jxx::Ptr<UnicodeBlock> LINEAR_B_SYLLABARY;
            static const jxx::Ptr<UnicodeBlock> LINEAR_B_IDEOGRAMS;
            static const jxx::Ptr<UnicodeBlock> AEGEAN_NUMBERS;
            static const jxx::Ptr<UnicodeBlock> OLD_ITALIC;
            static const jxx::Ptr<UnicodeBlock> GOTHIC;
            static const jxx::Ptr<UnicodeBlock> UGARITIC;
            static const jxx::Ptr<UnicodeBlock> DESERET;
            static const jxx::Ptr<UnicodeBlock> SHAVIAN;
            static const jxx::Ptr<UnicodeBlock> OSMANYA;
            static const jxx::Ptr<UnicodeBlock> CYPRIOT_SYLLABARY;
            static const jxx::Ptr<UnicodeBlock> BYZANTINE_MUSICAL_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> MUSICAL_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> TAI_XUAN_JING_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> MATHEMATICAL_ALPHANUMERIC_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B;
            static const jxx::Ptr<UnicodeBlock> CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> TAGS;
            static const jxx::Ptr<UnicodeBlock> VARIATION_SELECTORS_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> SUPPLEMENTARY_PRIVATE_USE_AREA_A;
            static const jxx::Ptr<UnicodeBlock> SUPPLEMENTARY_PRIVATE_USE_AREA_B;
            static const jxx::Ptr<UnicodeBlock> HIGH_SURROGATES;
            static const jxx::Ptr<UnicodeBlock> HIGH_PRIVATE_USE_SURROGATES;
            static const jxx::Ptr<UnicodeBlock> LOW_SURROGATES;
            static const jxx::Ptr<UnicodeBlock> ARABIC_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> NKO;
            static const jxx::Ptr<UnicodeBlock> SAMARITAN;
            static const jxx::Ptr<UnicodeBlock> MANDAIC;
            static const jxx::Ptr<UnicodeBlock> ETHIOPIC_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED;
            static const jxx::Ptr<UnicodeBlock> NEW_TAI_LUE;
            static const jxx::Ptr<UnicodeBlock> BUGINESE;
            static const jxx::Ptr<UnicodeBlock> TAI_THAM;
            static const jxx::Ptr<UnicodeBlock> BALINESE;
            static const jxx::Ptr<UnicodeBlock> SUNDANESE;
            static const jxx::Ptr<UnicodeBlock> BATAK;
            static const jxx::Ptr<UnicodeBlock> LEPCHA;
            static const jxx::Ptr<UnicodeBlock> OL_CHIKI;
            static const jxx::Ptr<UnicodeBlock> VEDIC_EXTENSIONS;
            static const jxx::Ptr<UnicodeBlock> PHONETIC_EXTENSIONS_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> COMBINING_DIACRITICAL_MARKS_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> GLAGOLITIC;
            static const jxx::Ptr<UnicodeBlock> LATIN_EXTENDED_C;
            static const jxx::Ptr<UnicodeBlock> COPTIC;
            static const jxx::Ptr<UnicodeBlock> GEORGIAN_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> TIFINAGH;
            static const jxx::Ptr<UnicodeBlock> ETHIOPIC_EXTENDED;
            static const jxx::Ptr<UnicodeBlock> CYRILLIC_EXTENDED_A;
            static const jxx::Ptr<UnicodeBlock> SUPPLEMENTAL_PUNCTUATION;
            static const jxx::Ptr<UnicodeBlock> CJK_STROKES;
            static const jxx::Ptr<UnicodeBlock> LISU;
            static const jxx::Ptr<UnicodeBlock> VAI;
            static const jxx::Ptr<UnicodeBlock> CYRILLIC_EXTENDED_B;
            static const jxx::Ptr<UnicodeBlock> BAMUM;
            static const jxx::Ptr<UnicodeBlock> MODIFIER_TONE_LETTERS;
            static const jxx::Ptr<UnicodeBlock> LATIN_EXTENDED_D;
            static const jxx::Ptr<UnicodeBlock> SYLOTI_NAGRI;
            static const jxx::Ptr<UnicodeBlock> COMMON_INDIC_NUMBER_FORMS;
            static const jxx::Ptr<UnicodeBlock> PHAGS_PA;
            static const jxx::Ptr<UnicodeBlock> SAURASHTRA;
            static const jxx::Ptr<UnicodeBlock> DEVANAGARI_EXTENDED;
            static const jxx::Ptr<UnicodeBlock> KAYAH_LI;
            static const jxx::Ptr<UnicodeBlock> REJANG;
            static const jxx::Ptr<UnicodeBlock> HANGUL_JAMO_EXTENDED_A;
            static const jxx::Ptr<UnicodeBlock> JAVANESE;
            static const jxx::Ptr<UnicodeBlock> CHAM;
            static const jxx::Ptr<UnicodeBlock> MYANMAR_EXTENDED_A;
            static const jxx::Ptr<UnicodeBlock> TAI_VIET;
            static const jxx::Ptr<UnicodeBlock> ETHIOPIC_EXTENDED_A;
            static const jxx::Ptr<UnicodeBlock> MEETEI_MAYEK;
            static const jxx::Ptr<UnicodeBlock> HANGUL_JAMO_EXTENDED_B;
            static const jxx::Ptr<UnicodeBlock> VERTICAL_FORMS;
            static const jxx::Ptr<UnicodeBlock> ANCIENT_GREEK_NUMBERS;
            static const jxx::Ptr<UnicodeBlock> ANCIENT_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> PHAISTOS_DISC;
            static const jxx::Ptr<UnicodeBlock> LYCIAN;
            static const jxx::Ptr<UnicodeBlock> CARIAN;
            static const jxx::Ptr<UnicodeBlock> OLD_PERSIAN;
            static const jxx::Ptr<UnicodeBlock> IMPERIAL_ARAMAIC;
            static const jxx::Ptr<UnicodeBlock> PHOENICIAN;
            static const jxx::Ptr<UnicodeBlock> LYDIAN;
            static const jxx::Ptr<UnicodeBlock> KHAROSHTHI;
            static const jxx::Ptr<UnicodeBlock> OLD_SOUTH_ARABIAN;
            static const jxx::Ptr<UnicodeBlock> AVESTAN;
            static const jxx::Ptr<UnicodeBlock> INSCRIPTIONAL_PARTHIAN;
            static const jxx::Ptr<UnicodeBlock> INSCRIPTIONAL_PAHLAVI;
            static const jxx::Ptr<UnicodeBlock> OLD_TURKIC;
            static const jxx::Ptr<UnicodeBlock> RUMI_NUMERAL_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> BRAHMI;
            static const jxx::Ptr<UnicodeBlock> KAITHI;
            static const jxx::Ptr<UnicodeBlock> CUNEIFORM;
            static const jxx::Ptr<UnicodeBlock> CUNEIFORM_NUMBERS_AND_PUNCTUATION;
            static const jxx::Ptr<UnicodeBlock> EGYPTIAN_HIEROGLYPHS;
            static const jxx::Ptr<UnicodeBlock> BAMUM_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> KANA_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> ANCIENT_GREEK_MUSICAL_NOTATION;
            static const jxx::Ptr<UnicodeBlock> COUNTING_ROD_NUMERALS;
            static const jxx::Ptr<UnicodeBlock> MAHJONG_TILES;
            static const jxx::Ptr<UnicodeBlock> DOMINO_TILES;
            static const jxx::Ptr<UnicodeBlock> PLAYING_CARDS;
            static const jxx::Ptr<UnicodeBlock> ENCLOSED_ALPHANUMERIC_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> ENCLOSED_IDEOGRAPHIC_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS;
            static const jxx::Ptr<UnicodeBlock> EMOTICONS;
            static const jxx::Ptr<UnicodeBlock> TRANSPORT_AND_MAP_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> ALCHEMICAL_SYMBOLS;
            static const jxx::Ptr<UnicodeBlock> CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C;
            static const jxx::Ptr<UnicodeBlock> CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D;
            static const jxx::Ptr<UnicodeBlock> ARABIC_EXTENDED_A;
            static const jxx::Ptr<UnicodeBlock> SUNDANESE_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> MEETEI_MAYEK_EXTENSIONS;
            static const jxx::Ptr<UnicodeBlock> MEROITIC_HIEROGLYPHS;
            static const jxx::Ptr<UnicodeBlock> MEROITIC_CURSIVE;
            static const jxx::Ptr<UnicodeBlock> SORA_SOMPENG;
            static const jxx::Ptr<UnicodeBlock> CHAKMA;
            static const jxx::Ptr<UnicodeBlock> SHARADA;
            static const jxx::Ptr<UnicodeBlock> TAKRI;
            static const jxx::Ptr<UnicodeBlock> MIAO;
            static const jxx::Ptr<UnicodeBlock> ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS;

        private:
            UnicodeBlock(const jxx::Ptr<String>& name, jint start, jint end);
            jint start_;
            jint end_;
        };

        class UnicodeScript final : public Enum<UnicodeScript> {
        public:
            using JxxSuper = Enum<UnicodeScript>;

            static jxx::Ptr<ClassAny> Class();
            static jxx::Ptr<JxxArray<jxx::Ptr<UnicodeScript>, 1>> values();
            static jxx::Ptr<UnicodeScript> valueOf(const jxx::Ptr<String>& name);
            static jxx::Ptr<UnicodeScript> of(jint codePoint);
            static jxx::Ptr<UnicodeScript> forName(const jxx::Ptr<String>& scriptName);

            static const jxx::Ptr<UnicodeScript> COMMON;
            static const jxx::Ptr<UnicodeScript> LATIN;
            static const jxx::Ptr<UnicodeScript> GREEK;
            static const jxx::Ptr<UnicodeScript> CYRILLIC;
            static const jxx::Ptr<UnicodeScript> ARMENIAN;
            static const jxx::Ptr<UnicodeScript> HEBREW;
            static const jxx::Ptr<UnicodeScript> ARABIC;
            static const jxx::Ptr<UnicodeScript> SYRIAC;
            static const jxx::Ptr<UnicodeScript> THAANA;
            static const jxx::Ptr<UnicodeScript> DEVANAGARI;
            static const jxx::Ptr<UnicodeScript> BENGALI;
            static const jxx::Ptr<UnicodeScript> GURMUKHI;
            static const jxx::Ptr<UnicodeScript> GUJARATI;
            static const jxx::Ptr<UnicodeScript> ORIYA;
            static const jxx::Ptr<UnicodeScript> TAMIL;
            static const jxx::Ptr<UnicodeScript> TELUGU;
            static const jxx::Ptr<UnicodeScript> KANNADA;
            static const jxx::Ptr<UnicodeScript> MALAYALAM;
            static const jxx::Ptr<UnicodeScript> SINHALA;
            static const jxx::Ptr<UnicodeScript> THAI;
            static const jxx::Ptr<UnicodeScript> LAO;
            static const jxx::Ptr<UnicodeScript> TIBETAN;
            static const jxx::Ptr<UnicodeScript> MYANMAR;
            static const jxx::Ptr<UnicodeScript> GEORGIAN;
            static const jxx::Ptr<UnicodeScript> HANGUL;
            static const jxx::Ptr<UnicodeScript> ETHIOPIC;
            static const jxx::Ptr<UnicodeScript> CHEROKEE;
            static const jxx::Ptr<UnicodeScript> CANADIAN_ABORIGINAL;
            static const jxx::Ptr<UnicodeScript> OGHAM;
            static const jxx::Ptr<UnicodeScript> RUNIC;
            static const jxx::Ptr<UnicodeScript> KHMER;
            static const jxx::Ptr<UnicodeScript> MONGOLIAN;
            static const jxx::Ptr<UnicodeScript> HIRAGANA;
            static const jxx::Ptr<UnicodeScript> KATAKANA;
            static const jxx::Ptr<UnicodeScript> BOPOMOFO;
            static const jxx::Ptr<UnicodeScript> HAN;
            static const jxx::Ptr<UnicodeScript> YI;
            static const jxx::Ptr<UnicodeScript> OLD_ITALIC;
            static const jxx::Ptr<UnicodeScript> GOTHIC;
            static const jxx::Ptr<UnicodeScript> DESERET;
            static const jxx::Ptr<UnicodeScript> INHERITED;
            static const jxx::Ptr<UnicodeScript> TAGALOG;
            static const jxx::Ptr<UnicodeScript> HANUNOO;
            static const jxx::Ptr<UnicodeScript> BUHID;
            static const jxx::Ptr<UnicodeScript> TAGBANWA;
            static const jxx::Ptr<UnicodeScript> LIMBU;
            static const jxx::Ptr<UnicodeScript> TAI_LE;
            static const jxx::Ptr<UnicodeScript> LINEAR_B;
            static const jxx::Ptr<UnicodeScript> UGARITIC;
            static const jxx::Ptr<UnicodeScript> SHAVIAN;
            static const jxx::Ptr<UnicodeScript> OSMANYA;
            static const jxx::Ptr<UnicodeScript> CYPRIOT;
            static const jxx::Ptr<UnicodeScript> BRAILLE;
            static const jxx::Ptr<UnicodeScript> BUGINESE;
            static const jxx::Ptr<UnicodeScript> COPTIC;
            static const jxx::Ptr<UnicodeScript> NEW_TAI_LUE;
            static const jxx::Ptr<UnicodeScript> GLAGOLITIC;
            static const jxx::Ptr<UnicodeScript> TIFINAGH;
            static const jxx::Ptr<UnicodeScript> SYLOTI_NAGRI;
            static const jxx::Ptr<UnicodeScript> OLD_PERSIAN;
            static const jxx::Ptr<UnicodeScript> KHAROSHTHI;
            static const jxx::Ptr<UnicodeScript> BALINESE;
            static const jxx::Ptr<UnicodeScript> CUNEIFORM;
            static const jxx::Ptr<UnicodeScript> PHOENICIAN;
            static const jxx::Ptr<UnicodeScript> PHAGS_PA;
            static const jxx::Ptr<UnicodeScript> NKO;
            static const jxx::Ptr<UnicodeScript> SUNDANESE;
            static const jxx::Ptr<UnicodeScript> LEPCHA;
            static const jxx::Ptr<UnicodeScript> OL_CHIKI;
            static const jxx::Ptr<UnicodeScript> VAI;
            static const jxx::Ptr<UnicodeScript> SAURASHTRA;
            static const jxx::Ptr<UnicodeScript> KAYAH_LI;
            static const jxx::Ptr<UnicodeScript> REJANG;
            static const jxx::Ptr<UnicodeScript> LYCIAN;
            static const jxx::Ptr<UnicodeScript> CARIAN;
            static const jxx::Ptr<UnicodeScript> LYDIAN;
            static const jxx::Ptr<UnicodeScript> CHAM;
            static const jxx::Ptr<UnicodeScript> TAI_THAM;
            static const jxx::Ptr<UnicodeScript> TAI_VIET;
            static const jxx::Ptr<UnicodeScript> AVESTAN;
            static const jxx::Ptr<UnicodeScript> EGYPTIAN_HIEROGLYPHS;
            static const jxx::Ptr<UnicodeScript> SAMARITAN;
            static const jxx::Ptr<UnicodeScript> MANDAIC;
            static const jxx::Ptr<UnicodeScript> LISU;
            static const jxx::Ptr<UnicodeScript> BAMUM;
            static const jxx::Ptr<UnicodeScript> JAVANESE;
            static const jxx::Ptr<UnicodeScript> MEETEI_MAYEK;
            static const jxx::Ptr<UnicodeScript> IMPERIAL_ARAMAIC;
            static const jxx::Ptr<UnicodeScript> OLD_SOUTH_ARABIAN;
            static const jxx::Ptr<UnicodeScript> INSCRIPTIONAL_PARTHIAN;
            static const jxx::Ptr<UnicodeScript> INSCRIPTIONAL_PAHLAVI;
            static const jxx::Ptr<UnicodeScript> OLD_TURKIC;
            static const jxx::Ptr<UnicodeScript> BRAHMI;
            static const jxx::Ptr<UnicodeScript> KAITHI;
            static const jxx::Ptr<UnicodeScript> MEROITIC_HIEROGLYPHS;
            static const jxx::Ptr<UnicodeScript> MEROITIC_CURSIVE;
            static const jxx::Ptr<UnicodeScript> SORA_SOMPENG;
            static const jxx::Ptr<UnicodeScript> CHAKMA;
            static const jxx::Ptr<UnicodeScript> SHARADA;
            static const jxx::Ptr<UnicodeScript> TAKRI;
            static const jxx::Ptr<UnicodeScript> MIAO;
            static const jxx::Ptr<UnicodeScript> UNKNOWN;


        private:
            UnicodeScript(const jxx::Ptr<String>& name, jint ordinal);
        };
        static constexpr jchar MIN_VALUE = 0x0000;
        static constexpr jchar MAX_VALUE = 0xFFFF;

        static constexpr jint MIN_RADIX = 2;
        static constexpr jint MAX_RADIX = 36;

        static constexpr jint SIZE = 16;
        static constexpr jint BYTES = 2;

        static constexpr jint MIN_CODE_POINT = 0x000000;
        static constexpr jint MAX_CODE_POINT = 0x10FFFF;
        static constexpr jint MIN_SUPPLEMENTARY_CODE_POINT = 0x010000;

        static constexpr jchar MIN_HIGH_SURROGATE = 0xD800;
        static constexpr jchar MAX_HIGH_SURROGATE = 0xDBFF;
        static constexpr jchar MIN_LOW_SURROGATE = 0xDC00;
        static constexpr jchar MAX_LOW_SURROGATE = 0xDFFF;

        // Java Character type constants
        static constexpr jint UNASSIGNED = 0;
        static constexpr jint UPPERCASE_LETTER = 1;
        static constexpr jint LOWERCASE_LETTER = 2;
        static constexpr jint TITLECASE_LETTER = 3;
        static constexpr jint MODIFIER_LETTER = 4;
        static constexpr jint OTHER_LETTER = 5;
        static constexpr jint NON_SPACING_MARK = 6;
        static constexpr jint ENCLOSING_MARK = 7;
        static constexpr jint COMBINING_SPACING_MARK = 8;
        static constexpr jint DECIMAL_DIGIT_NUMBER = 9;
        static constexpr jint LETTER_NUMBER = 10;
        static constexpr jint OTHER_NUMBER = 11;
        static constexpr jint SPACE_SEPARATOR = 12;
        static constexpr jint LINE_SEPARATOR = 13;
        static constexpr jint PARAGRAPH_SEPARATOR = 14;
        static constexpr jint CONTROL = 15;
        static constexpr jint FORMAT = 16;
        static constexpr jint PRIVATE_USE = 18;
        static constexpr jint SURROGATE = 19;
        static constexpr jint DASH_PUNCTUATION = 20;
        static constexpr jint START_PUNCTUATION = 21;
        static constexpr jint END_PUNCTUATION = 22;
        static constexpr jint CONNECTOR_PUNCTUATION = 23;
        static constexpr jint OTHER_PUNCTUATION = 24;
        static constexpr jint MATH_SYMBOL = 25;
        static constexpr jint CURRENCY_SYMBOL = 26;
        static constexpr jint MODIFIER_SYMBOL = 27;
        static constexpr jint OTHER_SYMBOL = 28;
        static constexpr jint INITIAL_QUOTE_PUNCTUATION = 29;
        static constexpr jint FINAL_QUOTE_PUNCTUATION = 30;

    public:
        using JxxSuper = Object;
        using Super = ClassBase<Character, Object, Comparable<Character>, jxx::io::SerializableI>;
        using JxxClassInfoMarker = ClassInfo<Character, Object, Comparable<Character>, jxx::io::SerializableI>;

        static jxx::Ptr<ClassAny> Class();
        static const jxx::Ptr<ClassAny> TYPE;

        explicit Character(jchar value);

        jchar charValue() const noexcept;

        jint compareTo(const jxx::Ptr<Character>& anotherCharacter) const override;
        jbool equals(const jxx::Ptr<Object>& obj) const override;
        jint hashCode() const override;
        jxx::Ptr<String> toString() const override;

        static jxx::Ptr<Character> valueOf(jchar c);
        static jint hashCode(jchar value) noexcept;
        static jint compare(jchar x, jchar y) noexcept;
        static jxx::Ptr<String> toString(jchar value);
        static jchar reverseBytes(jchar value) noexcept;

    public:
        static jbool isValidCodePoint(jint codePoint) noexcept;
        static jbool isBmpCodePoint(jint codePoint) noexcept;
        static jbool isSupplementaryCodePoint(jint codePoint) noexcept;

        static jbool isHighSurrogate(jchar ch) noexcept;
        static jbool isLowSurrogate(jchar ch) noexcept;
        static jbool isSurrogate(jchar ch) noexcept;
        static jbool isSurrogatePair(jchar high, jchar low) noexcept;

        static jint charCount(jint codePoint) noexcept;

        static jint toCodePoint(jchar high, jchar low) noexcept;
        static jchar highSurrogate(jint codePoint) noexcept;
        static jchar lowSurrogate(jint codePoint) noexcept;

        static jbool isLowerCase(jint codePoint) noexcept;
        static jbool isUpperCase(jint codePoint) noexcept;
        static jbool isTitleCase(jint codePoint) noexcept;
        static jbool isDigit(jint codePoint) noexcept;
        static jbool isLetter(jint codePoint) noexcept;
        static jbool isLetterOrDigit(jint codePoint) noexcept;
        static jbool isWhitespace(jint codePoint) noexcept;
        static jbool isSpaceChar(jint codePoint) noexcept;
        static jbool isISOControl(jint codePoint) noexcept;
        static jbool isDefined(jint codePoint) noexcept;
        static jbool isMirrored(jint codePoint) noexcept;

        static jint getType(jint codePoint) noexcept;
        static jint getNumericValue(jint codePoint) noexcept;
        static jint digit(jint codePoint, jint radix) noexcept;

        static jint toLowerCase(jint codePoint) noexcept;
        static jint toUpperCase(jint codePoint) noexcept;
        static jint toTitleCase(jint codePoint) noexcept;

        static ::jxx::lang::jint codePointAt(
            const ::jxx::Ptr<::jxx::lang::CharSequence>& seq,
            ::jxx::lang::jint index);
        static ::jxx::lang::jint codePointAt(
            const ::jxx::lang::CharArray& a,
            ::jxx::lang::jint index);
        static ::jxx::lang::jint codePointAt(
            const ::jxx::lang::CharArray& a,
            ::jxx::lang::jint index,
            ::jxx::lang::jint limit);

        static ::jxx::lang::jint codePointBefore(
            const ::jxx::Ptr<::jxx::lang::CharSequence>& seq,
            ::jxx::lang::jint index);
        static ::jxx::lang::jint codePointBefore(
            const ::jxx::lang::CharArray& a,
            ::jxx::lang::jint index);
        static ::jxx::lang::jint codePointBefore(
            const ::jxx::lang::CharArray& a,
            ::jxx::lang::jint index,
            ::jxx::lang::jint start);

        static ::jxx::lang::jint codePointCount(
            const ::jxx::Ptr<::jxx::lang::CharSequence>& seq,
            ::jxx::lang::jint beginIndex,
            ::jxx::lang::jint endIndex);
        static ::jxx::lang::jint codePointCount(
            const ::jxx::lang::CharArray& a,
            ::jxx::lang::jint offset,
            ::jxx::lang::jint count);

        static ::jxx::lang::jint offsetByCodePoints(
            const ::jxx::Ptr<::jxx::lang::CharSequence>& seq,
            ::jxx::lang::jint index,
            ::jxx::lang::jint codePointOffset);
        static ::jxx::lang::jint offsetByCodePoints(
            const ::jxx::lang::CharArray& a,
            ::jxx::lang::jint start,
            ::jxx::lang::jint count,
            ::jxx::lang::jint index,
            ::jxx::lang::jint codePointOffset);

        static ::jxx::lang::CharArray toChars(::jxx::lang::jint codePoint);
        static ::jxx::lang::jint toChars(
            ::jxx::lang::jint codePoint,
            const ::jxx::lang::CharArray& dst,
            ::jxx::lang::jint dstIndex);

        void writeObject(
            const jxx::Ptr<jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const jxx::Ptr<jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;



    private:
        jchar value_;
    };
}