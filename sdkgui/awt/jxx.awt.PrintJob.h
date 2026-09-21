#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::awt { class Dimension; class Graphics; class PrintJob : public ::jxx::lang::ClassBase<PrintJob,::jxx::lang::Object> { public: using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<PrintJob,JxxSuper>; ~PrintJob() override=default; virtual ::jxx::Ptr<Graphics> getGraphics()=0; virtual ::jxx::Ptr<Dimension> getPageDimension() const=0; virtual ::jxx::lang::jint getPageResolution() const=0; virtual ::jxx::lang::jbool lastPageFirst() const=0; virtual void end()=0; protected: PrintJob(); }; }
