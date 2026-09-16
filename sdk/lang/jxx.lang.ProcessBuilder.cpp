#include "lang/jxx.lang.ProcessBuilder.h"

#include <algorithm>
#include <atomic>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <limits>
#include <system_error>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <csignal>
#include <fcntl.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
extern char** environ;
#endif

#include "io/jxx.io.File.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalThreadStateException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Process.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::lang {
namespace {
#ifdef _WIN32
using NativeHandle=HANDLE;
static constexpr NativeHandle invalidHandle=nullptr;

class WinHandle final {
public:
    WinHandle() = default;
    explicit WinHandle(HANDLE value) : value_(value) {}
    ~WinHandle() { reset(); }
    WinHandle(const WinHandle&) = delete;
    WinHandle& operator=(const WinHandle&) = delete;
    WinHandle(WinHandle&& other) noexcept : value_(other.release()) {}
    WinHandle& operator=(WinHandle&& other) noexcept {
        if (this != &other) reset(other.release());
        return *this;
    }
    HANDLE get() const noexcept { return value_; }
    HANDLE release() noexcept { HANDLE value=value_; value_=nullptr; return value; }
    void reset(HANDLE value=nullptr) noexcept {
        if (value_ != nullptr && value_ != INVALID_HANDLE_VALUE) CloseHandle(value_);
        value_=value;
    }
private:
    HANDLE value_ = nullptr;
};

[[noreturn]] void throwWindowsIOException(const char* operation, DWORD error=GetLastError()) {
    LPWSTR systemMessage=nullptr;
    const DWORD flags=FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS;
    FormatMessageW(flags,nullptr,error,0,reinterpret_cast<LPWSTR>(&systemMessage),0,nullptr);
    std::string message(operation);
    message += " failed (Windows error " + std::to_string(error) + ")";
    if(systemMessage!=nullptr){
        const int size=WideCharToMultiByte(CP_UTF8,0,systemMessage,-1,nullptr,0,nullptr,nullptr);
        if(size>1){std::string text(static_cast<std::size_t>(size-1),'\0');WideCharToMultiByte(CP_UTF8,0,systemMessage,-1,text.data(),size,nullptr,nullptr);message += ": " + text;}
        LocalFree(systemMessage);
    }
    throw jxx::io::IOException(jxx::NEW<String>(message));
}

std::wstring utf16(const std::string& value) {
    if (value.empty()) return {};
    const int size=MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,value.data(),static_cast<int>(value.size()),nullptr,0);
    if (size<=0) throw jxx::io::IOException();
    std::wstring result(static_cast<std::size_t>(size),L'\0');
    if (MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,value.data(),static_cast<int>(value.size()),result.data(),size)!=size) throw jxx::io::IOException();
    return result;
}

std::wstring quoteArgument(const std::wstring& value) {
    if (!value.empty() && value.find_first_of(L" \\t\\n\\v\\\"")==std::wstring::npos) return value;
    std::wstring result(1,L'"');
    std::size_t backslashes=0;
    for (wchar_t character : value) {
        if (character==L'\\') { ++backslashes; continue; }
        if (character==L'"') {
            result.append(backslashes*2+1,L'\\');
            result.push_back(L'"');
            backslashes=0;
            continue;
        }
        result.append(backslashes,L'\\');
        backslashes=0;
        result.push_back(character);
    }
    result.append(backslashes*2,L'\\');
    result.push_back(L'"');
    return result;
}

std::vector<wchar_t> makeEnvironmentBlock(const std::unordered_map<std::string,std::string>& environment) {
    std::vector<std::pair<std::wstring,std::wstring>> entries;
    entries.reserve(environment.size());
    for (const auto& item : environment) entries.emplace_back(utf16(item.first),utf16(item.second));
    std::sort(entries.begin(),entries.end(),[](const auto& left,const auto& right){return _wcsicmp(left.first.c_str(),right.first.c_str())<0;});
    std::vector<wchar_t> block;
    for (const auto& item : entries) {
        block.insert(block.end(),item.first.begin(),item.first.end());
        block.push_back(L'=');
        block.insert(block.end(),item.second.begin(),item.second.end());
        block.push_back(L'\0');
    }
    block.push_back(L'\0');
    if (entries.empty()) block.push_back(L'\0');
    return block;
}

