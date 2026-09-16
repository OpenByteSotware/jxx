#include "lang/jxx.lang.ProcessBuilder.h"

#include <atomic>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <mutex>
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

namespace jxx::lang {
namespace {
#ifdef _WIN32
using NativeHandle=HANDLE;
static constexpr NativeHandle invalidHandle=nullptr;
#else
using NativeHandle=int;
static constexpr NativeHandle invalidHandle=-1;
#endif

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
  DWORD n=0; return handle_&&PeekNamedPipe(handle_,nullptr,0,nullptr,&n,nullptr)?static_cast<jint>(n):0;
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
  DWORD r=0; return handle_&&ReadFile(handle_,p,n,&r,nullptr)?static_cast<jint>(r):-1;
#else
  auto r=::read(handle_,p,static_cast<size_t>(n)); return static_cast<jint>(r);
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
  DWORD w=0;if(!handle_||!WriteFile(handle_,p,n,&w,nullptr)||w!=static_cast<DWORD>(n))throw jxx::io::IOException();
#else
  const auto* q=static_cast<const unsigned char*>(p);jint done=0;while(done<n){auto w=::write(handle_,q+done,n-done);if(w<0)throw jxx::io::IOException();done+=static_cast<jint>(w);}
#endif
 }
 NativeHandle handle_;
};

class NativeProcess final : public Process {
public:
#ifdef _WIN32
 NativeProcess(HANDLE process,HANDLE in,HANDLE out,HANDLE err):process_(process),input_(jxx::NEW<PipeInput>(out)),error_(jxx::NEW<PipeInput>(err)),output_(jxx::NEW<PipeOutput>(in)){}
 ~NativeProcess() override { if(process_)CloseHandle(process_); }
#else
 NativeProcess(pid_t process,const jxx::Ptr<jxx::io::OutputStream>& in,const jxx::Ptr<jxx::io::InputStream>& out,const jxx::Ptr<jxx::io::InputStream>& err):process_(process),input_(out),error_(err),output_(in){}
#endif
 jxx::Ptr<jxx::io::OutputStream> getOutputStream() override{return output_;}
 jxx::Ptr<jxx::io::InputStream> getInputStream() override{return input_;}
 jxx::Ptr<jxx::io::InputStream> getErrorStream() override{return error_;}
 jint waitFor() override {
  std::lock_guard<std::mutex>l(mutex_);if(done_)return exit_;
#ifdef _WIN32
  WaitForSingleObject(process_,INFINITE);DWORD code=0;GetExitCodeProcess(process_,&code);exit_=static_cast<jint>(code);
#else
  int status=0;while(waitpid(process_,&status,0)<0&&errno==EINTR){} exit_=WIFEXITED(status)?WEXITSTATUS(status):128+WTERMSIG(status);
#endif
  done_=true;return exit_;
 }
 jint exitValue() override {
  std::lock_guard<std::mutex>l(mutex_);if(done_)return exit_;
#ifdef _WIN32
  DWORD code=STILL_ACTIVE;if(!GetExitCodeProcess(process_,&code)||code==STILL_ACTIVE)throw IllegalThreadStateException();exit_=static_cast<jint>(code);
#else
  int status=0;auto r=waitpid(process_,&status,WNOHANG);if(r==0)throw IllegalThreadStateException();if(r<0)throw IllegalThreadStateException();exit_=WIFEXITED(status)?WEXITSTATUS(status):128+WTERMSIG(status);
#endif
  done_=true;return exit_;
 }
 void destroy() override {
#ifdef _WIN32
  TerminateProcess(process_,1);
#else
  ::kill(process_,SIGTERM);
#endif
 }
 jxx::Ptr<Process> destroyForcibly() override {
#ifdef _WIN32
  TerminateProcess(process_,1);
#else
  ::kill(process_,SIGKILL);
#endif
  return jxx::CAST<Process>(thisPtr());
 }
private:
#ifdef _WIN32
 HANDLE process_;
#else
 pid_t process_;
#endif
 jxx::Ptr<jxx::io::InputStream> input_,error_;jxx::Ptr<jxx::io::OutputStream> output_;std::mutex mutex_;jbool done_=false;jint exit_=0;
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

jxx::Ptr<Process> ProcessBuilder::start(){if(command_.empty())throw IllegalArgumentException();
#ifdef _WIN32
 SECURITY_ATTRIBUTES sa{sizeof(sa),nullptr,TRUE};HANDLE cinR,cinW,coutR,coutW,cerrR,cerrW;if(!CreatePipe(&cinR,&cinW,&sa,0)||!CreatePipe(&coutR,&coutW,&sa,0)||!CreatePipe(&cerrR,&cerrW,&sa,0))throw jxx::io::IOException();SetHandleInformation(cinW,HANDLE_FLAG_INHERIT,0);SetHandleInformation(coutR,HANDLE_FLAG_INHERIT,0);SetHandleInformation(cerrR,HANDLE_FLAG_INHERIT,0);std::wstring cmd;for(const auto&s:command_){if(!cmd.empty())cmd+=L' ';cmd+=L'"'+std::filesystem::u8path(s).wstring()+L'"';}STARTUPINFOW si{};si.cb=sizeof(si);si.dwFlags=STARTF_USESTDHANDLES;si.hStdInput=cinR;si.hStdOutput=coutW;si.hStdError=redirectErrorStream_?coutW:cerrW;PROCESS_INFORMATION pi{};auto dir=directory_?std::filesystem::u8path(directory_->getPath()->utf8()).wstring():std::wstring();if(!CreateProcessW(nullptr,cmd.data(),nullptr,nullptr,TRUE,0,nullptr,dir.empty()?nullptr:dir.c_str(),&si,&pi))throw jxx::io::IOException();CloseHandle(pi.hThread);CloseHandle(cinR);CloseHandle(coutW);CloseHandle(cerrW);return jxx::CAST<Process>(jxx::NEW<NativeProcess>(pi.hProcess,cinW,coutR,cerrR));
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
 auto input=inputRedirect_->type()==Redirect::Type::PIPE?jxx::NEW<PipeOutput>(inPipe[1]):jxx::Ptr<jxx::io::OutputStream>(nullptr);if(!input)close(inPipe[1]);
 auto output=outputRedirect_->type()==Redirect::Type::PIPE?jxx::NEW<PipeInput>(outPipe[0]):jxx::Ptr<jxx::io::InputStream>(nullptr);if(!output)close(outPipe[0]);
 auto error=(!redirectErrorStream_&&errorRedirect_->type()==Redirect::Type::PIPE)?jxx::NEW<PipeInput>(errPipe[0]):jxx::Ptr<jxx::io::InputStream>(nullptr);if(!error)close(errPipe[0]);
 return jxx::CAST<Process>(jxx::NEW<NativeProcess>(pid,input,output,error));
#endif
}
} // namespace jxx::lang
