
#include "jxx/io/jxx.io.SequenceInputStream.h"
namespace jxx { namespace io {
SequenceInputStream::SequenceInputStream(std::shared_ptr<InputStream> s1, std::shared_ptr<InputStream> s2) : seq{std::move(s1),std::move(s2)}, idx(0){}
SequenceInputStream::SequenceInputStream(std::vector<std::shared_ptr<InputStream>> streams) : seq(std::move(streams)), idx(0){}
int SequenceInputStream::read(){ while(idx<seq.size()){ int c=seq[idx]->read(); if(c!=-1) return c; ++idx;} return -1; }
int SequenceInputStream::read(ByteArray& b, int off, int len){ if(len==0) return 0; int total=0; while(len>0 && idx<seq.size()){ auto r=seq[idx]->read(b,off,len); if(r==-1){ ++idx; continue; } off+=r; len-=r; total+=r; if(r>0) break; } return (total>0)? total : -1; }
int SequenceInputStream::available(){ if(idx>=seq.size()) return 0; return seq[idx]->available(); }
void SequenceInputStream::close(){ for(auto&s:seq) if(s) s->close(); }
}}
