#include "io/jxx.io.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.CharConversionException.h"
#include "io/jxx.io.EOFException.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.InterruptedIOException.h"
#include "io/jxx.io.ObjectStreamException.h"
#include "io/jxx.io.InvalidClassException.h"
#include "io/jxx.io.InvalidObjectException.h"
#include "io/jxx.io.NotActiveException.h"
#include "io/jxx.io.NotSerializableException.h"
#include "io/jxx.io.OptionalDataException.h"
#include "io/jxx.io.StreamCorruptedException.h"
#include "io/jxx.io.WriteAbortedException.h"
#include "io/jxx.io.SyncFailedException.h"
#include "io/jxx.io.UnsupportedEncodingException.h"
#include "io/jxx.io.UTFDataFormatException.h"
#include "io/jxx.io.UncheckedIOException.h"
#include "io/jxx.io.IOError.h"
#include "io/jxx.io.SerializableI.h"

namespace jxx::io {

namespace {

template <typename T>
void registerType()
{
    jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;

    std::call_once(
        flag,
        [] {
            registerType<IOException>();
            registerType<CharConversionException>();
            registerType<EOFException>();
            registerType<FileNotFoundException>();
            registerType<InterruptedIOException>();
            registerType<ObjectStreamException>();
            registerType<InvalidClassException>();
            registerType<InvalidObjectException>();
            registerType<NotActiveException>();
            registerType<NotSerializableException>();
            registerType<OptionalDataException>();
            registerType<StreamCorruptedException>();
            registerType<WriteAbortedException>();
            registerType<SyncFailedException>();
            registerType<UnsupportedEncodingException>();
            registerType<UTFDataFormatException>();
            registerType<UncheckedIOException>();
            registerType<IOError>();
            registerType<SerializableI>();
        });
}

} // namespace jxx::io
