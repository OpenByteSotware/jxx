#include "awt/jxx.awt.SplashScreen.h"

#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Rectangle.h"

namespace jxx::awt
{
    SplashScreen::SplashScreen() : Super(){}
    ::jxx::Ptr<SplashScreen> SplashScreen::getSplashScreen(){return nullptr;}
    ::jxx::lang::jbool SplashScreen::isVisible() const{return visible_;}
    ::jxx::Ptr<Rectangle> SplashScreen::getBounds() const{return ::jxx::NEW<Rectangle>();}
    ::jxx::Ptr<Dimension> SplashScreen::getSize() const{return ::jxx::NEW<Dimension>();}
    ::jxx::Ptr<::jxx::net::URL> SplashScreen::getImageURL() const{return imageURL_;}
    void SplashScreen::setImageURL(const ::jxx::Ptr<::jxx::net::URL>& value){imageURL_=value;}
    ::jxx::Ptr<Graphics2D> SplashScreen::createGraphics(){return nullptr;}
    void SplashScreen::update(){}
    void SplashScreen::close(){visible_=false;}
}