void createPipePair(WinHandle& childEnd, WinHandle& parentEnd, bool childReads) {
    SECURITY_ATTRIBUTES attributes{sizeof(attributes),nullptr,TRUE};
    HANDLE readHandle=nullptr,writeHandle=nullptr;
    if (!CreatePipe(&readHandle,&writeHandle,&attributes,0)) throw jxx::io::IOException();
    WinHandle readOwner(readHandle),writeOwner(writeHandle);
    HANDLE parent=childReads?writeHandle:readHandle;
    if (!SetHandleInformation(parent,HANDLE_FLAG_INHERIT,0)) throw jxx::io::IOException();
    if (childReads) { childEnd.reset(readOwner.release()); parentEnd.reset(writeOwner.release()); }
    else { childEnd.reset(writeOwner.release()); parentEnd.reset(readOwner.release()); }
}

WinHandle openRedirectFile(const jxx::Ptr<ProcessBuilder::Redirect>& redirect, bool input) {
    SECURITY_ATTRIBUTES attributes{sizeof(attributes),nullptr,TRUE};
    DWORD access=input?GENERIC_READ:GENERIC_WRITE;
    DWORD creation=input?OPEN_EXISTING:(redirect->type()==ProcessBuilder::Redirect::Type::APPEND?OPEN_ALWAYS:CREATE_ALWAYS);
    HANDLE handle=CreateFileW(utf16(redirect->file()->getPath()->utf8()).c_str(),access,FILE_SHARE_READ|FILE_SHARE_WRITE,&attributes,creation,FILE_ATTRIBUTE_NORMAL,nullptr);
    if (handle==INVALID_HANDLE_VALUE) throw jxx::io::IOException();
    WinHandle result(handle);
    if (!input && redirect->type()==ProcessBuilder::Redirect::Type::APPEND && SetFilePointer(handle,0,nullptr,FILE_END)==INVALID_SET_FILE_POINTER && GetLastError()!=NO_ERROR) throw jxx::io::IOException();
    return result;
}
#else
using NativeHandle=int;
static constexpr NativeHandle invalidHandle=-1;
#endif

class NullInputStream final : public jxx::io::InputStream {
public:
    jint read() override { return -1; }
    jint read(const ByteArray& buffer, jint offset, jint length) override {
        if (buffer == nullptr) throw NullPointerException();
        if (offset < 0 || length < 0 || offset > static_cast<jint>(buffer->length) - length) throw IndexOutOfBoundsException();
        return length == 0 ? 0 : -1;
    }
    jint available() override { return 0; }
    void close() override {}
};

class NullOutputStream final : public jxx::io::OutputStream {
public:
    void write(jint) override { throw jxx::io::IOException(); }
    void write(const ByteArray&, jint, jint) override { throw jxx::io::IOException(); }
    void close() override {}
};

