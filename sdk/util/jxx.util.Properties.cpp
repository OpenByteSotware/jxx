#include <vector>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.InputStream.h"
#include "util/jxx.util.Properties.h"
#include "io/jxx.io.PrintWriter.h"

namespace jxx::util {

template <typename E>
class SnapshotIteratorImpl : public virtual Iterator<E> {
private:
    std::vector<jxx::Ptr<E>> snapshot_;
    std::size_t index_;
public:
    explicit SnapshotIteratorImpl(std::vector<jxx::Ptr<E>> snapshot)
        : snapshot_(std::move(snapshot)), index_(0) {}
    virtual ~SnapshotIteratorImpl() = default;
    virtual jxx::lang::jbool hasNext() override { return static_cast<jxx::lang::jbool>(index_ < snapshot_.size()); }
    virtual jxx::Ptr<E> next() override {
        if (index_ >= snapshot_.size()) throw jxx::util::NoSuchElementException();
        return snapshot_[index_++];
    }
};

template <typename E>
class SnapshotEnumerationImpl : public virtual Enumeration<E> {
private:
    std::vector<jxx::Ptr<E>> snapshot_;
    std::size_t index_;
public:
    explicit SnapshotEnumerationImpl(std::vector<jxx::Ptr<E>> snapshot)
        : snapshot_(std::move(snapshot)), index_(0) {}
    virtual ~SnapshotEnumerationImpl() = default;
    virtual jxx::lang::jbool hasMoreElements() override { return static_cast<jxx::lang::jbool>(index_ < snapshot_.size()); }
    virtual jxx::Ptr<E> nextElement() override {
        if (index_ >= snapshot_.size()) throw NoSuchElementException();
        return snapshot_[index_++];
    }
};

class SnapshotStringSetImpl : public virtual Set<jxx::lang::String> {
private:
    std::vector<jxx::Ptr<jxx::lang::String>> values_;
public:
    explicit SnapshotStringSetImpl(std::vector<jxx::Ptr<jxx::lang::String>> values)
        : values_(std::move(values)) {}
    virtual ~SnapshotStringSetImpl() = default;
    virtual jxx::lang::jint size() override { return static_cast<jxx::lang::jint>(values_.size()); }
    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
        for (const auto& v : values_) {
            if (o == nullptr) {
                if (v == nullptr) return static_cast<jxx::lang::jbool>(true);
            } else if (v != nullptr && o->equals(jxx::CAST<jxx::lang::Object, jxx::lang::String>(v))) {
                return static_cast<jxx::lang::jbool>(true);
            }
        }
        return static_cast<jxx::lang::jbool>(false);
    }
    virtual jxx::Ptr<Iterator<jxx::lang::String>> iterator() override {
        return jxx::Ptr<Iterator<jxx::lang::String>>(jxx::NEW<SnapshotIteratorImpl<jxx::lang::String>>(values_));
    }
    virtual jxx::lang::jbool add(jxx::Ptr<jxx::lang::String> /*e*/) override { throw jxx::lang::UnsupportedOperationException(); }
    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> /*o*/) override { throw jxx::lang::UnsupportedOperationException(); }
    virtual void clear() override { throw jxx::lang::UnsupportedOperationException(); }
};

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
        } else if (existing != nullptr && value->equals(jxx::CAST<jxx::lang::Object, jxx::lang::String>(existing))) {
            return static_cast<jxx::lang::jbool>(true);
        }
    }
    return static_cast<jxx::lang::jbool>(false);
}

void Properties::collectPropertyNames(std::vector<jxx::Ptr<jxx::lang::Object>>& out) {
    if (defaults != nullptr) defaults->collectPropertyNames(out);
    auto e = this->keys();
    while (e->hasMoreElements()) {
        auto key = jxx::CAST<jxx::lang::Object, jxx::lang::Object>(e->nextElement());
        if (!containsObjectVector(out, key)) out.push_back(key);
    }
}

