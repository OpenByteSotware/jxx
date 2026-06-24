#include <vector>
#include "lang/jxx.lang.String.h"
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.Writer.h"
#include "io/jxx.io.Reader.h"
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jxx.util.Properties.h"

namespace jxx {
namespace util {
namespace {

class ObjectEnumerationImpl : public virtual Enumeration<jxx::lang::Object> {
private:
    std::vector<jxx::Ptr<jxx::lang::Object>> snapshot_;
    std::size_t index_;
public:
    explicit ObjectEnumerationImpl(std::vector<jxx::Ptr<jxx::lang::Object>> snapshot)
        : snapshot_(std::move(snapshot)), index_(0) {}
    virtual ~ObjectEnumerationImpl() = default;
    virtual jxx::lang::jbool hasMoreElements() override {
        return static_cast<jxx::lang::jbool>(index_ < snapshot_.size());
    }
    virtual jxx::Ptr<jxx::lang::Object> nextElement() override {
        if (index_ >= snapshot_.size()) throw NoSuchElementException();
        return snapshot_[index_++];
    }
};

class StringIteratorImpl : public virtual Iterator<jxx::lang::String> {
private:
    std::vector<jxx::Ptr<jxx::lang::String>> snapshot_;
    std::size_t index_;
public:
    explicit StringIteratorImpl(std::vector<jxx::Ptr<jxx::lang::String>> snapshot)
        : snapshot_(std::move(snapshot)), index_(0) {}
    virtual ~StringIteratorImpl() = default;
    virtual jxx::lang::jbool hasNext() override {
        return static_cast<jxx::lang::jbool>(index_ < snapshot_.size());
    }
    virtual jxx::Ptr<jxx::lang::String> next() override {
        if (index_ >= snapshot_.size()) throw NoSuchElementException();
        return snapshot_[index_++];
    }
    virtual void remove() override {
        throw jxx::lang::UnsupportedOperationException();
    }
};

class StringSetImpl : public virtual Set<jxx::lang::String> {
private:
    std::vector<jxx::Ptr<jxx::lang::String>> values_;
public:
    explicit StringSetImpl(std::vector<jxx::Ptr<jxx::lang::String>> values)
        : values_(std::move(values)) {}
    virtual ~StringSetImpl() = default;
    virtual jxx::lang::jint size() override {
        return static_cast<jxx::lang::jint>(values_.size());
    }
    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
        for (const auto& value : values_) {
            if (o == nullptr) {
                if (value == nullptr) return static_cast<jxx::lang::jbool>(true);
            } else if (value != nullptr && o->equals(jxx::CAST<jxx::lang::Object>(value))) {
                return static_cast<jxx::lang::jbool>(true);
            }
        }
        return static_cast<jxx::lang::jbool>(false);
    }
    virtual jxx::Ptr<Iterator<jxx::lang::String>> iterator() override {
        return std::make_shared<StringIteratorImpl>(values_);
    }
    virtual jxx::lang::jbool add(jxx::Ptr<jxx::lang::String> /*e*/) override {
        throw jxx::lang::UnsupportedOperationException();
    }
    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> /*o*/) override {
        throw jxx::lang::UnsupportedOperationException();
    }
    virtual void clear() override {
        throw jxx::lang::UnsupportedOperationException();
    }
};

} // anonymous namespace

jxx::lang::jbool Properties::containsObjectVector(const std::vector<jxx::Ptr<jxx::lang::Object>>& values, jxx::Ptr<jxx::lang::Object> value) {
    for (const auto& existing : values) {
        if (value == nullptr) {
            if (existing == nullptr) return static_cast<jxx::lang::jbool>(true);
        } else if (existing != nullptr && value->equals(existing)) {
            return static_cast<jxx::lang::jbool>(true);
        }
    }
    return static_cast<jxx::lang::jbool>(false);
}

jxx::lang::jbool Properties::containsStringVector(const std::vector<jxx::Ptr<jxx::lang::String>>& values, jxx::Ptr<jxx::lang::String> value) {
    for (const auto& existing : values) {
        if (value == nullptr) {
            if (existing == nullptr) return static_cast<jxx::lang::jbool>(true);
        } else if (existing != nullptr && value->equals(jxx::CAST<jxx::lang::Object>(existing))) {
            return static_cast<jxx::lang::jbool>(true);
        }
    }
    return static_cast<jxx::lang::jbool>(false);
}

void Properties::collectPropertyNames(std::vector<jxx::Ptr<jxx::lang::Object>>& out) {
    if (defaults != nullptr) defaults->collectPropertyNames(out);
    auto e = this->keys();
    while (e->hasMoreElements()) {
        auto key = jxx::CAST<jxx::lang::Object>(e->nextElement());
        if (!containsObjectVector(out, key)) {
            out.push_back(key);
        }
    }
}

