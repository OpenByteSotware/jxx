#include <mutex>
#include "io/jxx.io.ClassInitialization.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.BufferedInputStream.h"
#include "io/jxx.io.BufferedOutputStream.h"
#include "io/jxx.io.BufferedReader.h"
#include "io/jxx.io.BufferedWriter.h"
#include "io/jxx.io.Console.h"
#include "io/jxx.io.DataInputStream.h"
#include "io/jxx.io.DataOutputStream.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.RandomAccessFile.h"
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.FileReader.h"
#include "io/jxx.io.FileWriter.h"
#include "io/jxx.io.InputStreamReader.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.OutputStreamWriter.h"
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.PrintWriter.h"
#include "io/jxx.io.PushbackInputStream.h"
#include "io/jxx.io.SequenceInputStream.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.AutoCloseable.h"
#include "io/jxx.io.CharConversionException.h"
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.DataInput.h"
#include "io/jxx.io.DataOutput.h"
#include "io/jxx.io.EOFException.h"
#include "io/jxx.io.Externalizable.h"
#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.FilterInputStream.h"
#include "io/jxx.io.FilterOutputStream.h"
#include "io/jxx.io.FilterReader.h"
#include "io/jxx.io.Flushable.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.InterruptedIOException.h"
#include "io/jxx.io.InvalidClassException.h"
#include "io/jxx.io.InvalidObjectException.h"
#include "io/jxx.io.IOError.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.NotActiveException.h"
#include "io/jxx.io.NotSerializableException.h"
#include "io/jxx.io.ObjectInput.h"
#include "io/jxx.io.ObjectOutput.h"
#include "io/jxx.io.ObjectStreamException.h"
#include "io/jxx.io.OptionalDataException.h"
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.PipedInputStream.h"
#include "io/jxx.io.PipedOutputStream.h"
#include "io/jxx.io.PushbackReader.h"
#include "io/jxx.io.Reader.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.StreamCorruptedException.h"
#include "io/jxx.io.SyncFailedException.h"
#include "io/jxx.io.UncheckedIOException.h"
#include "io/jxx.io.UnsupportedEncodingException.h"
#include "io/jxx.io.UTFDataFormatException.h"
#include "io/jxx.io.WriteAbortedException.h"
#include "io/jxx.io.Writer.h"
namespace jxx::io
{
	namespace
	{
		template<typename T> void reg()
		{
			::jxx::lang::class_info_detail::registerClassInfo<T>();
		}
	} void initializeClasses()
	{
		static std::once_flag once; std::call_once(once, []
	   {
				  reg<ByteArrayInputStream>(); 
				  reg<ByteArrayOutputStream>(); 
				  reg<BufferedInputStream>(); reg<BufferedOutputStream>();
				  reg<BufferedReader>(); reg<BufferedWriter>(); reg<Console>();
				  reg<DataInputStream>(); reg<DataOutputStream>(); reg<File>();
                  ::jxx::io::serialization::registerSerializableClass<File>(
                      static_cast<::jxx::lang::jlong>(301077366599181567LL),
                      [] { return ::jxx::NEW<File>(::jxx::NEW<::jxx::lang::String>()); });
                  ::jxx::io::serialization::registerClassReferenceArrayCodecs<File, 1U, 2U>(); 
				  reg<FileInputStream>(); reg<FileOutputStream>(); reg<FileReader>(); reg<RandomAccessFile>(); 
				  reg<FileWriter>(); reg<InputStreamReader>(); reg<ObjectInputStream>();
				  reg<ObjectOutputStream>(); reg<OutputStreamWriter>(); reg<PrintStream>();
				  reg<PrintWriter>(); reg<PushbackInputStream>(); reg<SequenceInputStream>();
				  reg<StringReader>();
        reg<AutoCloseable>();
        reg<CharConversionException>();
        reg<Closeable>();
        reg<DataInput>();
        reg<DataOutput>();
        reg<EOFException>();
        reg<Externalizable>();
        reg<FileDescriptor>();
        reg<FileNotFoundException>();
        reg<FilterInputStream>();
        reg<FilterOutputStream>();
        reg<FilterReader>();
        reg<Flushable>();
        reg<InputStream>();
        reg<InterruptedIOException>();
        reg<InvalidClassException>();
        reg<InvalidObjectException>();
        reg<IOError>();
        reg<IOException>();
        reg<NotActiveException>();
        reg<NotSerializableException>();
        reg<ObjectInput>();
        reg<ObjectOutput>();
        reg<ObjectStreamException>();
        reg<OptionalDataException>();
        reg<OutputStream>();
        reg<PipedInputStream>();
        reg<PipedOutputStream>();
        reg<PushbackReader>();
        reg<Reader>();
        reg<SerializableI>();
        reg<StreamCorruptedException>();
        reg<SyncFailedException>();
        reg<UncheckedIOException>();
        reg<UnsupportedEncodingException>();
        reg<UTFDataFormatException>();
        reg<WriteAbortedException>();
        reg<Writer>();
	   });
	}

void initialize()
{
    initializeClasses();
}

}
