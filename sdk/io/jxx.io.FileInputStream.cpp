#include "io/jxx.io.FileInputStream.h"
#include <algorithm>
#include <cerrno>
#include "io/jxx.io.File.h"
#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.String.h"
namespace jxx::io { FileInputStream::FileInputStream(const ::jxx::Ptr<::jxx::lang::String>& n):FileInputStream(::jxx::NEW<File>(n)){} FileInputStream::FileInputStream(const ::jxx::Ptr<File>& f){handle_=std::fopen(f->getPath()->utf8().c_str(),"rb");if(!handle_)throw FileNotFoundException(f->getPath());owned_=true;descriptor_=::jxx::NEW<FileDescriptor>(handle_,false);} FileInputStream::FileInputStream(const ::jxx::Ptr<FileDescriptor>& d):handle_(d?d->nativeHandle():nullptr),descriptor_(d){} FileInputStream::~FileInputStream(){close();} ::jxx::lang::jint FileInputStream::read(){auto c=std::fgetc(handle_);return c==EOF?-1:c;} ::jxx::lang::jint FileInputStream::read(const ::jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint l){IOHelper::checkBounds(b,o,l);if(l==0)return 0;auto n=std::fread(&(*b)[o],1,static_cast<std::size_t>(l),handle_);return n==0?-1:static_cast<::jxx::lang::jint>(n);} ::jxx::lang::jlong FileInputStream::skip(::jxx::lang::jlong n){auto p=std::ftell(handle_);if(p<0||std::fseek(handle_,static_cast<long>(n),SEEK_CUR)!=0)return 0;return std::ftell(handle_)-p;} ::jxx::lang::jint FileInputStream::available(){auto p=std::ftell(handle_);if(p<0)return 0;std::fseek(handle_,0,SEEK_END);auto e=std::ftell(handle_);std::fseek(handle_,p,SEEK_SET);return static_cast<::jxx::lang::jint>(std::max<long>(0,e-p));} void FileInputStream::close(){if(owned_&&handle_)std::fclose(handle_);handle_=nullptr;owned_=false;} ::jxx::Ptr<FileDescriptor> FileInputStream::getFD()const{return descriptor_;} }
