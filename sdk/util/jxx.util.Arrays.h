#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_set>

#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.ComparatorSuper.h"

namespace jxx::util {

template <typename E> class List;
template <typename E> class Spliterator;
namespace function {
template <typename T> class BinaryOperator;
class IntBinaryOperator;
class LongBinaryOperator;
class DoubleBinaryOperator;
template <typename T> class IntFunction;
class IntUnaryOperator;
class IntToLongFunction;
class IntToDoubleFunction;
}
namespace stream {
template <typename T> class Stream;
class IntStream;
class LongStream;
class DoubleStream;
}

/**
 * Java 8 java.util.Arrays for JXX/C++17.
 *
 * The class is deliberately non-template. Java-generic static methods are
 * represented by C++ function templates. Public array APIs use JxxArray.
 *
 * Core Java 8 operations are implemented here. Methods requiring the JXX
 * List/Spliterator/Stream/function layer are declared when
 * JXX_ARRAYS_ENABLE_COLLECTION_STREAM_API is defined and are implemented by
 * jxx.util.Arrays.adapters.h against the runtime's exact interfaces.
 */
class Arrays final {
private:
    Arrays() = delete;
    ~Arrays() = delete;

    template <typename T>
    using Array = jxx::Ptr<jxx::lang::JxxArray<T, 1U>>;

    template <typename T>
    using RefArray = jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<T>, 1U>>;

    template <typename T>
    static void require_(const Array<T>& a) {
        if (a == nullptr) throw jxx::lang::NullPointerException();
    }

    template <typename T>
    static jxx::lang::jint len_(const Array<T>& a) {
        require_(a);
        return static_cast<jxx::lang::jint>(a->length);
    }

    template <typename T>
    static void range_(const Array<T>& a, jxx::lang::jint from, jxx::lang::jint to) {
        const auto n = len_(a);
        if (from > to) throw jxx::lang::IllegalArgumentException();
        if (from < 0 || to > n) throw jxx::lang::ArrayIndexOutOfBoundsException();
    }

    static jxx::lang::jint wrap_(std::uint32_t v) noexcept {
        return static_cast<jxx::lang::jint>(static_cast<std::int32_t>(v));
    }

    static std::uint32_t floatBits_(jxx::lang::jfloat v) noexcept {
        if (std::isnan(v)) return 0x7fc00000U;
        std::uint32_t b{}; std::memcpy(&b, &v, sizeof(b)); return b;
    }

    static std::uint64_t doubleBits_(jxx::lang::jdouble v) noexcept {
        if (std::isnan(v)) return 0x7ff8000000000000ULL;
        std::uint64_t b{}; std::memcpy(&b, &v, sizeof(b)); return b;
    }

    static jxx::lang::jint cmpFloat_(jxx::lang::jfloat a, jxx::lang::jfloat b) noexcept {
        if (a < b) return -1; if (a > b) return 1;
        const auto x=floatBits_(a), y=floatBits_(b);
        if (x==y) return 0;
        return static_cast<std::int32_t>(x) < static_cast<std::int32_t>(y) ? -1 : 1;
    }

    static jxx::lang::jint cmpDouble_(jxx::lang::jdouble a, jxx::lang::jdouble b) noexcept {
        if (a < b) return -1; if (a > b) return 1;
        const auto x=doubleBits_(a), y=doubleBits_(b);
        if (x==y) return 0;
        return static_cast<std::int64_t>(x) < static_cast<std::int64_t>(y) ? -1 : 1;
    }

    template <typename T>
    static jxx::lang::jbool eq_(const Array<T>& a, const Array<T>& b) {
        if (a==b) return true;
        if (a==nullptr || b==nullptr || a->length!=b->length) return false;
        for (jxx::lang::jint i=0;i<len_(a);++i) if ((*a)[i]!=(*b)[i]) return false;
        return true;
    }

    template <typename T>
    static Array<T> copy_(const Array<T>& a, jxx::lang::jint n) {
        require_(a); if (n<0) throw jxx::lang::NegativeArraySizeException();
        using A=jxx::lang::JxxArray<T,1U>;
        auto r=jxx::NEW<A>(static_cast<typename A::size_type>(n));
        const auto m=std::min(len_(a),n);
        for(jxx::lang::jint i=0;i<m;++i) (*r)[i]=(*a)[i];
        return r;
    }

