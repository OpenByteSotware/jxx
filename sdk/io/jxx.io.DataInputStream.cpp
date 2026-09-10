#include "io/jxx.io.DataInputStream.h"
#include <cstring>
#include <string>
#include "io/jxx.io.EOFException.h"
#include "io/jxx.io.IOHelper.h"
#include "io/jxx.io.ModifiedUTF.h"
#include "lang/jxx.lang.String.h"
namespace jxx::io
{
	DataInputStream::DataInputStream(const ::jxx::Ptr<InputStream>& i) : Super(i)
	{
	} ::jxx::lang::jint DataInputStream::required()
	{
		auto v = read(); if (v < 0)throw EOFException(); return v;
	} void DataInputStream::readFully(const ::jxx::lang::ByteArray& b)
	{
		readFully(b, 0, static_cast<::jxx::lang::jint>(b->length));
	} void DataInputStream::readFully(const ::jxx::lang::ByteArray& b, ::jxx::lang::jint o, ::jxx::lang::jint l)
	{
		IOHelper::checkBounds(b, o, l); while (l > 0) {
			auto n = read(b, o, l); if (n < 0)throw EOFException(); o += n; l -= n;
		}
	} ::jxx::lang::jint DataInputStream::skipBytes(::jxx::lang::jint n)
	{
		::jxx::lang::jint total = 0; while (total < n) {
			auto k = skip(n - total); if (k <= 0)break; total += static_cast<::jxx::lang::jint>(k);
		}return total;
	} ::jxx::lang::jbool DataInputStream::readBoolean()
	{
		return required() != 0;
	} ::jxx::lang::jbyte DataInputStream::readByte()
	{
		return static_cast<::jxx::lang::jbyte>(required());
	} ::jxx::lang::jint DataInputStream::readUnsignedByte()
	{
		return required();
	} ::jxx::lang::jshort DataInputStream::readShort()
	{
		return static_cast<::jxx::lang::jshort>((required() << 8) | required());
	} ::jxx::lang::jint DataInputStream::readUnsignedShort()
	{
		return (required() << 8) | required();
	} ::jxx::lang::jchar DataInputStream::readChar()
	{
		return static_cast<::jxx::lang::jchar>(readUnsignedShort());
	} ::jxx::lang::jint DataInputStream::readInt()
	{
		return static_cast<::jxx::lang::jint>((static_cast<std::uint32_t>(required()) << 24) | (static_cast<std::uint32_t>(required()) << 16) | (static_cast<std::uint32_t>(required()) << 8) | required());
	} ::jxx::lang::jlong DataInputStream::readLong()
	{
		auto hi = static_cast<std::uint32_t>(readInt()); auto lo = static_cast<std::uint32_t>(readInt()); return static_cast<::jxx::lang::jlong>((static_cast<std::uint64_t>(hi) << 32) | lo);
	} ::jxx::lang::jfloat DataInputStream::readFloat()
	{
		auto n = static_cast<std::uint32_t>(readInt()); ::jxx::lang::jfloat v; std::memcpy(&v, &n, sizeof(v)); return v;
	} ::jxx::lang::jdouble DataInputStream::readDouble()
	{
		auto n = static_cast<std::uint64_t>(readLong()); ::jxx::lang::jdouble v; std::memcpy(&v, &n, sizeof(v)); return v;
	} ::jxx::Ptr<::jxx::lang::String> DataInputStream::readLine()
	{
		std::string s; for (;;) {
			auto c = read(); if (c < 0)return s.empty() ? nullptr : jxx::NEW<::jxx::lang::String>(s); if (c == '\n')break; if (c == '\r')break; s.push_back(static_cast<char>(c));
		}return ::jxx::NEW<::jxx::lang::String>(s);
	} ::jxx::Ptr<::jxx::lang::String> DataInputStream::readUTF()
	{
		return readUTF(::jxx::CAST<DataInput>(thisPtr()));
	} ::jxx::Ptr<::jxx::lang::String> DataInputStream::readUTF(const ::jxx::Ptr<DataInput>& i)
	{
		auto size = i->readUnsignedShort(); auto b = ::jxx::NEW<::jxx::lang::ByteArrayType>(static_cast<std::uint32_t>(size)); i->readFully(b); return ModifiedUTF::decode(b);
	}
}