class PipeInput final : public jxx::io::InputStream {
public:
 explicit PipeInput(NativeHandle h):handle_(h){}
 ~PipeInput() override { close(); }
 jint read() override { unsigned char c; return readNative(&c,1)==1?c:-1; }
 jint read(const ByteArray& b,jint o,jint n) override {
  if(!b)throw NullPointerException(); if(o<0||n<0||o>static_cast<jint>(b->length)-n)throw IndexOutOfBoundsException();
  if(n==0)return 0; std::vector<unsigned char> x(static_cast<size_t>(n)); auto r=readNative(x.data(),n); if(r<=0)return -1;
  for(jint i=0;i<r;++i)(*b)[o+i]=static_cast<jbyte>(x[i]); return r;
 }
 jint available() override {
#ifdef _WIN32
  if(!handle_)throw jxx::io::IOException();DWORD n=0;if(!PeekNamedPipe(handle_,nullptr,0,nullptr,&n,nullptr)){const DWORD error=GetLastError();if(error==ERROR_BROKEN_PIPE)return 0;throwWindowsIOException("PeekNamedPipe",error);}return static_cast<jint>(n);
#else
  int n=0; return handle_>=0&&ioctl(handle_,FIONREAD,&n)==0?n:0;
#endif
 }
 void close() override {
#ifdef _WIN32
  if(handle_){CloseHandle(handle_);handle_=nullptr;}
#else
  if(handle_>=0){::close(handle_);handle_=-1;}
#endif
 }
private:
 jint readNative(void* p,jint n){
#ifdef _WIN32
  if (!handle_) throw jxx::io::IOException();
  DWORD r=0;
  if (!ReadFile(handle_,p,n,&r,nullptr)) {
      if (GetLastError() == ERROR_BROKEN_PIPE) return 0;
      throw jxx::io::IOException();
  }
  return static_cast<jint>(r);
#else
  ssize_t r;
  do { r=::read(handle_,p,static_cast<size_t>(n)); } while(r<0 && errno==EINTR);
  if(r<0) throw jxx::io::IOException(jxx::NEW<String>(std::strerror(errno)));
  return static_cast<jint>(r);
#endif
 }
 NativeHandle handle_;
};
class PipeOutput final : public jxx::io::OutputStream {
public:
 explicit PipeOutput(NativeHandle h):handle_(h){}
 ~PipeOutput() override { close(); }
 void write(jint v) override { unsigned char c=static_cast<unsigned char>(v); writeNative(&c,1); }
 void write(const ByteArray& b,jint o,jint n) override { if(!b)throw NullPointerException();if(o<0||n<0||o>static_cast<jint>(b->length)-n)throw IndexOutOfBoundsException();std::vector<unsigned char>x(n);for(jint i=0;i<n;++i)x[i]=static_cast<unsigned char>((*b)[o+i]);writeNative(x.data(),n); }
 void close() override {
#ifdef _WIN32
  if(handle_){CloseHandle(handle_);handle_=nullptr;}
#else
  if(handle_>=0){::close(handle_);handle_=-1;}
#endif
 }
private:
 void writeNative(const void*p,jint n){
#ifdef _WIN32
  if(!handle_)throw jxx::io::IOException();const auto* bytes=static_cast<const unsigned char*>(p);DWORD completed=0;while(completed<static_cast<DWORD>(n)){DWORD written=0;if(!WriteFile(handle_,bytes+completed,static_cast<DWORD>(n)-completed,&written,nullptr))throwWindowsIOException("WriteFile");if(written==0)throw jxx::io::IOException();completed+=written;}
#else
  const auto* q=static_cast<const unsigned char*>(p);jint done=0;while(done<n){ssize_t w;do{w=::write(handle_,q+done,static_cast<size_t>(n-done));}while(w<0&&errno==EINTR);if(w<=0)throw jxx::io::IOException(jxx::NEW<String>(std::strerror(errno)));done+=static_cast<jint>(w);}
#endif
 }
 NativeHandle handle_;
};

class NativeProcess final : public Process {
private:
    struct Completion final {
        std::mutex mutex;
        std::condition_variable condition;
        jbool done = false;
        jint exitCode = 0;
        jxx::Ptr<jxx::lang::Throwable> failure;
    };

public:
#ifdef _WIN32
    NativeProcess(
        HANDLE process,
        const jxx::Ptr<jxx::io::OutputStream>& input,
        const jxx::Ptr<jxx::io::InputStream>& output,
        const jxx::Ptr<jxx::io::InputStream>& error)
        : process_(process), input_(output), error_(error), output_(input),
          completion_(std::make_shared<Completion>()) {
        HANDLE waitHandle = nullptr;
        if (!DuplicateHandle(GetCurrentProcess(), process_, GetCurrentProcess(),
                &waitHandle, SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION,
                FALSE, 0)) {
            throwWindowsIOException("DuplicateHandle");
        }
        auto completion = completion_;
        std::thread([waitHandle, completion] {
            WinHandle owner(waitHandle);
            const DWORD wait = WaitForSingleObject(waitHandle, INFINITE);
            std::lock_guard<std::mutex> lock(completion->mutex);
            if (wait == WAIT_OBJECT_0) {
                DWORD code = 0;
                if (GetExitCodeProcess(waitHandle, &code)) {
                    completion->exitCode = static_cast<jint>(code);
                }
                else {
                    completion->failure = jxx::NEW<jxx::io::IOException>();
                }
            }
            else {
                completion->failure = jxx::NEW<jxx::io::IOException>();
            }
            completion->done = true;
            completion->condition.notify_all();
        }).detach();
    }
    ~NativeProcess() override {
        if (process_) CloseHandle(process_);
    }
#else
    NativeProcess(
        pid_t process,
        const jxx::Ptr<jxx::io::OutputStream>& input,
        const jxx::Ptr<jxx::io::InputStream>& output,
        const jxx::Ptr<jxx::io::InputStream>& error)
        : process_(process), input_(output), error_(error), output_(input),
          completion_(std::make_shared<Completion>()) {
        auto completion = completion_;
        std::thread([process, completion] {
            int status = 0;
            pid_t result;
            do { result = waitpid(process, &status, 0); }
            while (result < 0 && errno == EINTR);

            std::lock_guard<std::mutex> lock(completion->mutex);
            if (result < 0) {
                completion->failure = jxx::NEW<jxx::io::IOException>(
                    jxx::NEW<String>(std::strerror(errno)));
            }
            else if (WIFEXITED(status)) {
                completion->exitCode = WEXITSTATUS(status);
            }
            else if (WIFSIGNALED(status)) {
                completion->exitCode = 128 + WTERMSIG(status);
            }
            else {
                completion->failure = jxx::NEW<jxx::io::IOException>();
            }
            completion->done = true;
            completion->condition.notify_all();
        }).detach();
    }
#endif

