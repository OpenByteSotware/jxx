#pragma once
#include "../lang/jxx.lang.Object.h"
#include "jxx.io.IOException.h"
namespace jxx {
	namespace io {
		class EOFException : public IOException {
		public:
			using jxx::io::IOException::IOException;
			
		protected:
			JXX_OBJECT_CLONE(EOFException)
			const char* typeName() const noexcept override { return "EOFException"; }
		};
	}
}
