#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.Future.h"
namespace jxx::util::concurrent
{
	template <typename V> class RunnableFuture :public virtual jxx::lang::Runnable, 
		public virtual Future<V>
	{
	public:virtual ~RunnableFuture() = default;
	};
}
