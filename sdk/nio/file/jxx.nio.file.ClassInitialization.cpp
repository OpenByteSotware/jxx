#include "nio/file/jxx.nio.file.ClassInitialization.h"
#include "nio/file/attribute/jxx.nio.file.attribute.ClassInitialization.h"

namespace jxx::nio::file
{
void initialize()
{
    ::jxx::nio::file::attribute::initialize();
}

} // namespace jxx::nio::file