    template <typename T>
    static Array<T> rangeCopy_(const Array<T>& a, jxx::lang::jint from, jxx::lang::jint to) {
        require_(a);
        if(from>to) throw jxx::lang::IllegalArgumentException();
        if(from<0 || from>len_(a)) throw jxx::lang::ArrayIndexOutOfBoundsException();
        const auto n=to-from;
        using A=jxx::lang::JxxArray<T,1U>;
        auto r=jxx::NEW<A>(static_cast<typename A::size_type>(n));
        const auto m=std::min(len_(a)-from,n);
        for(jxx::lang::jint i=0;i<m;++i) (*r)[i]=(*a)[from+i];
        return r;
    }

    template <typename T>
    static void fill_(const Array<T>& a,jxx::lang::jint from,jxx::lang::jint to,const T& v) {
        range_(a,from,to); std::fill(a->begin()+from,a->begin()+to,v);
    }

    template <typename T>
    static void sort_(const Array<T>& a,jxx::lang::jint from,jxx::lang::jint to) {
        range_(a,from,to); std::sort(a->begin()+from,a->begin()+to);
    }

    template <typename T, typename Compare>
    static jxx::lang::jint search_(const Array<T>& a,jxx::lang::jint from,jxx::lang::jint to,const T& key,Compare cmp) {
        range_(a,from,to); jxx::lang::jint low=from, high=to-1;
        while(low<=high){ const auto mid=low+((high-low)>>1); const auto c=cmp((*a)[mid],key);
            if(c<0) low=mid+1; else if(c>0) high=mid-1; else return mid; }
        return -(low+1);
    }

    template <typename T>
    static jxx::lang::jint refCompare_(const jxx::Ptr<T>& a,const jxx::Ptr<T>& b,const jxx::Ptr<ComparatorSuper<T>>& c) {
        if(c!=nullptr) return c->compareSuper(a,b);
        if(a==nullptr || b==nullptr) throw jxx::lang::NullPointerException();
        auto x=jxx::CAST<jxx::lang::Comparable<T>>(a);
        if(x==nullptr) throw jxx::lang::ClassCastException();
        return x->compareTo(b);
    }

    template <typename T>
    static jxx::Ptr<jxx::lang::String> numericString_(const Array<T>& a) {
        if(a==nullptr) return jxx::NEW<jxx::lang::String>("null");
        std::ostringstream s; s<<"[";
        for(jxx::lang::jint i=0;i<len_(a);++i){ if(i) s<<", "; s<<(*a)[i]; }
        s<<"]"; return jxx::NEW<jxx::lang::String>(s.str());
    }

public:
#define JXX_ARRAYS_EQ(ALIAS) static jxx::lang::jbool equals(ALIAS a,ALIAS b){return eq_(a,b);}
    JXX_ARRAYS_EQ(jxx::lang::BooleanArray)
    JXX_ARRAYS_EQ(jxx::lang::ByteArray)
    JXX_ARRAYS_EQ(jxx::lang::CharArray)
    JXX_ARRAYS_EQ(jxx::lang::ShortArray)
    JXX_ARRAYS_EQ(jxx::lang::IntArray)
    JXX_ARRAYS_EQ(jxx::lang::LongArray)
#undef JXX_ARRAYS_EQ

    static jxx::lang::jbool equals(jxx::lang::FloatArray a,jxx::lang::FloatArray b){
        if(a==b)return true;if(a==nullptr||b==nullptr||a->length!=b->length)return false;
        for(jxx::lang::jint i=0;i<len_(a);++i)if(floatBits_((*a)[i])!=floatBits_((*b)[i]))return false;return true;}
    static jxx::lang::jbool equals(jxx::lang::DoubleArray a,jxx::lang::DoubleArray b){
        if(a==b)return true;if(a==nullptr||b==nullptr||a->length!=b->length)return false;
        for(jxx::lang::jint i=0;i<len_(a);++i)if(doubleBits_((*a)[i])!=doubleBits_((*b)[i]))return false;return true;}
    static jxx::lang::jbool equals(jxx::lang::ObjectArray a,jxx::lang::ObjectArray b){
        if(a==b)return true;if(a==nullptr||b==nullptr||a->length!=b->length)return false;
        for(jxx::lang::jint i=0;i<len_(a);++i){auto x=(*a)[i],y=(*b)[i];if(x==nullptr||y==nullptr){if(x!=y)return false;}else if(!x->equals(y))return false;}return true;}
    template<typename T> static jxx::lang::jbool equals(RefArray<T> a,RefArray<T> b){
        if(a==b)return true;if(a==nullptr||b==nullptr||a->length!=b->length)return false;
        for(jxx::lang::jint i=0;i<len_(a);++i){auto x=jxx::CAST<jxx::lang::Object>((*a)[i]);auto y=jxx::CAST<jxx::lang::Object>((*b)[i]);if(x==nullptr||y==nullptr){if(x!=y)return false;}else if(!x->equals(y))return false;}return true;}