    jxx::Ptr<jxx::io::OutputStream> getOutputStream() override { return output_; }
    jxx::Ptr<jxx::io::InputStream> getInputStream() override { return input_; }
    jxx::Ptr<jxx::io::InputStream> getErrorStream() override { return error_; }

    jint waitFor() override {
        std::unique_lock<std::mutex> lock(completion_->mutex);
        completion_->condition.wait(lock, [&] { return completion_->done; });
        return completedExitValue_();
    }

    jbool waitFor(
        jlong timeout,
        const jxx::Ptr<jxx::util::concurrent::TimeUnit>& unit) override {
        if (unit == nullptr) throw NullPointerException();
        std::unique_lock<std::mutex> lock(completion_->mutex);
        if (!completion_->condition.wait_for(
                lock, unit->toChrono(timeout),
                [&] { return completion_->done; })) {
            return false;
        }
        (void)completedExitValue_();
        return true;
    }

    jint exitValue() override {
        std::lock_guard<std::mutex> lock(completion_->mutex);
        if (!completion_->done) throw IllegalThreadStateException();
        return completedExitValue_();
    }

    jbool isAlive() override {
        std::lock_guard<std::mutex> lock(completion_->mutex);
        return !completion_->done;
    }

    void destroy() override {
        if (!isAlive()) return;
#ifdef _WIN32
        if (!TerminateProcess(process_, 1)) {
            const DWORD error = GetLastError();
            if (error != ERROR_ACCESS_DENIED || isAlive()) {
                throwWindowsIOException("TerminateProcess", error);
            }
        }
#else
        if (::kill(process_, SIGTERM) != 0 && errno != ESRCH) {
            throw jxx::io::IOException(jxx::NEW<String>(std::strerror(errno)));
        }
#endif
    }

    jxx::Ptr<Process> destroyForcibly() override {
        if (isAlive()) {
#ifdef _WIN32
            if (!TerminateProcess(process_, 1)) {
                const DWORD error = GetLastError();
                if (error != ERROR_ACCESS_DENIED || isAlive()) {
                    throwWindowsIOException("TerminateProcess", error);
                }
            }
#else
            if (::kill(process_, SIGKILL) != 0 && errno != ESRCH) {
                throw jxx::io::IOException(jxx::NEW<String>(std::strerror(errno)));
            }
#endif
        }
        return jxx::CAST<Process>(thisPtr());
    }

private:
    jint completedExitValue_() const {
        if (completion_->failure != nullptr) {
            throw *jxx::CAST<jxx::io::IOException>(completion_->failure);
        }
        return completion_->exitCode;
    }

#ifdef _WIN32
    HANDLE process_;
#else
    pid_t process_;
#endif
    jxx::Ptr<jxx::io::InputStream> input_;
    jxx::Ptr<jxx::io::InputStream> error_;
    jxx::Ptr<jxx::io::OutputStream> output_;
    std::shared_ptr<Completion> completion_;
};

std::unordered_map<std::string,std::string> currentEnvironment(){std::unordered_map<std::string,std::string> result;
#ifdef _WIN32
 auto block=GetEnvironmentStringsW();if(block){for(auto p=block;*p;){std::wstring s(p);auto pos=s.find(L'=');if(pos>0)result[std::filesystem::path(s.substr(0,pos)).u8string()]=std::filesystem::path(s.substr(pos+1)).u8string();p+=s.size()+1;}FreeEnvironmentStringsW(block);}
#else
 for(char**p=environ;p&&*p;++p){std::string s(*p);auto pos=s.find('=');if(pos!=std::string::npos)result[s.substr(0,pos)]=s.substr(pos+1);}
#endif
 return result;}
}

