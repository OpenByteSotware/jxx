#pragma once
#include "nio/channels/jxx.nio.channels.ReadableByteChannel.h"
namespace jxx::nio::channels {
class ScatteringByteChannel:public ::jxx::lang::InterfaceBase<ScatteringByteChannel,ReadableByteChannel>{public:using BufferArray=::jxx::lang::JxxArray<::jxx::Ptr<::jxx::nio::ByteBuffer>,1U>;~ScatteringByteChannel()override=default;virtual ::jxx::lang::jlong read(const ::jxx::Ptr<BufferArray>&)=0;virtual ::jxx::lang::jlong read(const ::jxx::Ptr<BufferArray>&,::jxx::lang::jint,::jxx::lang::jint)=0;};}
