#include "util/jxx.util.logging.ErrorManager.h"
#include <iostream>
namespace jxx::util::logging
{
	void ErrorManager::error(const jxx::Ptr<jxx::lang::String>& m, const jxx::Ptr<jxx::lang::Exception>&, jxx::lang::jint c)
	{
		std::cerr << "java.util.logging.ErrorManager: " << c << ": " << (m ? m->utf8() : "") << std::endl;
	}
} // namespace jxx::util::logging