    static jxx::lang::jint hashCode(jxx::lang::BooleanArray a){if(a==nullptr)return 0;std::uint32_t h=1;for(auto v:*a)h=31U*h+(v?1231U:1237U);return wrap_(h);}
    static jxx::lang::jint hashCode(jxx::lang::ByteArray a){if(a==nullptr)return 0;std::uint32_t h=1;for(auto v:*a)h=31U*h+static_cast<std::uint32_t>(static_cast<std::int32_t>(v));return wrap_(h);}
    static jxx::lang::jint hashCode(jxx::lang::CharArray a){if(a==nullptr)return 0;std::uint32_t h=1;for(auto v:*a)h=31U*h+static_cast<std::uint32_t>(v);return wrap_(h);}
    static jxx::lang::jint hashCode(jxx::lang::ShortArray a){if(a==nullptr)return 0;std::uint32_t h=1;for(auto v:*a)h=31U*h+static_cast<std::uint32_t>(static_cast<std::int32_t>(v));return wrap_(h);}
    static jxx::lang::jint hashCode(jxx::lang::IntArray a){if(a==nullptr)return 0;std::uint32_t h=1;for(auto v:*a)h=31U*h+static_cast<std::uint32_t>(v);return wrap_(h);}
    static jxx::lang::jint hashCode(jxx::lang::LongArray a){if(a==nullptr)return 0;std::uint32_t h=1;for(auto v:*a){auto x=static_cast<std::uint64_t>(v);h=31U*h+static_cast<std::uint32_t>(x^(x>>32));}return wrap_(h);}
    static jxx::lang::jint hashCode(jxx::lang::FloatArray a){if(a==nullptr)return 0;std::uint32_t h=1;for(auto v:*a)h=31U*h+floatBits_(v);return wrap_(h);}
    static jxx::lang::jint hashCode(jxx::lang::DoubleArray a){if(a==nullptr)return 0;std::uint32_t h=1;for(auto v:*a){auto x=doubleBits_(v);h=31U*h+static_cast<std::uint32_t>(x^(x>>32));}return wrap_(h);}
    static jxx::lang::jint hashCode(jxx::lang::ObjectArray a){if(a==nullptr)return 0;std::uint32_t h=1;for(const auto&v:*a)h=31U*h+static_cast<std::uint32_t>(v==nullptr?0:v->hashCode());return wrap_(h);}
    template<typename T> static jxx::lang::jint hashCode(RefArray<T>a){if(a==nullptr)return 0;std::uint32_t h=1;for(const auto&v:*a){auto o=jxx::CAST<jxx::lang::Object>(v);h=31U*h+static_cast<std::uint32_t>(o==nullptr?0:o->hashCode());}return wrap_(h);}

#define JXX_ARRAYS_COPY(ALIAS) \
    static ALIAS copyOf(ALIAS a,jxx::lang::jint n){return copy_(a,n);} \
    static ALIAS copyOfRange(ALIAS a,jxx::lang::jint f,jxx::lang::jint t){return rangeCopy_(a,f,t);}
    JXX_ARRAYS_COPY(jxx::lang::BooleanArray)
    JXX_ARRAYS_COPY(jxx::lang::ByteArray)
    JXX_ARRAYS_COPY(jxx::lang::CharArray)
    JXX_ARRAYS_COPY(jxx::lang::ShortArray)
    JXX_ARRAYS_COPY(jxx::lang::IntArray)
    JXX_ARRAYS_COPY(jxx::lang::LongArray)
    JXX_ARRAYS_COPY(jxx::lang::FloatArray)
    JXX_ARRAYS_COPY(jxx::lang::DoubleArray)
    JXX_ARRAYS_COPY(jxx::lang::ObjectArray)
#undef JXX_ARRAYS_COPY
    template<typename T> static RefArray<T> copyOf(RefArray<T>a,jxx::lang::jint n){return copy_(a,n);}
    template<typename T> static RefArray<T> copyOfRange(RefArray<T>a,jxx::lang::jint f,jxx::lang::jint t){return rangeCopy_(a,f,t);}

#define JXX_ARRAYS_FILL(ALIAS,TYPE) \
    static void fill(ALIAS a,TYPE v){fill_(a,0,len_(a),v);} \
    static void fill(ALIAS a,jxx::lang::jint f,jxx::lang::jint t,TYPE v){fill_(a,f,t,v);}
    JXX_ARRAYS_FILL(jxx::lang::BooleanArray,jxx::lang::jbool)
    JXX_ARRAYS_FILL(jxx::lang::ByteArray,jxx::lang::jbyte)
    JXX_ARRAYS_FILL(jxx::lang::CharArray,jxx::lang::jchar)
    JXX_ARRAYS_FILL(jxx::lang::ShortArray,jxx::lang::jshort)
    JXX_ARRAYS_FILL(jxx::lang::IntArray,jxx::lang::jint)
    JXX_ARRAYS_FILL(jxx::lang::LongArray,jxx::lang::jlong)
    JXX_ARRAYS_FILL(jxx::lang::FloatArray,jxx::lang::jfloat)
    JXX_ARRAYS_FILL(jxx::lang::DoubleArray,jxx::lang::jdouble)
    JXX_ARRAYS_FILL(jxx::lang::ObjectArray,jxx::Ptr<jxx::lang::Object>)
#undef JXX_ARRAYS_FILL
    template<typename T> static void fill(RefArray<T>a,jxx::Ptr<T>v){fill_(a,0,len_(a),v);}
    template<typename T> static void fill(RefArray<T>a,jxx::lang::jint f,jxx::lang::jint t,jxx::Ptr<T>v){fill_(a,f,t,v);}

#define JXX_ARRAYS_SORT(ALIAS) \
    static void sort(ALIAS a){sort_(a,0,len_(a));} \
    static void sort(ALIAS a,jxx::lang::jint f,jxx::lang::jint t){sort_(a,f,t);} \
    static void parallelSort(ALIAS a){sort(a);} \
    static void parallelSort(ALIAS a,jxx::lang::jint f,jxx::lang::jint t){sort(a,f,t);}
    JXX_ARRAYS_SORT(jxx::lang::ByteArray)
    JXX_ARRAYS_SORT(jxx::lang::CharArray)
    JXX_ARRAYS_SORT(jxx::lang::ShortArray)
    JXX_ARRAYS_SORT(jxx::lang::IntArray)
    JXX_ARRAYS_SORT(jxx::lang::LongArray)
#undef JXX_ARRAYS_SORT

