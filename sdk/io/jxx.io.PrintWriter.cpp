
#include "io/jxx.io.PrintWriter.h"
#include "io/jxx.io.UTF8.h"
#include <sstream>
namespace jxx { namespace io {
PrintWriter::PrintWriter(std::shared_ptr<Writer> w, bool autoFlush_) : out(std::move(w)), autoFlush(autoFlush_) {}
PrintWriter::PrintWriter(std::shared_ptr<OutputStream> os, bool autoFlush_, const std::string& charset) : out(std::make_shared<OutputStreamWriter>(std::move(os), charset)), autoFlush(autoFlush_) {}
void PrintWriter::flush(){ try{ out->flush(); } catch(...){ setError(); } }
void PrintWriter::close(){ try{ out->close(); } catch(...){ setError(); } }
void PrintWriter::doWrite(const std::u16string& s){ try{ out->write(s);} catch(...){ setError(); } }
void PrintWriter::write(const jxx::lang::jchar* buf, int off, int len){ try{ out->write(buf,off,len);} catch(...){ setError(); } }
void PrintWriter::write(const std::u16string& s){ doWrite(s); }
void PrintWriter::print(const std::string& s){ std::u16string u; u.reserve(s.size()); for(unsigned char c: s) u.push_back((jxx::lang::jchar)c); doWrite(u);} 
void PrintWriter::print(const char* s){ print(s? std::string(s) : std::string("null")); }
void PrintWriter::print(const std::u16string& s){ doWrite(s);} 
void PrintWriter::print(bool v){ print(v? std::string("true") : std::string("false")); }
void PrintWriter::print(int v){ std::ostringstream oss; oss<<v; print(oss.str()); }
void PrintWriter::print(long long v){ std::ostringstream oss; oss<<v; print(oss.str()); }
void PrintWriter::print(float v){ std::ostringstream oss; oss<<v; print(oss.str()); }
void PrintWriter::print(double v){ std::ostringstream oss; oss<<v; print(oss.str()); }
void PrintWriter::println(){ print("
"); if(autoFlush) flush(); }
void PrintWriter::println(const std::string& s){ print(s); println(); }
void PrintWriter::println(const char* s){ print(s); println(); }
void PrintWriter::println(const std::u16string& s){ print(s); println(); }
void PrintWriter::println(bool v){ print(v); println(); }
void PrintWriter::println(int v){ print(v); println(); }
void PrintWriter::println(long long v){ print(v); println(); }
void PrintWriter::println(float v){ print(v); println(); }
void PrintWriter::println(double v){ print(v); println(); }
}}
