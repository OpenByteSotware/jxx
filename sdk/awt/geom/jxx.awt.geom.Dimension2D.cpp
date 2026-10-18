#include "awt/geom/jxx.awt.geom.Dimension2D.h"
namespace jxx::awt::geom { void Dimension2D::setSize(const ::jxx::Ptr<Dimension2D>& d){ if(d) setSize(d->getWidth(),d->getHeight()); } }
