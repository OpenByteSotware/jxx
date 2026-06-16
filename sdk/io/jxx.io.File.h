#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.buildin_array.h"
#include "io/jxx.io.Serializable.h"

namespace jxx::lang {
    class String;
}
namespace jxx::io
{
	class ObjectOutputStream;
	class ObjectInputStream;
    /**
     * Practical Java-8-style subset of java.io.File suitable for the stream
     * family and common runtime use.
     *
     * Note: the Java method delete() is spelled delete_() here because delete
     * is a C++ keyword.
     */
    class File final
        : public jxx::lang::Object
        , public jxx::lang::Comparable<File>
        , public jxx::io::Serializable
    {
    public:
        static constexpr char separatorChar = '/';
        static constexpr char pathSeparatorChar = ':';

    public:
        explicit File(jxx::Ptr<jxx::lang::String> pathname);
        File(jxx::Ptr<jxx::lang::String> parent,
             jxx::Ptr<jxx::lang::String> child);
        File(jxx::Ptr<File> parent,
             jxx::Ptr<jxx::lang::String> child);

        ~File() override = default;

    public:
        jxx::Ptr<jxx::lang::String> getName() const;
        jxx::Ptr<jxx::lang::String> getParent() const;
        jxx::Ptr<File> getParentFile() const;

        jxx::Ptr<jxx::lang::String> getPath() const;
        jxx::lang::jbool isAbsolute() const;
        jxx::Ptr<jxx::lang::String> getAbsolutePath() const;
        jxx::Ptr<File> getAbsoluteFile() const;
        jxx::Ptr<jxx::lang::String> getCanonicalPath() const;
        jxx::Ptr<File> getCanonicalFile() const;

        jxx::lang::jbool canRead() const;
        jxx::lang::jbool canWrite() const;
        jxx::lang::jbool canExecute() const;

        jxx::lang::jbool exists() const;
        jxx::lang::jbool isDirectory() const;
        jxx::lang::jbool isFile() const;
        jxx::lang::jbool isHidden() const;

        jxx::lang::jlong lastModified() const;
        jxx::lang::jlong length() const;

        jxx::lang::jbool createNewFile();
        jxx::lang::jbool delete_();
        void deleteOnExit();

        jxx::Ptr<jxx::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>> list() const;
        jxx::Ptr<jxx::JxxArray<jxx::Ptr<File>, 1U>> listFiles() const;

        jxx::lang::jbool mkdir();
        jxx::lang::jbool mkdirs();

        jxx::lang::jbool renameTo(jxx::Ptr<File> dest);

        jxx::lang::jbool setLastModified(jxx::lang::jlong time);
        jxx::lang::jbool setReadOnly();
        jxx::lang::jbool setWritable(jxx::lang::jbool writable);
        jxx::lang::jbool setReadable(jxx::lang::jbool readable);
        jxx::lang::jbool setExecutable(jxx::lang::jbool executable);

        jxx::lang::jint compareTo(const jxx::Ptr<File> pathname) const override;
        jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;
        jxx::Ptr<jxx::lang::String> toString() const override;

        static jxx::Ptr<File> createTempFile(jxx::Ptr<jxx::lang::String> prefix,
                                             jxx::Ptr<jxx::lang::String> suffix);
        static jxx::Ptr<File> createTempFile(jxx::Ptr<jxx::lang::String> prefix,
                                             jxx::Ptr<jxx::lang::String> suffix,
                                             jxx::Ptr<File> directory);

        static jxx::Ptr<jxx::JxxArray<jxx::Ptr<File>, 1U>> listRoots();

        virtual void writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out) override;
        virtual void readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) override;
        virtual void readObjectNoData() override;

    private:
        std::string path_;
    };
}
