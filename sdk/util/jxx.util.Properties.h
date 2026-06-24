#pragma once

#include <vector>
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.Hashtable.h"
#include "util/jxx.util.Set.h"

namespace jxx {
namespace lang { class String; }

namespace io { class PrintStream; class PrintWriter; class Reader; class InputStream; class OutputStream; class Writer; }
namespace util {

class Properties : public virtual Hashtable<jxx::lang::Object, jxx::lang::Object> {
protected:
    jxx::Ptr<Properties> defaults;

private:
    static jxx::lang::jbool containsObjectVector(const std::vector<jxx::Ptr<jxx::lang::Object>>& values, jxx::Ptr<jxx::lang::Object> value);
    static jxx::lang::jbool containsStringVector(const std::vector<jxx::Ptr<jxx::lang::String>>& values, jxx::Ptr<jxx::lang::String> value);
    void collectPropertyNames(std::vector<jxx::Ptr<jxx::lang::Object>>& out);
    void collectStringPropertyNames(std::vector<jxx::Ptr<jxx::lang::String>>& out);

public:
    Properties();
    explicit Properties(jxx::Ptr<Properties> defaults);
    virtual ~Properties() = default;

    virtual jxx::Ptr<jxx::lang::Object> setProperty(jxx::Ptr<jxx::lang::String> key, jxx::Ptr<jxx::lang::String> value);
    virtual jxx::Ptr<jxx::lang::String> getProperty(jxx::Ptr<jxx::lang::String> key);
    virtual jxx::Ptr<jxx::lang::String> getProperty(jxx::Ptr<jxx::lang::String> key, jxx::Ptr<jxx::lang::String> defaultValue);
    virtual jxx::Ptr<Enumeration<jxx::lang::Object>> propertyNames();
    virtual jxx::Ptr<Set<jxx::lang::String>> stringPropertyNames();

    virtual void list(jxx::Ptr<jxx::io::PrintStream> out);
    virtual void list(jxx::Ptr<jxx::io::PrintWriter> out);
    virtual void load(jxx::Ptr<jxx::io::Reader> reader);
    virtual void load(jxx::Ptr<jxx::io::InputStream> inStream);
    virtual void save(jxx::Ptr<jxx::io::OutputStream> out, jxx::Ptr<jxx::lang::String> comments);
    virtual void store(jxx::Ptr<jxx::io::Writer> writer, jxx::Ptr<jxx::lang::String> comments);
    virtual void store(jxx::Ptr<jxx::io::OutputStream> out, jxx::Ptr<jxx::lang::String> comments);
    virtual void loadFromXML(jxx::Ptr<jxx::io::InputStream> in);
    virtual void storeToXML(jxx::Ptr<jxx::io::OutputStream> os, jxx::Ptr<jxx::lang::String> comment);
    virtual void storeToXML(jxx::Ptr<jxx::io::OutputStream> os, jxx::Ptr<jxx::lang::String> comment, jxx::Ptr<jxx::lang::String> encoding);
};

} // namespace util
} // namespace jxx
