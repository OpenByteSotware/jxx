#include "nio/file/attribute/jxx.nio.file.attribute.ClassInitialization.h"
#include <mutex>
#include "nio/file/attribute/jxx.nio.file.attribute.FileTime.h"
namespace jxx::nio::file::attribute {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            FileTime,
            ::jxx::lang::Object,
            ::jxx::lang::Comparable<FileTime>>::Class();
    });
}
} // namespace jxx::nio::file::attribute
