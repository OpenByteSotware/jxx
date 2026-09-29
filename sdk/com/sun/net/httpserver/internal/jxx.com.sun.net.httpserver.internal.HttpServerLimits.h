#pragma once
#include <cstddef>
namespace jxx::com::sun::net::httpserver::internal
{
	struct HttpServerLimits final
	{
		std::size_t maxRequestLineBytes = 8192U, maxHeaderBytes = 65536U, maxHeaderCount = 100U, maxHeaderNameBytes = 256U, maxHeaderValueBytes = 8192U, maxChunkLineBytes = 1024U, maxTrailerBytes = 16384U, maxTrailerCount = 32U, maxDecodedBodyBytes = 16U * 1024U * 1024U;
	};
}
