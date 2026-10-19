#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
namespace jxx::ext::imageio { class ImageWriteParam : public ::jxx::lang::ClassBase<ImageWriteParam,::jxx::lang::Object> { public: using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<ImageWriteParam,JxxSuper>; static constexpr ::jxx::lang::jint MODE_DISABLED=0,MODE_DEFAULT=1,MODE_EXPLICIT=2,MODE_COPY_FROM_METADATA=3; ImageWriteParam(); void setCompressionMode(::jxx::lang::jint mode); ::jxx::lang::jint getCompressionMode()const{return mode_;} void setCompressionQuality(::jxx::lang::jfloat quality); ::jxx::lang::jfloat getCompressionQuality()const{return quality_;} protected: ::jxx::lang::jint mode_=MODE_COPY_FROM_METADATA; ::jxx::lang::jfloat quality_=0.75F; }; }
