#pragma once
#include "jxx_types.h"
#include "jxx.lang.Object.h"


namespace jxx::lang {
	class String;

class Boolean : public Object {
private:
    bool value;


public:
    // private constructor for static instances
    explicit Boolean(jbool v);
    virtual ~Boolean() override = default;

    // Static cached instances
    static jxx::Ptr<Boolean> TRUE;
    static jxx::Ptr<Boolean> FALSE;

    // Constructors
    static jxx::Ptr<Boolean> valueOf(jbool b);
    static jxx::Ptr<Boolean> valueOf(const jxx::Ptr<String> s);

    // Parsing
    static jbool parseBoolean(const jxx::Ptr<String> s);

    // Instance methods
    jbool booleanValue() const;

    jxx::Ptr<String> toString() const override;
    static jxx::Ptr<String> toString(jbool b);

    jint hashCode() const override;
    static jint hashCode(jbool value);

    jbool equals(const jxx::Ptr<Object> obj) const override;

    jint compareTo(const jxx::Ptr<Boolean> b) const;
    static jint compare(jbool x, jbool y);
};

}