#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.UTF8.h"

#include <sstream>
namespace jxx { namespace io {
PrintStream::PrintStream(std::shared_ptr<OutputStream> out_, bool autoFlush_, const std::string& charset): out(std::move(out_)), writer(std::make_shared<OutputStreamWriter>(out, charset)), autoFlush(autoFlush_){}
void PrintStream::handleError(){ error_=true; }
void PrintStream::writeString(const String& s){ try{ ByteArray b(s.begin(), s.end());
out->write(b.data(), b.length); } catch (...) { handleError(); } }
void PrintStream::writeU16(const std::u16string& s){ try{
	auto b=UTF8::encode(s); out->write(b->data(), b->length); }
catch(...){ handleError(); } }
void PrintStream::flush(){ try{ out->flush(); } catch(...){ handleError(); } }
void PrintStream::close(){ try{ out->close(); } catch(...){ handleError(); } }
void PrintStream::newline(){
	writeString(lineSep); 
	if(autoFlush) flush();
}
void PrintStream::print(const std::string& s){ writeString(s);} void PrintStream::print(const char* s){ 
	writeString(s? std::string(s) : std::string("null")); } void PrintStream::print(const std::u16string& s){ writeU16(s);} 
	void PrintStream::print(bool v){ writeString(v?"true":"false"); } void PrintStream::print(int v){ std::ostringstream oss; oss<<v; writeString(oss.str()); } 
	void PrintStream::print(long long v){ std::ostringstream oss; oss<<v; writeString(oss.str()); }
	void PrintStream::print(float v){ std::ostringstream oss; oss<<v; writeString(oss.str()); } 
	void PrintStream::print(double v){ std::ostringstream oss; oss<<v; writeString(oss.str()); }
void PrintStream::println(){ newline(); } void PrintStream::println(const std::string& s){ print(s); newline(); } 
void PrintStream::println(const char* s){ print(s); newline(); } void PrintStream::println(const std::u16string& s){ print(s); newline(); } 
void PrintStream::println(bool v){ print(v); newline(); } void PrintStream::println(int v){ print(v); newline(); } 
void PrintStream::println(long long v){ print(v); newline(); } void PrintStream::println(float v){ print(v); newline(); } 
void PrintStream::println(double v){ print(v); newline(); }
}}