void Properties::collectStringPropertyNames(std::vector<jxx::Ptr<jxx::lang::String>>& out) {
    if (defaults != nullptr) defaults->collectStringPropertyNames(out);
    auto e = this->keys();
    while (e->hasMoreElements()) {
        auto keyObj = jxx::CAST<jxx::lang::Object>(e->nextElement());
        auto valueObj = jxx::CAST<jxx::lang::Object>(this->get(keyObj));
        auto keyStr = jxx::CAST<jxx::lang::String>(keyObj);
        auto valueStr = jxx::CAST<jxx::lang::String>(valueObj);
        if (keyStr != nullptr && valueStr != nullptr && !containsStringVector(out, keyStr)) {
            out.push_back(keyStr);
        }
    }
}

Properties::Properties()
    : Hashtable<jxx::lang::Object, jxx::lang::Object>()
    , defaults(nullptr) {
}

Properties::Properties(jxx::Ptr<Properties> defaults)
    : Hashtable<jxx::lang::Object, jxx::lang::Object>()
    , defaults(defaults) {
}

jxx::Ptr<jxx::lang::Object> Properties::setProperty(jxx::Ptr<jxx::lang::String> key, jxx::Ptr<jxx::lang::String> value) {
    if (key == nullptr || value == nullptr) throw NullPointerException();
    return this->put(jxx::CAST<jxx::lang::Object>(key), jxx::CAST<jxx::lang::Object>(value));
}

jxx::Ptr<jxx::lang::String> Properties::getProperty(jxx::Ptr<jxx::lang::String> key) {
    if (key == nullptr) throw NullPointerException();
    auto valueObj = jxx::CAST<jxx::lang::Object>(this->get(jxx::CAST<jxx::lang::Object>(key)));
    auto valueStr = jxx::CAST<jxx::lang::String>(valueObj);
    if (valueStr != nullptr) return valueStr;
    return defaults == nullptr ? nullptr : defaults->getProperty(key);
}

jxx::Ptr<jxx::lang::String> Properties::getProperty(jxx::Ptr<jxx::lang::String> key, jxx::Ptr<jxx::lang::String> defaultValue) {
    auto value = getProperty(key);
    return value == nullptr ? defaultValue : value;
}

jxx::Ptr<Enumeration<jxx::lang::Object>> Properties::propertyNames() {
    std::vector<jxx::Ptr<jxx::lang::Object>> names;
    collectPropertyNames(names);
    return std::make_shared<ObjectEnumerationImpl>(std::move(names));
}

jxx::Ptr<Set<jxx::lang::String>> Properties::stringPropertyNames() {
    std::vector<jxx::Ptr<jxx::lang::String>> names;
    collectStringPropertyNames(names);
    return std::make_shared<StringSetImpl>(std::move(names));
}

void Properties::list(jxx::Ptr<jxx::io::PrintStream> out) {
    if (out == nullptr) throw NullPointerException();
    throw UnsupportedOperationException();
}

void Properties::list(jxx::Ptr<jxx::io::PrintWriter> out) {
    if (out == nullptr) throw NullPointerException();
    throw UnsupportedOperationException();
}

void Properties::load(jxx::Ptr<jxx::io::Reader> reader) {
    if (reader == nullptr) throw NullPointerException();
    throw UnsupportedOperationException();
}

void Properties::load(jxx::Ptr<jxx::io::InputStream> inStream) {
    if (inStream == nullptr) throw NullPointerException();
    throw UnsupportedOperationException();
}

void Properties::save(jxx::Ptr<jxx::io::OutputStream> out, jxx::Ptr<jxx::lang::String> comments) {
    if (out == nullptr) throw NullPointerException();
    (void)comments;
    throw UnsupportedOperationException();
}

void Properties::store(jxx::Ptr<jxx::io::Writer> writer, jxx::Ptr<jxx::lang::String> comments) {
    if (writer == nullptr) throw NullPointerException();
    (void)comments;
    throw UnsupportedOperationException();
}

void Properties::store(jxx::Ptr<jxx::io::OutputStream> out, jxx::Ptr<jxx::lang::String> comments) {
    if (out == nullptr) throw NullPointerException();
    (void)comments;
    throw UnsupportedOperationException();
}

void Properties::loadFromXML(jxx::Ptr<jxx::io::InputStream> in) {
    if (in == nullptr) throw NullPointerException();
    throw UnsupportedOperationException();
}

void Properties::storeToXML(jxx::Ptr<jxx::io::OutputStream> os, jxx::Ptr<jxx::lang::String> comment) {
    if (os == nullptr) throw NullPointerException();
    (void)comment;
    throw UnsupportedOperationException();
}

void Properties::storeToXML(jxx::Ptr<jxx::io::OutputStream> os, jxx::Ptr<jxx::lang::String> comment, jxx::Ptr<jxx::lang::String> encoding) {
    if (os == nullptr) throw NullPointerException();
    (void)comment;
    (void)encoding;
    throw UnsupportedOperationException();
}

} // namespace util
} // namespace jxx
