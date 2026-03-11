
#include "lang/jxx.lang.System.h"

using namespace jxx::io;
namespace jxx { namespace lang {

std::shared_ptr<InputStream> System::in;
std::shared_ptr<PrintStream> System::out;
std::shared_ptr<PrintStream> System::err;

static struct __SystemBootstrap { __SystemBootstrap(){ System::init(); } } __sys_bootstrap;

void System::init(){
    System::in  = std::make_shared<FileInputStream>(FileDescriptor::in());
    auto outOs  = std::make_shared<FileOutputStream>(FileDescriptor::out());
    auto errOs  = std::make_shared<FileOutputStream>(FileDescriptor::err());
    System::out = std::make_shared<PrintStream>(outOs, true);
    System::err = std::make_shared<PrintStream>(errOs, true);
}
}} // ns

jxx::lang::jlong System::currentTimeMillis(){
    using namespace std::chrono;
    auto now = time_point_cast<milliseconds>(system_clock::now());
    return (jxx::lang::jlong)now.time_since_epoch().count();
}