void Properties::collectStringPropertyNames(std::vector<jxx::Ptr<jxx::lang::String>>& out) {
    if (defaults != nullptr) defaults->collectStringPropertyNames(out);
    auto e = this->keys();
    while (e->hasMoreElements()) {
        auto keyObj = jxx::CAST<jxx::lang::Object, jxx::lang::Object>(e->nextElement());
        auto valObj = jxx::CAST<jxx::lang::Object, jxx::lang::Object>(this->get(keyObj));
        auto keyStr = jxx::CAST<jxx::lang::String, jxx::lang::Object>(keyObj);
        auto valStr = jxx::CAST<jxx::lang::String, jxx::lang::Object>(valObj);
        if (keyStr != nullptr && valStr != nullptr && !containsStringVector(out, keyStr)) {
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
    if (key == nullptr || value == nullptr) throw jxx::lang::NullPointerException();
    return this->put(jxx::CAST<jxx::lang::Object, jxx::lang::String>(key), 
        jxx::CAST<jxx::lang::Object, jxx::lang::String>(value));
}

jxx::Ptr<jxx::lang::String> Properties::getProperty(jxx::Ptr<jxx::lang::String> key) {
    if (key == nullptr) throw jxx::lang::NullPointerException();
    auto value = this->get(jxx::CAST<jxx::lang::Object, jxx::lang::String>(key));
    auto asString = jxx::CAST<jxx::lang::String, jxx::lang::Object>(value);
    if (asString != nullptr) return asString;
    return defaults == nullptr ? nullptr : defaults->getProperty(key);
}

jxx::Ptr<jxx::lang::String> Properties::getProperty(jxx::Ptr<jxx::lang::String> key, jxx::Ptr<jxx::lang::String> defaultValue) {
    auto value = getProperty(key);
    return value == nullptr ? defaultValue : value;
}

jxx::Ptr<Enumeration<jxx::lang::Object>> Properties::propertyNames() {
    std::vector<jxx::Ptr<jxx::lang::Object>> names;
    collectPropertyNames(names);
    return jxx::Ptr<Enumeration<jxx::lang::Object>>(new SnapshotEnumerationImpl<jxx::lang::Object>(std::move(names)));
}

jxx::Ptr<Set<jxx::lang::String>> Properties::stringPropertyNames() {
    std::vector<jxx::Ptr<jxx::lang::String>> names;
    collectStringPropertyNames(names);
    return jxx::Ptr<Set<jxx::lang::String>>(jxx::NEW<SnapshotStringSetImpl>(std::move(names)));
}

void Properties::list(jxx::Ptr<jxx::io::PrintStream> out) {
    if (out == nullptr) throw jxx::lang::NullPointerException();
    throw jxx::lang::UnsupportedOperationException();
}

void Properties::list(jxx::Ptr<jxx::io::PrintWriter> out) {
    if (out == nullptr) throw jxx::lang::NullPointerException();
    throw jxx::lang::UnsupportedOperationException();
}

void Properties::load(jxx::Ptr<jxx::io::Reader> reader) {
    if (reader == nullptr) throw jxx::lang::NullPointerException();
    throw jxx::lang::UnsupportedOperationException();
}

void Properties::load(jxx::Ptr<jxx::io::InputStream> inStream) {
    if (inStream == nullptr) throw jxx::lang::NullPointerException();
    throw jxx::lang::UnsupportedOperationException();
}

void Properties::save(jxx::Ptr<jxx::io::OutputStream> out, jxx::Ptr<jxx::lang::String> comments) {
    if (out == nullptr) throw jxx::lang::NullPointerException();
    (void)comments;
    throw jxx::lang::UnsupportedOperationException();
}

void Properties::store(jxx::Ptr<jxx::io::Writer> writer, jxx::Ptr<jxx::lang::String> comments) {
    if (writer == nullptr) throw jxx::lang::NullPointerException();
    (void)comments;
    throw jxx::lang::UnsupportedOperationException();
}

void Properties::store(jxx::Ptr<jxx::io::OutputStream> out, jxx::Ptr<jxx::lang::String> comments) {
    if (out == nullptr) throw jxx::lang::NullPointerException();
    (void)comments;
    throw jxx::lang::UnsupportedOperationException();
}

void Properties::loadFromXML(jxx::Ptr<jxx::io::InputStream> in) {
    if (in == nullptr) throw jxx::lang::NullPointerException();
    throw jxx::lang::UnsupportedOperationException();
}

void Properties::storeToXML(jxx::Ptr<jxx::io::OutputStream> os, jxx::Ptr<jxx::lang::String> comment) {
    if (os == nullptr) throw jxx::lang::NullPointerException();
    (void)comment;
    throw jxx::lang::UnsupportedOperationException();
}

void Properties::storeToXML(jxx::Ptr<jxx::io::OutputStream> os, jxx::Ptr<jxx::lang::String> comment, jxx::Ptr<jxx::lang::String> encoding) {
    if (os == nullptr) throw jxx::lang::NullPointerException();
    (void)comment;
    (void)encoding;
    throw jxx::lang::UnsupportedOperationException();
}

}