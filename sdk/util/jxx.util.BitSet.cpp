#include "util/jxx.util.BitSet.h"
#include <algorithm>
#include <sstream>
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
namespace jxx::util {
namespace { constexpr int BITS=64; }
BitSet::BitSet():Super(){} BitSet::BitSet(::jxx::lang::jint n):Super(){if(n<0)throw ::jxx::lang::IndexOutOfBoundsException();words_.resize((n+63)/64);}
void BitSet::checkIndex_(::jxx::lang::jint i){if(i<0)throw ::jxx::lang::IndexOutOfBoundsException();}
void BitSet::checkRange_(::jxx::lang::jint f,::jxx::lang::jint t){if(f<0||t<0||f>t)throw ::jxx::lang::IndexOutOfBoundsException();}
void BitSet::ensure_(::jxx::lang::jint i){auto n=static_cast<std::size_t>(i/BITS+1);if(words_.size()<n)words_.resize(n);}
void BitSet::trim_(){while(!words_.empty()&&words_.back()==0)words_.pop_back();}
void BitSet::set(::jxx::lang::jint i){checkIndex_(i);ensure_(i);words_[i/BITS]|=std::uint64_t{1}<<(i%BITS);}
void BitSet::set(::jxx::lang::jint i,::jxx::lang::jbool v){if(v)set(i);else clear(i);}
void BitSet::set(::jxx::lang::jint f,::jxx::lang::jint t){set(f,t,true);}
void BitSet::set(::jxx::lang::jint f,::jxx::lang::jint t,::jxx::lang::jbool v){checkRange_(f,t);for(auto i=f;i<t;++i)set(i,v);}
void BitSet::clear(::jxx::lang::jint i){checkIndex_(i);if(static_cast<std::size_t>(i/BITS)<words_.size())words_[i/BITS]&=~(std::uint64_t{1}<<(i%BITS));trim_();}
void BitSet::clear(::jxx::lang::jint f,::jxx::lang::jint t){checkRange_(f,t);for(auto i=f;i<t;++i)clear(i);}
void BitSet::clear(){words_.clear();}
void BitSet::flip(::jxx::lang::jint i){checkIndex_(i);ensure_(i);words_[i/BITS]^=std::uint64_t{1}<<(i%BITS);trim_();}
void BitSet::flip(::jxx::lang::jint f,::jxx::lang::jint t){checkRange_(f,t);for(auto i=f;i<t;++i)flip(i);}
::jxx::lang::jbool BitSet::get(::jxx::lang::jint i)const{checkIndex_(i);return static_cast<std::size_t>(i/BITS)<words_.size()&&(words_[i/BITS]&(std::uint64_t{1}<<(i%BITS)))!=0;}
::jxx::Ptr<BitSet> BitSet::get(::jxx::lang::jint f,::jxx::lang::jint t)const{checkRange_(f,t);auto r=::jxx::NEW<BitSet>();for(auto i=f;i<t;++i)if(get(i))r->set(i-f);return r;}
::jxx::lang::jint BitSet::nextSetBit(::jxx::lang::jint f)const{checkIndex_(f);for(auto i=f;i<length();++i)if(get(i))return i;return -1;}
::jxx::lang::jint BitSet::nextClearBit(::jxx::lang::jint f)const{checkIndex_(f);for(auto i=f;;++i)if(!get(i))return i;}
::jxx::lang::jint BitSet::previousSetBit(::jxx::lang::jint f)const{if(f<-1)throw ::jxx::lang::IndexOutOfBoundsException();for(auto i=f;i>=0;--i)if(get(i))return i;return -1;}
::jxx::lang::jint BitSet::previousClearBit(::jxx::lang::jint f)const{if(f<-1)throw ::jxx::lang::IndexOutOfBoundsException();for(auto i=f;i>=0;--i)if(!get(i))return i;return -1;}
::jxx::lang::jint BitSet::length()const{if(words_.empty())return 0;auto w=words_.back();int bits=0;while(w){++bits;w>>=1;}return static_cast<::jxx::lang::jint>((words_.size()-1)*64+bits);}
::jxx::lang::jbool BitSet::isEmpty()const{return length()==0;}
::jxx::lang::jint BitSet::cardinality()const{::jxx::lang::jint n=0;for(auto w:words_)while(w){w&=w-1;++n;}return n;}
::jxx::lang::jbool BitSet::intersects(const ::jxx::Ptr<BitSet>& v)const{if(!v)throw ::jxx::lang::NullPointerException();for(std::size_t i=0;i<std::min(words_.size(),v->words_.size());++i)if(words_[i]&v->words_[i])return true;return false;}
void BitSet::andSet(const ::jxx::Ptr<BitSet>&v){if(!v)throw ::jxx::lang::NullPointerException();for(std::size_t i=0;i<words_.size();++i)words_[i]&=i<v->words_.size()?v->words_[i]:0;trim_();}
void BitSet::orSet(const ::jxx::Ptr<BitSet>&v){if(!v)throw ::jxx::lang::NullPointerException();if(words_.size()<v->words_.size())words_.resize(v->words_.size());for(std::size_t i=0;i<v->words_.size();++i)words_[i]|=v->words_[i];}
void BitSet::xorSet(const ::jxx::Ptr<BitSet>&v){if(!v)throw ::jxx::lang::NullPointerException();if(words_.size()<v->words_.size())words_.resize(v->words_.size());for(std::size_t i=0;i<v->words_.size();++i)words_[i]^=v->words_[i];trim_();}
void BitSet::andNot(const ::jxx::Ptr<BitSet>&v){if(!v)throw ::jxx::lang::NullPointerException();for(std::size_t i=0;i<std::min(words_.size(),v->words_.size());++i)words_[i]&=~v->words_[i];trim_();}
::jxx::lang::jint BitSet::size()const{return static_cast<::jxx::lang::jint>(words_.size()*64);}
::jxx::lang::jbool BitSet::equals(const ::jxx::Ptr<::jxx::lang::Object>&o)const{auto v=::jxx::CAST<BitSet>(o);return v&&words_==v->words_;}
::jxx::lang::jint BitSet::hashCode()const{std::uint64_t h=1234;for(std::size_t i=words_.size();i>0;--i)h^=words_[i-1]*i;return static_cast<::jxx::lang::jint>((h>>32)^h);}
::jxx::Ptr<::jxx::lang::String> BitSet::toString()const{std::ostringstream s;s<<'{';bool first=true;for(auto i=nextSetBit(0);i>=0;i=nextSetBit(i+1)){if(!first)s<<", ";first=false;s<<i;}s<<'}';return ::jxx::NEW<::jxx::lang::String>(s.str());}
void BitSet::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&){throw ::jxx::lang::UnsupportedOperationException();}
void BitSet::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&){throw ::jxx::lang::UnsupportedOperationException();}
void BitSet::readObjectNoData(){throw ::jxx::lang::UnsupportedOperationException();}
::jxx::Ptr<::jxx::lang::Object> BitSet::cloneImpl()const{auto copy=::jxx::NEW<BitSet>();copy->words_=words_;return ::jxx::CAST<::jxx::lang::Object>(copy);}
} // namespace jxx::util
