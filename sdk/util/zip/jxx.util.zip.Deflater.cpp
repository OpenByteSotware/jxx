#include "util/zip/jxx.util.zip.Deflater.h"

#include <limits>

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::util::zip {
namespace {
void throwState(const char* message) { throw ::jxx::lang::IllegalStateException(message); }
}

Deflater::Deflater() : Deflater(DEFAULT_COMPRESSION, false) {}
Deflater::Deflater(::jxx::lang::jint level) : Deflater(level, false) {}
Deflater::Deflater(::jxx::lang::jint level, ::jxx::lang::jbool nowrap)
    : level_(level), nowrap_(nowrap) {
    if (level < DEFAULT_COMPRESSION || level > BEST_COMPRESSION) throw ::jxx::lang::IllegalArgumentException();
    const auto result = ::deflateInit2(&stream_, level, Z_DEFLATED, nowrap ? -MAX_WBITS : MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
    if (result != Z_OK) throwState("unable to initialize deflater");
}
Deflater::~Deflater() { end(); }
void Deflater::ensureOpen_() const { if (ended_) throwState("deflater has been ended"); }
void Deflater::checkRange_(const ::jxx::lang::ByteArray& a, ::jxx::lang::jint o, ::jxx::lang::jint l) {
    if (a == nullptr) throw ::jxx::lang::NullPointerException();
    if (o < 0 || l < 0 || o > a->length - l) throw ::jxx::lang::ArrayIndexOutOfBoundsException();
}
void Deflater::setInput(const ::jxx::lang::ByteArray& input) { if (input == nullptr) throw ::jxx::lang::NullPointerException(); setInput(input,0,input->length); }
void Deflater::setInput(const ::jxx::lang::ByteArray& input, ::jxx::lang::jint offset, ::jxx::lang::jint length) {
    ensureOpen_(); checkRange_(input,offset,length); input_=input; inputOffset_=offset; inputLength_=length;
}
void Deflater::setDictionary(const ::jxx::lang::ByteArray& dictionary) { if (dictionary == nullptr) throw ::jxx::lang::NullPointerException(); setDictionary(dictionary,0,dictionary->length); }
void Deflater::setDictionary(const ::jxx::lang::ByteArray& dictionary, ::jxx::lang::jint offset, ::jxx::lang::jint length) {
    ensureOpen_(); checkRange_(dictionary,offset,length);
    const auto result=::deflateSetDictionary(&stream_, reinterpret_cast<const Bytef*>(&(*dictionary)[offset]), static_cast<uInt>(length));
    if(result!=Z_OK) throwState("dictionary cannot be set in the current state");
}
void Deflater::setStrategy(::jxx::lang::jint strategy) {
    ensureOpen_(); if(strategy!=DEFAULT_STRATEGY && strategy!=FILTERED && strategy!=HUFFMAN_ONLY) throw ::jxx::lang::IllegalArgumentException(); strategy_=strategy; parametersDirty_=true;
}
void Deflater::setLevel(::jxx::lang::jint level) {
    ensureOpen_(); if(level<DEFAULT_COMPRESSION || level>BEST_COMPRESSION) throw ::jxx::lang::IllegalArgumentException(); level_=level; parametersDirty_=true;
}
::jxx::lang::jbool Deflater::needsInput() const noexcept { return inputLength_==0; }
void Deflater::finish() noexcept { finishRequested_=true; }
::jxx::lang::jbool Deflater::finished() const noexcept { return finished_; }
void Deflater::applyParameters_() {
    if(!parametersDirty_) return;
    const auto result=::deflateParams(&stream_,level_,strategy_);
    if(result!=Z_OK && result!=Z_BUF_ERROR) throwState("unable to apply compression parameters");
    parametersDirty_=false;
}
::jxx::lang::jint Deflater::deflate(const ::jxx::lang::ByteArray& output) { if(output==nullptr) throw ::jxx::lang::NullPointerException(); return deflate(output,0,output->length,NO_FLUSH); }
::jxx::lang::jint Deflater::deflate(const ::jxx::lang::ByteArray& output,::jxx::lang::jint offset,::jxx::lang::jint length) { return deflate(output,offset,length,NO_FLUSH); }
::jxx::lang::jint Deflater::deflate(const ::jxx::lang::ByteArray& output,::jxx::lang::jint offset,::jxx::lang::jint length,::jxx::lang::jint flush) {
    ensureOpen_(); checkRange_(output,offset,length);
    if(flush!=NO_FLUSH && flush!=SYNC_FLUSH && flush!=FULL_FLUSH) throw ::jxx::lang::IllegalArgumentException();
    applyParameters_();
    stream_.next_in=inputLength_==0?Z_NULL:reinterpret_cast<Bytef*>(&(*input_)[inputOffset_]); stream_.avail_in=static_cast<uInt>(inputLength_);
    stream_.next_out=length==0?Z_NULL:reinterpret_cast<Bytef*>(&(*output)[offset]); stream_.avail_out=static_cast<uInt>(length);
    const auto beforeIn=stream_.avail_in; const auto beforeOut=stream_.avail_out;
    const auto result=::deflate(&stream_,finishRequested_?Z_FINISH:flush);
    const auto consumed=beforeIn-stream_.avail_in; inputOffset_+=static_cast<::jxx::lang::jint>(consumed); inputLength_-=static_cast<::jxx::lang::jint>(consumed);
    if(inputLength_==0) input_.reset();
    if(result==Z_STREAM_END) finished_=true;
    else if(result!=Z_OK && result!=Z_BUF_ERROR) throwState(stream_.msg==nullptr?"compression failed":stream_.msg);
    return static_cast<::jxx::lang::jint>(beforeOut-stream_.avail_out);
}
::jxx::lang::jint Deflater::getAdler() const { ensureOpen_(); return static_cast<::jxx::lang::jint>(stream_.adler); }
::jxx::lang::jint Deflater::getTotalIn() const noexcept { return static_cast<::jxx::lang::jint>(stream_.total_in); }
::jxx::lang::jlong Deflater::getBytesRead() const noexcept { return static_cast<::jxx::lang::jlong>(stream_.total_in); }
::jxx::lang::jint Deflater::getTotalOut() const noexcept { return static_cast<::jxx::lang::jint>(stream_.total_out); }
::jxx::lang::jlong Deflater::getBytesWritten() const noexcept { return static_cast<::jxx::lang::jlong>(stream_.total_out); }
void Deflater::reset() {
    ensureOpen_(); if(::deflateReset(&stream_)!=Z_OK) throwState("unable to reset deflater"); input_.reset(); inputOffset_=inputLength_=0; finishRequested_=finished_=false; parametersDirty_=false;
}
void Deflater::end() noexcept { if(!ended_){::deflateEnd(&stream_); ended_=true; input_.reset(); inputOffset_=inputLength_=0;} }

} // namespace jxx::util::zip