ProcessBuilder::Redirect::Redirect(Type type, const jxx::Ptr<jxx::io::File>& file)
    : type_(type), file_(file) {}
jxx::Ptr<ProcessBuilder::Redirect> ProcessBuilder::Redirect::PIPE =
    jxx::Ptr<Redirect>(new Redirect(Type::PIPE, nullptr));
jxx::Ptr<ProcessBuilder::Redirect> ProcessBuilder::Redirect::INHERIT =
    jxx::Ptr<Redirect>(new Redirect(Type::INHERIT, nullptr));
jxx::Ptr<ProcessBuilder::Redirect> ProcessBuilder::Redirect::from(const jxx::Ptr<jxx::io::File>& file){if(!file)throw NullPointerException();return jxx::Ptr<Redirect>(new Redirect(Type::READ,file));}
jxx::Ptr<ProcessBuilder::Redirect> ProcessBuilder::Redirect::to(const jxx::Ptr<jxx::io::File>& file){if(!file)throw NullPointerException();return jxx::Ptr<Redirect>(new Redirect(Type::WRITE,file));}
jxx::Ptr<ProcessBuilder::Redirect> ProcessBuilder::Redirect::appendTo(const jxx::Ptr<jxx::io::File>& file){if(!file)throw NullPointerException();return jxx::Ptr<Redirect>(new Redirect(Type::APPEND,file));}
ProcessBuilder::Redirect::Type ProcessBuilder::Redirect::type()const{return type_;}
jxx::Ptr<jxx::io::File> ProcessBuilder::Redirect::file()const{return file_;}
jbool ProcessBuilder::Redirect::equals(const jxx::Ptr<Object>& other) const {auto value=jxx::CAST<Redirect>(other);if(value==nullptr||type_!=value->type_)return false;if(file_==nullptr||value->file_==nullptr)return file_==value->file_;return file_->getPath()->equals(value->file_->getPath());}
jint ProcessBuilder::Redirect::hashCode() const {return static_cast<jint>(type_)*31+(file_==nullptr?0:file_->getPath()->hashCode());}

