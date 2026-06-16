#include "io/jxx.io.FileDescriptor.h"
#include "lang/jxx.lang.System.h"

using namespace jxx::io;
namespace jxx { namespace lang {

std::shared_ptr<InputStream> System::in;
std::shared_ptr<PrintStream> System::out;
std::shared_ptr<PrintStream> System::err;

static struct __SystemBootstrap { __SystemBootstrap(){ System::init(); } } __sys_bootstrap;

void System::init(){
    System::in  = jxx::NEW<FileInputStream>(FileDescriptor::in);
    auto outOs  = jxx::NEW<FileOutputStream>(FileDescriptor::out);
    auto errOs  = jxx::NEW<FileOutputStream>(FileDescriptor::err);
    System::out = jxx::NEW<PrintStream>(outOs, true);
    System::err = jxx::NEW<PrintStream>(errOs, true);
}

jxx::lang::jlong System::currentTimeMillis() {
    using namespace std::chrono;
    auto now = time_point_cast<milliseconds>(system_clock::now());
    return (jxx::lang::jlong)now.time_since_epoch().count();
}

}} // ns

