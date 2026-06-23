#pragma once

#include <vector>
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.Hashtable.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Set.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "io/jxx.io.Reader.h"
#include "io/jxx.io.Writer.h"
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.PrintWriter.h"

namespace jxx { namespace lang { class String; } }

namespace jxx {
namespace util {

class Properties : public virtual Hashtable<jxx::lang::Object, jxx::lang::Object> {
protected:
    jxx::Ptr<Properties> defaults;

private:
    template <typename E>
    class SnapshotIterator : public virtual Iterator<E> {
    private:
        std::vector<jxx::Ptr<E>> snapshot_;
        std::size_t index_;
    public:
        explicit SnapshotIterator(std::vector<jxx::Ptr<E>> snapshot)
            : snapshot_(std::move(snapshot)), index_(0) {}
        virtual ~SnapshotIterator() = default;
        virtual jxx::lang::jbool hasNext() override { return static_cast<jxx::lang::jbool>(index_ < snapshot_.size()); }
        virtual jxx::Ptr<E> next() override {
            if (index_ >= snapshot_.size()) throw NoSuchElementException();
            return snapshot_[index_++];
        }
    };

    template <typename E>
    class SnapshotEnumeration : public virtual Enumeration<E> {
    private:
        std::vector<jxx::Ptr<E>> snapshot_;
        std::size_t index_;
    public:
        explicit SnapshotEnumeration(std::vector<jxx::Ptr<E>> snapshot)
            : snapshot_(std::move(snapshot)), index_(0) {}
        virtual ~SnapshotEnumeration() = default;
        virtual jxx::lang::jbool hasMoreElements() override { return static_cast<jxx::lang::jbool>(index_ < snapshot_.size()); }
        virtual jxx::Ptr<E> nextElement() override {
            if (index_ >= snapshot_.size()) throw NoSuchElementException();
            return snapshot_[index_++];
        }
    };

    class SnapshotStringSet : public virtual Set<jxx::lang::String> {
    private:
        std::vector<jxx::Ptr<jxx::lang::String>> values_;
    public:
        explicit SnapshotStringSet(std::vector<jxx::Ptr<jxx::lang::String>> values)
            : values_(std::move(values)) {}
        virtual ~SnapshotStringSet() = default;
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
            return jxx::Ptr<Iterator<jxx::lang::String>>(jxx::NEW<SnapshotIterator<jxx::lang::String>>(values_));
        }
        virtual jxx::lang::jbool add(jxx::Ptr<jxx::lang::String> /*e*/) override { throw jxx::lang::UnsupportedOperationException(); }
        virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> /*o*/) override { throw jxx::lang::UnsupportedOperationException(); }
        virtual void clear() override { throw jxx::lang::UnsupportedOperationException(); }
    };

        
public:
    Properties();
    
    explicit Properties(jxx::Ptr<Properties> defaults);
  
    virtual ~Properties() = default;

    virtual jxx::Ptr<jxx::lang::Object> setProperty(jxx::Ptr<jxx::lang::String> key, jxx::Ptr<jxx::lang::String> value);
    virtual jxx::Ptr<jxx::lang::String> getProperty(jxx::Ptr<jxx::lang::String> key);
    virtual jxx::Ptr<jxx::lang::String> getProperty(jxx::Ptr<jxx::lang::String> key,
        jxx::Ptr<jxx::lang::String> defaultValue);
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
    virtual void storeToXML(jxx::Ptr<jxx::io::OutputStream> os, jxx::Ptr<jxx::lang::String> comment,
        jxx::Ptr<jxx::lang::String> encoding);
    
    void collectPropertyNames(std::vector<jxx::Ptr<jxx::lang::Object>>& out);

    void collectStringPropertyNames(std::vector<jxx::Ptr<jxx::lang::String>>& out);

    static inline jxx::lang::jbool containsObjectVector(const std::vector<jxx::Ptr<jxx::lang::Object>>& values,
        jxx::Ptr<jxx::lang::Object> value);

    static inline jxx::lang::jbool containsStringVector(const std::vector<jxx::Ptr<jxx::lang::String>>& values,
        jxx::Ptr<jxx::lang::String> value);
};

} // namespace util
} // namespace jxx
