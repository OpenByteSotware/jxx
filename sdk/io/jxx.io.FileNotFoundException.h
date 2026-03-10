#pragma once
#include "io/jxx.io.IOException.h"
namespace jxx { namespace io { struct FileNotFoundException : public IOException { using IOException::IOException; }; }}
