#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Comparable.h"
#include "io/jxx.io.Serializable.h"
namespace jxx::lang { class String; }
namespace jxx::io {
class File final : public ::jxx::lang::ClassBase<File,::jxx::lang::Object,::jxx::lang::Comparable<File>,Serializable> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<File,JxxSuper,::jxx::lang::Comparable<File>,Serializable>;
 static const ::jxx::lang::jchar separatorChar; static const ::jxx::lang::jchar pathSeparatorChar; static ::jxx::Ptr<::jxx::lang::String> separator; static ::jxx::Ptr<::jxx::lang::String> pathSeparator;
 explicit File(const ::jxx::Ptr<::jxx::lang::String>& pathname); File(const ::jxx::Ptr<::jxx::lang::String>& parent,const ::jxx::Ptr<::jxx::lang::String>& child); File(const ::jxx::Ptr<File>& parent,const ::jxx::Ptr<::jxx::lang::String>& child);
 ::jxx::Ptr<::jxx::lang::String> getName()const; ::jxx::Ptr<::jxx::lang::String> getParent()const; ::jxx::Ptr<File> getParentFile()const; ::jxx::Ptr<::jxx::lang::String> getPath()const; ::jxx::lang::jbool isAbsolute()const; ::jxx::Ptr<::jxx::lang::String> getAbsolutePath()const; ::jxx::Ptr<File> getAbsoluteFile()const; ::jxx::Ptr<::jxx::lang::String> getCanonicalPath()const; ::jxx::Ptr<File> getCanonicalFile()const;
 ::jxx::lang::jbool canRead()const; ::jxx::lang::jbool canWrite()const; ::jxx::lang::jbool canExecute()const; ::jxx::lang::jbool exists()const; ::jxx::lang::jbool isDirectory()const; ::jxx::lang::jbool isFile()const; ::jxx::lang::jbool isHidden()const; ::jxx::lang::jlong lastModified()const; ::jxx::lang::jlong length()const;
 ::jxx::lang::jbool createNewFile(); ::jxx::lang::jbool delete_(); void deleteOnExit(); ::jxx::lang::jbool mkdir(); ::jxx::lang::jbool mkdirs(); ::jxx::lang::jbool renameTo(const ::jxx::Ptr<File>& destination); ::jxx::lang::jbool setLastModified(::jxx::lang::jlong time); ::jxx::lang::jbool setReadOnly(); ::jxx::lang::jbool setWritable(::jxx::lang::jbool writable,::jxx::lang::jbool ownerOnly=true); ::jxx::lang::jbool setReadable(::jxx::lang::jbool readable,::jxx::lang::jbool ownerOnly=true); ::jxx::lang::jbool setExecutable(::jxx::lang::jbool executable,::jxx::lang::jbool ownerOnly=true); ::jxx::lang::jlong getTotalSpace()const; ::jxx::lang::jlong getFreeSpace()const; ::jxx::lang::jlong getUsableSpace()const;
 ::jxx::lang::jint compareTo(const ::jxx::Ptr<File>& other)const override; ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& other)const override; ::jxx::lang::jint hashCode()const override; ::jxx::Ptr<::jxx::lang::String> toString()const override;
private: ::jxx::Ptr<::jxx::lang::String> path_;
};
}
