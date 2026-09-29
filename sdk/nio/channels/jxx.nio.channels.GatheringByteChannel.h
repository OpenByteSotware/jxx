#pragma once
#include "nio/channels/jxx.nio.channels.WritableByteChannel.h"
namespace jxx::nio::channels {
class GatheringByteChannel:public ::jxx::lang::InterfaceBase<GatheringByteChannel,WritableByteChannel>{public:using BufferArray=::jxx::lang::JxxArray<::jxx::Ptr<::jxx::nio::ByteBuffer>,1U>;~GatheringByteChannel()override=default;virtual ::jxx::lang::jlong write(const ::jxx::Ptr<BufferArray>&)=0;virtual ::jxx::lang::jlong write(const ::jxx::Ptr<BufferArray>&,::jxx::lang::jint,::jxx::lang::jint)=0;};}
