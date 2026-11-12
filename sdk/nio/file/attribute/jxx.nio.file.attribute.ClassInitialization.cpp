#include "nio/file/attribute/jxx.nio.file.attribute.ClassInitialization.h"
#include <mutex>
#include "nio/file/attribute/jxx.nio.file.attribute.FileTime.h"
namespace jxx::nio::file::attribute {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
            ::jxx::lang::class_info_detail::registerClassInfo<FileTime>();
        (void)::jxx::lang::ClassBase<
            FileTime,
            ::jxx::lang::Object,
            ::jxx::lang::Comparable<FileTime>>::Class();
    });
}

void initialize()
{
    initializeClasses();
}

} // namespace jxx::nio::file::attribute
