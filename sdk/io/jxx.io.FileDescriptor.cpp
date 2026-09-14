#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.SyncFailedException.h"
namespace jxx::io {
::jxx::Ptr<FileDescriptor> FileDescriptor::in=::jxx::NEW<FileDescriptor>(stdin,false); ::jxx::Ptr<FileDescriptor> FileDescriptor::out=::jxx::NEW<FileDescriptor>(stdout,false); ::jxx::Ptr<FileDescriptor> FileDescriptor::err=::jxx::NEW<FileDescriptor>(stderr,false);
FileDescriptor::FileDescriptor()=default; FileDescriptor::FileDescriptor(std::FILE* h,::jxx::lang::jbool o):handle_(h),owned_(o){} FileDescriptor::~FileDescriptor(){if(owned_&&handle_)std::fclose(handle_);} ::jxx::lang::jbool FileDescriptor::valid()const{
		return synchronized([&]() -> ::jxx::lang::jbool {return handle_!=nullptr;
		});
	} void FileDescriptor::sync(){
		synchronized([&] {if(!handle_||std::fflush(handle_)!=0)throw SyncFailedException();
		});
	} std::FILE* FileDescriptor::nativeHandle()const{
		return synchronized([&]() -> std::FILE* {return handle_;
		});
	}
}
