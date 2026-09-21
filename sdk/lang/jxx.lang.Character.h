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
            static jxx::Ptr<UnicodeBlock> of(jint codePoint);
            static jxx::Ptr<UnicodeBlock> forName(const jxx::Ptr<String>& blockName);

            static const jxx::Ptr<UnicodeBlock> BASIC_LATIN;
            static const jxx::Ptr<UnicodeBlock> LATIN_1_SUPPLEMENT;
            static const jxx::Ptr<UnicodeBlock> GREEK;
            static const jxx::Ptr<UnicodeBlock> CYRILLIC;
            static const jxx::Ptr<UnicodeBlock> HEBREW;
            static const jxx::Ptr<UnicodeBlock> ARABIC;
            static const jxx::Ptr<UnicodeBlock> GENERAL_PUNCTUATION;

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

        static CharArray toChars(jint codePoint);

        void writeObject(
            const jxx::Ptr<jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const jxx::Ptr<jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;



    private:
        jchar value_;
    };
}