    static void sort(jxx::lang::FloatArray a){sort(a,0,len_(a));}
    static void sort(jxx::lang::FloatArray a,jxx::lang::jint f,jxx::lang::jint t){range_(a,f,t);std::sort(a->begin()+f,a->begin()+t,[](auto x,auto y){return cmpFloat_(x,y)<0;});}
    static void sort(jxx::lang::DoubleArray a){sort(a,0,len_(a));}
    static void sort(jxx::lang::DoubleArray a,jxx::lang::jint f,jxx::lang::jint t){range_(a,f,t);std::sort(a->begin()+f,a->begin()+t,[](auto x,auto y){return cmpDouble_(x,y)<0;});}
    static void parallelSort(jxx::lang::FloatArray a){sort(a);} static void parallelSort(jxx::lang::FloatArray a,jxx::lang::jint f,jxx::lang::jint t){sort(a,f,t);}
    static void parallelSort(jxx::lang::DoubleArray a){sort(a);} static void parallelSort(jxx::lang::DoubleArray a,jxx::lang::jint f,jxx::lang::jint t){sort(a,f,t);}

    template<typename T> static void sort(RefArray<T>a){sort(a,0,len_(a),nullptr);}
    template<typename T> static void sort(RefArray<T>a,jxx::lang::jint f,jxx::lang::jint t){sort(a,f,t,nullptr);}
    template<typename T> static void sort(RefArray<T>a,jxx::Ptr<ComparatorSuper<T>>c){sort(a,0,len_(a),c);}
    template<typename T> static void sort(RefArray<T>a,jxx::lang::jint f,jxx::lang::jint t,jxx::Ptr<ComparatorSuper<T>>c){range_(a,f,t);std::stable_sort(a->begin()+f,a->begin()+t,[&](const auto&x,const auto&y){return refCompare_(x,y,c)<0;});}
    template<typename T> static void parallelSort(RefArray<T>a){sort(a);} template<typename T> static void parallelSort(RefArray<T>a,jxx::Ptr<ComparatorSuper<T>>c){sort(a,c);}
    template<typename T> static void parallelSort(RefArray<T>a,jxx::lang::jint f,jxx::lang::jint t){sort(a,f,t);}
    template<typename T> static void parallelSort(RefArray<T>a,jxx::lang::jint f,jxx::lang::jint t,jxx::Ptr<ComparatorSuper<T>>c){sort(a,f,t,c);}

#define JXX_ARRAYS_SEARCH(ALIAS,TYPE) \
    static jxx::lang::jint binarySearch(ALIAS a,TYPE k){return search_(a,0,len_(a),k,[](auto x,auto y){return x<y?-1:(x>y?1:0);});} \
    static jxx::lang::jint binarySearch(ALIAS a,jxx::lang::jint f,jxx::lang::jint t,TYPE k){return search_(a,f,t,k,[](auto x,auto y){return x<y?-1:(x>y?1:0);});}
    JXX_ARRAYS_SEARCH(jxx::lang::ByteArray,jxx::lang::jbyte)
    JXX_ARRAYS_SEARCH(jxx::lang::CharArray,jxx::lang::jchar)
    JXX_ARRAYS_SEARCH(jxx::lang::ShortArray,jxx::lang::jshort)
    JXX_ARRAYS_SEARCH(jxx::lang::IntArray,jxx::lang::jint)
    JXX_ARRAYS_SEARCH(jxx::lang::LongArray,jxx::lang::jlong)
#undef JXX_ARRAYS_SEARCH
    static jxx::lang::jint binarySearch(jxx::lang::FloatArray a,jxx::lang::jfloat k){return search_(a,0,len_(a),k,cmpFloat_);}
    static jxx::lang::jint binarySearch(jxx::lang::FloatArray a,jxx::lang::jint f,jxx::lang::jint t,jxx::lang::jfloat k){return search_(a,f,t,k,cmpFloat_);}
    static jxx::lang::jint binarySearch(jxx::lang::DoubleArray a,jxx::lang::jdouble k){return search_(a,0,len_(a),k,cmpDouble_);}
    static jxx::lang::jint binarySearch(jxx::lang::DoubleArray a,jxx::lang::jint f,jxx::lang::jint t,jxx::lang::jdouble k){return search_(a,f,t,k,cmpDouble_);}
    template<typename T> static jxx::lang::jint binarySearch(RefArray<T>a,jxx::Ptr<T>k){return binarySearch(a,0,len_(a),k,nullptr);}
    template<typename T> static jxx::lang::jint binarySearch(RefArray<T>a,jxx::lang::jint f,jxx::lang::jint t,jxx::Ptr<T>k){return binarySearch(a,f,t,k,nullptr);}
    template<typename T> static jxx::lang::jint binarySearch(RefArray<T>a,jxx::Ptr<T>k,jxx::Ptr<ComparatorSuper<T>>c){return binarySearch(a,0,len_(a),k,c);}
    template<typename T> static jxx::lang::jint binarySearch(RefArray<T>a,jxx::lang::jint f,jxx::lang::jint t,jxx::Ptr<T>k,jxx::Ptr<ComparatorSuper<T>>c){return search_(a,f,t,k,[&](const auto&x,const auto&y){return refCompare_(x,y,c);});}

