#include "awt/image/jxx.awt.image.ClassInitialization.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "awt/image/jxx.awt.image.ImageObserver.h"
#include "awt/image/jxx.awt.image.ColorModel.h"
#include "awt/image/jxx.awt.image.DirectColorModel.h"
#include "awt/image/jxx.awt.image.ImageFilter.h"
#include "awt/image/jxx.awt.image.RGBImageFilter.h"
#include <mutex>
namespace jxx::awt::image
{
	void initializeClasses()
	{
		static std::once_flag flag; std::call_once(flag, []
	   {
				  (void)ImageObserver::Class();                  
				  (void)BufferedImage::Class();
                  (void)ColorModel::Class();
                  (void)DirectColorModel::Class();
                  (void)ImageFilter::Class();
				  (void)RGBImageFilter::Class();
	   });
	}
}
