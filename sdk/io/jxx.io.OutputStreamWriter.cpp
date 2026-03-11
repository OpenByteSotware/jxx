
#include "io/jxx.io.OutputStreamWriter.h"
#include "io/jxx.io.IOException.h"
namespace jxx { namespace io {
OutputStreamWriter::OutputStreamWriter(std::shared_ptr<OutputStream> out_, const std::string& cs): out(std::move(out_)), charset(cs){}
void OutputStreamWriter::write(int c){ jxx::lang::jchar ch=(jxx::lang::jchar)c; std::u16string s(1,ch); ByteArray enc=UTF8::encode(s); out->write(enc);} 
void OutputStreamWriter::write(const jxx::lang::jchar* buf, int off, int len){ if(!buf||off<0||len<0) throw IOException("OutputStreamWriter.write: invalid args"); std::u16string s(buf+off, buf+off+len); ByteArray enc=UTF8::encode(s); out->write(enc);} 
void OutputStreamWriter::write(const std::u16string& s){ if(s.empty()) return; ByteArray enc=UTF8::encode(s); out->write(enc);} 
void OutputStreamWriter::flush(){ out->flush(); } 
void OutputStreamWriter::close(){ out->close(); }
}}