    static jxx::Ptr<jxx::lang::String> toString(jxx::lang::BooleanArray a){if(a==nullptr)return jxx::NEW<jxx::lang::String>("null");std::ostringstream s;s<<"[";for(jxx::lang::jint i=0;i<len_(a);++i){if(i)s<<", ";s<<((*a)[i]?"true":"false");}s<<"]";return jxx::NEW<jxx::lang::String>(s.str());}
    static jxx::Ptr<jxx::lang::String> toString(jxx::lang::ByteArray a){if(a==nullptr)return jxx::NEW<jxx::lang::String>("null");std::ostringstream s;s<<"[";for(jxx::lang::jint i=0;i<len_(a);++i){if(i)s<<", ";s<<static_cast<jxx::lang::jint>((*a)[i]);}s<<"]";return jxx::NEW<jxx::lang::String>(s.str());}
    static jxx::Ptr<jxx::lang::String> toString(jxx::lang::CharArray a){if(a==nullptr)return jxx::NEW<jxx::lang::String>("null");std::u16string s=u"[";for(jxx::lang::jint i=0;i<len_(a);++i){if(i)s+=u", ";s.push_back(static_cast<char16_t>((*a)[i]));}s+=u"]";return jxx::NEW<jxx::lang::String>(s);}
    static jxx::Ptr<jxx::lang::String> toString(jxx::lang::ShortArray a){return numericString_(a);} static jxx::Ptr<jxx::lang::String> toString(jxx::lang::IntArray a){return numericString_(a);} static jxx::Ptr<jxx::lang::String> toString(jxx::lang::LongArray a){return numericString_(a);} static jxx::Ptr<jxx::lang::String> toString(jxx::lang::FloatArray a){return numericString_(a);} static jxx::Ptr<jxx::lang::String> toString(jxx::lang::DoubleArray a){return numericString_(a);}
    static jxx::Ptr<jxx::lang::String> toString(jxx::lang::ObjectArray a){if(a==nullptr)return jxx::NEW<jxx::lang::String>("null");std::ostringstream s;s<<"[";for(jxx::lang::jint i=0;i<len_(a);++i){if(i)s<<", ";auto v=(*a)[i];if(v==nullptr)s<<"null";else{auto t=v->toString();s<<(t==nullptr?"null":t->utf8());}}s<<"]";return jxx::NEW<jxx::lang::String>(s.str());}
    template<typename T> static jxx::Ptr<jxx::lang::String> toString(RefArray<T>a){if(a==nullptr)return jxx::NEW<jxx::lang::String>("null");std::ostringstream s;s<<"[";for(jxx::lang::jint i=0;i<len_(a);++i){if(i)s<<", ";auto v=jxx::CAST<jxx::lang::Object>((*a)[i]);if(v==nullptr)s<<"null";else{auto t=v->toString();s<<(t==nullptr?"null":t->utf8());}}s<<"]";return jxx::NEW<jxx::lang::String>(s.str());}

