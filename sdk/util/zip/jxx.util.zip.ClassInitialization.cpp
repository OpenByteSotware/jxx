#include "util/zip/jxx.util.zip.ClassInitialization.h"

#include <mutex>

#include "util/zip/jxx.util.zip.Adler32.h"
#include "util/zip/jxx.util.zip.CRC32.h"
#include "util/zip/jxx.util.zip.CheckedInputStream.h"
#include "util/zip/jxx.util.zip.CheckedOutputStream.h"
#include "util/zip/jxx.util.zip.Checksum.h"
#include "util/zip/jxx.util.zip.DataFormatException.h"
#include "util/zip/jxx.util.zip.Deflater.h"
#include "util/zip/jxx.util.zip.DeflaterOutputStream.h"
#include "util/zip/jxx.util.zip.GZIPInputStream.h"
#include "util/zip/jxx.util.zip.GZIPOutputStream.h"
#include "util/zip/jxx.util.zip.Inflater.h"
#include "util/zip/jxx.util.zip.InflaterInputStream.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipException.h"
#include "util/zip/jxx.util.zip.ZipFile.h"
#include "util/zip/jxx.util.zip.ZipInputStream.h"
#include "util/zip/jxx.util.zip.ZipOutputStream.h"

namespace jxx::util::zip {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)Adler32::JxxClassInfoMarker::Class();
        (void)CRC32::JxxClassInfoMarker::Class();
        (void)CheckedInputStream::JxxClassInfoMarker::Class();
        (void)CheckedOutputStream::JxxClassInfoMarker::Class();
        (void)Checksum::JxxClassInfoMarker::Class();
        (void)DataFormatException::JxxClassInfoMarker::Class();
        (void)Deflater::JxxClassInfoMarker::Class();
        (void)DeflaterOutputStream::JxxClassInfoMarker::Class();
        (void)GZIPInputStream::JxxClassInfoMarker::Class();
        (void)GZIPOutputStream::JxxClassInfoMarker::Class();
        (void)Inflater::JxxClassInfoMarker::Class();
        (void)InflaterInputStream::JxxClassInfoMarker::Class();
        (void)ZipEntry::JxxClassInfoMarker::Class();
        (void)ZipException::JxxClassInfoMarker::Class();
        (void)ZipFile::JxxClassInfoMarker::Class();
        (void)ZipInputStream::JxxClassInfoMarker::Class();
        (void)ZipOutputStream::JxxClassInfoMarker::Class();
    });
}

} // namespace jxx::util::zip
