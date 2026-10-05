#include "util/zip/jxx.util.zip.Inflater.h"

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/zip/jxx.util.zip.DataFormatException.h"

namespace jxx::util::zip {
namespace { void throwInflaterState(const char* m){ throw ::jxx::lang::IllegalStateException(m); } }
Inflater::Inflater():Inflater(false){}
Inflater::Inflater(::jxx::lang::jbool nowrap):nowrap_(nowrap){ if(::inflateInit2(&stream_,nowrap?-MAX_WBITS:MAX_WBITS)!=Z_OK) throwInflaterState("unable to initialize inflater"); }
Inflater::~Inflater(){end();}
void Inflater::ensureOpen_()const{if(ended_)throwInflaterState("inflater has been ended");}
void Inflater::checkRange_(const ::jxx::lang::ByteArray&a,::jxx::lang::jint o,::jxx::lang::jint l){if(a==nullptr)throw ::jxx::lang::NullPointerException();if(o<0||l<0||o>a->length-l)throw ::jxx::lang::ArrayIndexOutOfBoundsException();}
void Inflater::setInput(const ::jxx::lang::ByteArray&input){if(input==nullptr)throw ::jxx::lang::NullPointerException();setInput(input,0,input->length);}
void Inflater::setInput(const ::jxx::lang::ByteArray&input,::jxx::lang::jint offset,::jxx::lang::jint length){ensureOpen_();checkRange_(input,offset,length);input_=input;inputOffset_=offset;inputLength_=length;}
void Inflater::setDictionary(const ::jxx::lang::ByteArray&d){if(d==nullptr)throw ::jxx::lang::NullPointerException();setDictionary(d,0,d->length);}
void Inflater::setDictionary(const ::jxx::lang::ByteArray&d,::jxx::lang::jint o,::jxx::lang::jint l){ensureOpen_();checkRange_(d,o,l);const auto r=::inflateSetDictionary(&stream_,reinterpret_cast<const Bytef*>(&(*d)[o]),static_cast<uInt>(l));if(r!=Z_OK)throw ::jxx::lang::IllegalArgumentException();dictionaryNeeded_=false;}
::jxx::lang::jint Inflater::getRemaining()const noexcept{return inputLength_;}
::jxx::lang::jbool Inflater::needsInput()const noexcept{return inputLength_==0;}
::jxx::lang::jbool Inflater::needsDictionary()const noexcept{return dictionaryNeeded_;}
::jxx::lang::jbool Inflater::finished()const noexcept{return finished_;}
::jxx::lang::jint Inflater::inflate(const ::jxx::lang::ByteArray&o){if(o==nullptr)throw ::jxx::lang::NullPointerException();return inflate(o,0,o->length);}
::jxx::lang::jint Inflater::inflate(const ::jxx::lang::ByteArray&o,::jxx::lang::jint off,::jxx::lang::jint len){
 ensureOpen_();checkRange_(o,off,len);stream_.next_in=inputLength_==0?Z_NULL:reinterpret_cast<Bytef*>(&(*input_)[inputOffset_]);stream_.avail_in=static_cast<uInt>(inputLength_);stream_.next_out=len==0?Z_NULL:reinterpret_cast<Bytef*>(&(*o)[off]);stream_.avail_out=static_cast<uInt>(len);const auto bi=stream_.avail_in,bo=stream_.avail_out;const auto r=::inflate(&stream_,Z_NO_FLUSH);const auto used=bi-stream_.avail_in;inputOffset_+=static_cast<::jxx::lang::jint>(used);inputLength_-=static_cast<::jxx::lang::jint>(used);if(inputLength_==0)input_.reset();dictionaryNeeded_=r==Z_NEED_DICT;if(r==Z_STREAM_END)finished_=true;else if(r==Z_DATA_ERROR||r==Z_STREAM_ERROR||r==Z_MEM_ERROR)throw DataFormatException(stream_.msg==nullptr?"invalid compressed data":stream_.msg);return static_cast<::jxx::lang::jint>(bo-stream_.avail_out);
}
::jxx::lang::jint Inflater::getAdler()const{ensureOpen_();return static_cast<::jxx::lang::jint>(stream_.adler);}
::jxx::lang::jint Inflater::getTotalIn()const noexcept{return static_cast<::jxx::lang::jint>(stream_.total_in);}
::jxx::lang::jlong Inflater::getBytesRead()const noexcept{return static_cast<::jxx::lang::jlong>(stream_.total_in);}
::jxx::lang::jint Inflater::getTotalOut()const noexcept{return static_cast<::jxx::lang::jint>(stream_.total_out);}
::jxx::lang::jlong Inflater::getBytesWritten()const noexcept{return static_cast<::jxx::lang::jlong>(stream_.total_out);}
void Inflater::reset(){ensureOpen_();if(::inflateReset(&stream_)!=Z_OK)throwInflaterState("unable to reset inflater");input_.reset();inputOffset_=inputLength_=0;dictionaryNeeded_=finished_=false;}
void Inflater::end()noexcept{if(!ended_){::inflateEnd(&stream_);ended_=true;input_.reset();inputOffset_=inputLength_=0;}}
} // namespace jxx::util::zip