    // Deep operations for the named rank-2/rank-3 JXX aliases.
#define JXX_ARRAYS_DEEP2(ALIAS2) \
    static jxx::lang::jbool deepEquals(ALIAS2 a,ALIAS2 b){if(a==b)return true;if(a==nullptr||b==nullptr||a->length!=b->length)return false;for(jxx::lang::jint i=0;i<static_cast<jxx::lang::jint>(a->length);++i)if(!equals((*a)[i].ptr(),(*b)[i].ptr()))return false;return true;}
    JXX_ARRAYS_DEEP2(jxx::lang::BooleanArray2D)
    JXX_ARRAYS_DEEP2(jxx::lang::ByteArray2D)
    JXX_ARRAYS_DEEP2(jxx::lang::CharArray2D)
    JXX_ARRAYS_DEEP2(jxx::lang::ShortArray2D)
    JXX_ARRAYS_DEEP2(jxx::lang::IntArray2D)
    JXX_ARRAYS_DEEP2(jxx::lang::LongArray2D)
    JXX_ARRAYS_DEEP2(jxx::lang::FloatArray2D)
    JXX_ARRAYS_DEEP2(jxx::lang::DoubleArray2D)
#undef JXX_ARRAYS_DEEP2

#define JXX_ARRAYS_DEEP_HASH2(ALIAS2) \
    static jxx::lang::jint deepHashCode(ALIAS2 a){if(a==nullptr)return 0;std::uint32_t h=1;for(jxx::lang::jint i=0;i<static_cast<jxx::lang::jint>(a->length);++i)h=31U*h+static_cast<std::uint32_t>(hashCode((*a)[i].ptr()));return wrap_(h);}
    JXX_ARRAYS_DEEP_HASH2(jxx::lang::BooleanArray2D)
    JXX_ARRAYS_DEEP_HASH2(jxx::lang::ByteArray2D)
    JXX_ARRAYS_DEEP_HASH2(jxx::lang::CharArray2D)
    JXX_ARRAYS_DEEP_HASH2(jxx::lang::ShortArray2D)
    JXX_ARRAYS_DEEP_HASH2(jxx::lang::IntArray2D)
    JXX_ARRAYS_DEEP_HASH2(jxx::lang::LongArray2D)
    JXX_ARRAYS_DEEP_HASH2(jxx::lang::FloatArray2D)
    JXX_ARRAYS_DEEP_HASH2(jxx::lang::DoubleArray2D)
#undef JXX_ARRAYS_DEEP_HASH2

#ifdef JXX_ARRAYS_ENABLE_COLLECTION_STREAM_API
    template<typename T> static jxx::Ptr<List<T>> asList(RefArray<T> a);
    template<typename T> static jxx::Ptr<Spliterator<T>> spliterator(RefArray<T> a);
    template<typename T> static jxx::Ptr<Spliterator<T>> spliterator(RefArray<T> a,jxx::lang::jint start,jxx::lang::jint end);
    template<typename T> static jxx::Ptr<stream::Stream<T>> stream(RefArray<T> a);
    template<typename T> static jxx::Ptr<stream::Stream<T>> stream(RefArray<T> a,jxx::lang::jint start,jxx::lang::jint end);
    static jxx::Ptr<stream::IntStream> stream(jxx::lang::IntArray a);
    static jxx::Ptr<stream::IntStream> stream(jxx::lang::IntArray a,jxx::lang::jint start,jxx::lang::jint end);
    static jxx::Ptr<stream::LongStream> stream(jxx::lang::LongArray a);
    static jxx::Ptr<stream::LongStream> stream(jxx::lang::LongArray a,jxx::lang::jint start,jxx::lang::jint end);
    static jxx::Ptr<stream::DoubleStream> stream(jxx::lang::DoubleArray a);
    static jxx::Ptr<stream::DoubleStream> stream(jxx::lang::DoubleArray a,jxx::lang::jint start,jxx::lang::jint end);