ProcessBuilder::ProcessBuilder(const jxx::Ptr<JxxArray<jxx::Ptr<String>,1>>& c):environment_(currentEnvironment()),inputRedirect_(Redirect::PIPE),outputRedirect_(Redirect::PIPE),errorRedirect_(Redirect::PIPE){command(c);}
jxx::Ptr<ProcessBuilder> ProcessBuilder::command(const jxx::Ptr<JxxArray<jxx::Ptr<String>,1>>& c){if(!c||c->length==0)throw IllegalArgumentException();command_.clear();for(uint32_t i=0;i<c->length;++i){if(!(*c)[i])throw NullPointerException();command_.push_back((*c)[i]->utf8());}return jxx::CAST<ProcessBuilder>(thisPtr());}
jxx::Ptr<JxxArray<jxx::Ptr<String>,1>> ProcessBuilder::command()const{auto a=jxx::NEW<JxxArray<jxx::Ptr<String>,1>>(command_.size());for(uint32_t i=0;i<a->length;++i)(*a)[i]=jxx::NEW<String>(command_[i]);return a;}
jxx::Ptr<jxx::io::File> ProcessBuilder::directory()const{return directory_;}
jxx::Ptr<ProcessBuilder> ProcessBuilder::directory(const jxx::Ptr<jxx::io::File>&d){directory_=d;return jxx::CAST<ProcessBuilder>(thisPtr());}
jxx::Ptr<ProcessBuilder> ProcessBuilder::environment(const jxx::Ptr<String>&n,const jxx::Ptr<String>&v){if(!n||!v)throw NullPointerException();environment_[n->utf8()]=v->utf8();return jxx::CAST<ProcessBuilder>(thisPtr());}
jxx::Ptr<String> ProcessBuilder::environment(const jxx::Ptr<String>&n)const{if(!n)throw NullPointerException();auto i=environment_.find(n->utf8());return i==environment_.end()?nullptr:jxx::NEW<String>(i->second);}
jxx::Ptr<ProcessBuilder> ProcessBuilder::clearEnvironment(){environment_.clear();return jxx::CAST<ProcessBuilder>(thisPtr());}
jxx::Ptr<ProcessBuilder::Redirect> ProcessBuilder::redirectInput()const{return inputRedirect_;}
jxx::Ptr<ProcessBuilder::Redirect> ProcessBuilder::redirectOutput()const{return outputRedirect_;}
jxx::Ptr<ProcessBuilder::Redirect> ProcessBuilder::redirectError()const{return errorRedirect_;}
jxx::Ptr<ProcessBuilder> ProcessBuilder::redirectInput(const jxx::Ptr<Redirect>& r){if(!r)throw NullPointerException();if(r->type()!=Redirect::Type::PIPE&&r->type()!=Redirect::Type::INHERIT&&r->type()!=Redirect::Type::READ)throw IllegalArgumentException();inputRedirect_=r;return jxx::CAST<ProcessBuilder>(thisPtr());}
jxx::Ptr<ProcessBuilder> ProcessBuilder::redirectOutput(const jxx::Ptr<Redirect>& r){if(!r)throw NullPointerException();if(r->type()==Redirect::Type::READ)throw IllegalArgumentException();outputRedirect_=r;return jxx::CAST<ProcessBuilder>(thisPtr());}
jxx::Ptr<ProcessBuilder> ProcessBuilder::redirectError(const jxx::Ptr<Redirect>& r){if(!r)throw NullPointerException();if(r->type()==Redirect::Type::READ)throw IllegalArgumentException();errorRedirect_=r;return jxx::CAST<ProcessBuilder>(thisPtr());}
jxx::Ptr<ProcessBuilder> ProcessBuilder::redirectInput(const jxx::Ptr<jxx::io::File>& f){return redirectInput(Redirect::from(f));}
jxx::Ptr<ProcessBuilder> ProcessBuilder::redirectOutput(const jxx::Ptr<jxx::io::File>& f){return redirectOutput(Redirect::to(f));}
jxx::Ptr<ProcessBuilder> ProcessBuilder::redirectError(const jxx::Ptr<jxx::io::File>& f){return redirectError(Redirect::to(f));}
jxx::Ptr<ProcessBuilder> ProcessBuilder::inheritIO(){inputRedirect_=Redirect::INHERIT;outputRedirect_=Redirect::INHERIT;errorRedirect_=Redirect::INHERIT;return jxx::CAST<ProcessBuilder>(thisPtr());}
jbool ProcessBuilder::redirectErrorStream()const{return redirectErrorStream_;}
jxx::Ptr<ProcessBuilder> ProcessBuilder::redirectErrorStream(jbool v){redirectErrorStream_=v;return jxx::CAST<ProcessBuilder>(thisPtr());}