    static void parallelPrefix(jxx::lang::IntArray a,jxx::Ptr<function::IntBinaryOperator> op);
    static void parallelPrefix(jxx::lang::IntArray a,jxx::lang::jint from,jxx::lang::jint to,jxx::Ptr<function::IntBinaryOperator> op);
    static void parallelPrefix(jxx::lang::LongArray a,jxx::Ptr<function::LongBinaryOperator> op);
    static void parallelPrefix(jxx::lang::LongArray a,jxx::lang::jint from,jxx::lang::jint to,jxx::Ptr<function::LongBinaryOperator> op);
    static void parallelPrefix(jxx::lang::DoubleArray a,jxx::Ptr<function::DoubleBinaryOperator> op);
    static void parallelPrefix(jxx::lang::DoubleArray a,jxx::lang::jint from,jxx::lang::jint to,jxx::Ptr<function::DoubleBinaryOperator> op);
    template<typename T> static void parallelPrefix(RefArray<T>a,jxx::Ptr<function::BinaryOperator<T>>op);
    template<typename T> static void parallelPrefix(RefArray<T>a,jxx::lang::jint from,jxx::lang::jint to,jxx::Ptr<function::BinaryOperator<T>>op);

    static void setAll(jxx::lang::IntArray a,jxx::Ptr<function::IntUnaryOperator> g);
    static void setAll(jxx::lang::LongArray a,jxx::Ptr<function::IntToLongFunction> g);
    static void setAll(jxx::lang::DoubleArray a,jxx::Ptr<function::IntToDoubleFunction> g);
    template<typename T> static void setAll(RefArray<T>a,jxx::Ptr<function::IntFunction<T>>g);
    static void parallelSetAll(jxx::lang::IntArray a,jxx::Ptr<function::IntUnaryOperator> g);
    static void parallelSetAll(jxx::lang::LongArray a,jxx::Ptr<function::IntToLongFunction> g);
    static void parallelSetAll(jxx::lang::DoubleArray a,jxx::Ptr<function::IntToDoubleFunction> g);
    template<typename T> static void parallelSetAll(RefArray<T>a,jxx::Ptr<function::IntFunction<T>>g);
#endif
};

} // namespace jxx::util