jxx::Ptr<Process> ProcessBuilder::start(){
 if(command_.empty() || command_[0].empty())throw IllegalArgumentException();
 for(const auto& argument:command_)if(argument.find('\0')!=std::string::npos)throw IllegalArgumentException();
 for(const auto& item:environment_){if(item.first.empty()||item.first.find('=')!=std::string::npos||item.first.find('\0')!=std::string::npos||item.second.find('\0')!=std::string::npos)throw IllegalArgumentException();}
#ifdef _WIN32
 WinHandle childInput,parentInput,childOutput,parentOutput,childError,parentError;
 auto configureInput=[&]{
   if(inputRedirect_->type()==Redirect::Type::PIPE)createPipePair(childInput,parentInput,true);
   else if(inputRedirect_->type()==Redirect::Type::INHERIT){HANDLE duplicate=nullptr;HANDLE source=GetStdHandle(STD_INPUT_HANDLE);if(source==nullptr||source==INVALID_HANDLE_VALUE||!DuplicateHandle(GetCurrentProcess(),source,GetCurrentProcess(),&duplicate,0,TRUE,DUPLICATE_SAME_ACCESS))throw jxx::io::IOException();childInput.reset(duplicate);}
   else childInput=openRedirectFile(inputRedirect_,true);
 };
 auto configureOutput=[&](const jxx::Ptr<Redirect>& redirect,WinHandle& child,WinHandle& parent,DWORD standardHandle){
   if(redirect->type()==Redirect::Type::PIPE)createPipePair(child,parent,false);
   else if(redirect->type()==Redirect::Type::INHERIT){HANDLE duplicate=nullptr;HANDLE source=GetStdHandle(standardHandle);if(source==nullptr||source==INVALID_HANDLE_VALUE||!DuplicateHandle(GetCurrentProcess(),source,GetCurrentProcess(),&duplicate,0,TRUE,DUPLICATE_SAME_ACCESS))throw jxx::io::IOException();child.reset(duplicate);}
   else child=openRedirectFile(redirect,false);
 };
 configureInput();
 configureOutput(outputRedirect_,childOutput,parentOutput,STD_OUTPUT_HANDLE);
 if(redirectErrorStream_) {
   HANDLE duplicate=nullptr;
   if(!DuplicateHandle(GetCurrentProcess(),childOutput.get(),GetCurrentProcess(),&duplicate,0,TRUE,DUPLICATE_SAME_ACCESS))throw jxx::io::IOException();
   childError.reset(duplicate);
 } else configureOutput(errorRedirect_,childError,parentError,STD_ERROR_HANDLE);

 std::wstring commandLine;
 for(const auto& argument:command_){if(!commandLine.empty())commandLine.push_back(L' ');commandLine+=quoteArgument(utf16(argument));}
 std::vector<wchar_t> writableCommand(commandLine.begin(),commandLine.end());writableCommand.push_back(L'\0');
 auto environmentBlock=makeEnvironmentBlock(environment_);
 std::wstring directory=directory_?utf16(directory_->getPath()->utf8()):std::wstring();
 STARTUPINFOEXW startup{};startup.StartupInfo.cb=sizeof(startup);startup.StartupInfo.dwFlags=STARTF_USESTDHANDLES;
 startup.StartupInfo.hStdInput=childInput.get();startup.StartupInfo.hStdOutput=childOutput.get();startup.StartupInfo.hStdError=childError.get();
 std::vector<HANDLE> inheritedHandles{childInput.get(),childOutput.get(),childError.get()};
 std::sort(inheritedHandles.begin(),inheritedHandles.end());
 inheritedHandles.erase(std::unique(inheritedHandles.begin(),inheritedHandles.end()),inheritedHandles.end());
 SIZE_T attributeSize=0;InitializeProcThreadAttributeList(nullptr,1,0,&attributeSize);
 std::vector<unsigned char> attributeStorage(attributeSize);
 startup.lpAttributeList=reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(attributeStorage.data());
 if(!InitializeProcThreadAttributeList(startup.lpAttributeList,1,0,&attributeSize))throwWindowsIOException("InitializeProcThreadAttributeList");
 struct AttributeGuard{LPPROC_THREAD_ATTRIBUTE_LIST value;~AttributeGuard(){if(value)DeleteProcThreadAttributeList(value);}} attributeGuard{startup.lpAttributeList};
 if(!UpdateProcThreadAttribute(startup.lpAttributeList,0,PROC_THREAD_ATTRIBUTE_HANDLE_LIST,inheritedHandles.data(),inheritedHandles.size()*sizeof(HANDLE),nullptr,nullptr))throwWindowsIOException("UpdateProcThreadAttribute");
 PROCESS_INFORMATION process{};
 const DWORD creationFlags=CREATE_UNICODE_ENVIRONMENT|EXTENDED_STARTUPINFO_PRESENT;
 if(!CreateProcessW(nullptr,writableCommand.data(),nullptr,nullptr,TRUE,creationFlags,environmentBlock.data(),directory.empty()?nullptr:directory.c_str(),&startup.StartupInfo,&process))throwWindowsIOException("CreateProcessW");
 WinHandle processHandle(process.hProcess),threadHandle(process.hThread);
 childInput.reset();childOutput.reset();childError.reset();
 jxx::Ptr<jxx::io::OutputStream> input=inputRedirect_->type()==Redirect::Type::PIPE?jxx::CAST<jxx::io::OutputStream>(jxx::NEW<PipeOutput>(parentInput.release())):jxx::CAST<jxx::io::OutputStream>(jxx::NEW<NullOutputStream>());
 jxx::Ptr<jxx::io::InputStream> output=outputRedirect_->type()==Redirect::Type::PIPE?jxx::CAST<jxx::io::InputStream>(jxx::NEW<PipeInput>(parentOutput.release())):jxx::CAST<jxx::io::InputStream>(jxx::NEW<NullInputStream>());
 jxx::Ptr<jxx::io::InputStream> error=(!redirectErrorStream_&&errorRedirect_->type()==Redirect::Type::PIPE)?jxx::CAST<jxx::io::InputStream>(jxx::NEW<PipeInput>(parentError.release())):jxx::CAST<jxx::io::InputStream>(jxx::NEW<NullInputStream>());
 return jxx::CAST<Process>(jxx::NEW<NativeProcess>(processHandle.release(),input,output,error));
#else
 int inPipe[2],outPipe[2],errPipe[2],startup[2];
 if(pipe(inPipe)||pipe(outPipe)||pipe(errPipe)||pipe(startup))throw jxx::io::IOException();
 fcntl(startup[1],F_SETFD,FD_CLOEXEC);
 pid_t pid=fork();
 if(pid<0)throw jxx::io::IOException();
 if(pid==0){
   close(startup[0]);
   auto configure=[&](const jxx::Ptr<Redirect>& r,int standardFd,int pipeFd,bool input){
     if(r->type()==Redirect::Type::PIPE){dup2(pipeFd,standardFd);return;}
     if(r->type()==Redirect::Type::INHERIT)return;
     int flags=input?O_RDONLY:(O_WRONLY|O_CREAT|(r->type()==Redirect::Type::APPEND?O_APPEND:O_TRUNC));
     int fd=open(r->file()->getPath()->utf8().c_str(),flags,0666);if(fd<0){int e=errno;write(startup[1],&e,sizeof(e));_exit(127);}dup2(fd,standardFd);close(fd);
   };
   configure(inputRedirect_,STDIN_FILENO,inPipe[0],true);
   configure(outputRedirect_,STDOUT_FILENO,outPipe[1],false);
   configure(redirectErrorStream_?outputRedirect_:errorRedirect_,STDERR_FILENO,redirectErrorStream_?outPipe[1]:errPipe[1],false);
   close(inPipe[0]);close(inPipe[1]);close(outPipe[0]);close(outPipe[1]);close(errPipe[0]);close(errPipe[1]);
   if(directory_&&chdir(directory_->getPath()->utf8().c_str())!=0){int e=errno;write(startup[1],&e,sizeof(e));_exit(127);}
   std::vector<std::string> envStrings;for(const auto&v:environment_)envStrings.push_back(v.first+"="+v.second);
   std::vector<char*>argv,envp;for(auto&s:command_)argv.push_back(const_cast<char*>(s.c_str()));argv.push_back(nullptr);for(auto&s:envStrings)envp.push_back(const_cast<char*>(s.c_str()));envp.push_back(nullptr);
   std::string executable=command_[0];
   if(executable.find('/')==std::string::npos){auto it=environment_.find("PATH");std::string path=it==environment_.end()?"/bin:/usr/bin":it->second;size_t begin=0;while(begin<=path.size()){size_t split=path.find(':',begin);std::string dir=path.substr(begin,split==std::string::npos?std::string::npos:split-begin);std::string candidate=(dir.empty()?".":dir)+"/"+executable;if(access(candidate.c_str(),X_OK)==0){executable=candidate;break;}if(split==std::string::npos)break;begin=split+1;}}
   execve(executable.c_str(),argv.data(),envp.data());int e=errno;write(startup[1],&e,sizeof(e));_exit(127);
 }
 close(startup[1]);close(inPipe[0]);close(outPipe[1]);close(errPipe[1]);
 int childError=0;ssize_t count;do{count=read(startup[0],&childError,sizeof(childError));}while(count<0&&errno==EINTR);close(startup[0]);
 if(count>0){close(inPipe[1]);close(outPipe[0]);close(errPipe[0]);int status=0;waitpid(pid,&status,0);throw jxx::io::IOException(jxx::NEW<String>(std::strerror(childError)));}
 jxx::Ptr<jxx::io::OutputStream> input;if(inputRedirect_->type()==Redirect::Type::PIPE)input=jxx::NEW<PipeOutput>(inPipe[1]);else{close(inPipe[1]);input=jxx::NEW<NullOutputStream>();}
 jxx::Ptr<jxx::io::InputStream> output;if(outputRedirect_->type()==Redirect::Type::PIPE)output=jxx::NEW<PipeInput>(outPipe[0]);else{close(outPipe[0]);output=jxx::NEW<NullInputStream>();}
 jxx::Ptr<jxx::io::InputStream> error;if(!redirectErrorStream_&&errorRedirect_->type()==Redirect::Type::PIPE)error=jxx::NEW<PipeInput>(errPipe[0]);else{close(errPipe[0]);error=jxx::NEW<NullInputStream>();}
 return jxx::CAST<Process>(jxx::NEW<NativeProcess>(pid,input,output,error));
#endif
}
} // namespace jxx::lang
