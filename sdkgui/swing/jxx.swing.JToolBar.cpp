#include "swing/jxx.swing.JToolBar.h"
#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Insets.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/jxx.swing.AbstractButton.h"
#include "swing/jxx.swing.JSeparator.h"
namespace jxx::swing
{
	JToolBar::JToolBar() : JToolBar(HORIZONTAL)
	{
	}
	JToolBar::JToolBar(::jxx::lang::jint orientation) : Super()
	{
		setOrientation(orientation);
	}
	JToolBar::JToolBar(const ::jxx::Ptr<::jxx::lang::String>& name) : JToolBar(name, HORIZONTAL)
	{
	}
	JToolBar::JToolBar(const ::jxx::Ptr<::jxx::lang::String>& name, ::jxx::lang::jint orientation) : Super()
	{
		setName(name); setOrientation(orientation);
	}
	::jxx::Ptr<::jxx::awt::Component> JToolBar::add(const ::jxx::Ptr<::jxx::awt::Component>& component)
	{
		return JComponent::add(component);
	}
	::jxx::Ptr<AbstractButton> JToolBar::add(const ::jxx::Ptr<AbstractButton>& button)
	{
		JComponent::add(button); return button;
	}
	void JToolBar::addSeparator()
	{
		addSeparator(nullptr);
	}
	void JToolBar::addSeparator(const ::jxx::Ptr<::jxx::awt::Dimension>& size)
	{
		auto separator = ::jxx::NEW<JSeparator>(orientation_ == HORIZONTAL ? VERTICAL : HORIZONTAL); if (size) separator->setSize(size); JComponent::add(separator);
	}
	void JToolBar::setOrientation(::jxx::lang::jint value)
	{
		if (value != HORIZONTAL && value != VERTICAL) throw ::jxx::lang::IllegalArgumentException("orientation"); orientation_ = value; invalidate();
	}
	::jxx::lang::jint JToolBar::getOrientation() const
	{
		return orientation_;
	}
	void JToolBar::setFloatable(::jxx::lang::jbool value)
	{
		floatable_ = value;
	} ::jxx::lang::jbool JToolBar::isFloatable() const
	{
		return floatable_;
	}
	void JToolBar::setRollover(::jxx::lang::jbool value)
	{
		rollover_ = value;
	} ::jxx::lang::jbool JToolBar::isRollover() const
	{
		return rollover_;
	}
	void JToolBar::setBorderPainted(::jxx::lang::jbool value)
	{
		borderPainted_ = value;
	} ::jxx::lang::jbool JToolBar::isBorderPainted() const
	{
		return borderPainted_;
	}
	void JToolBar::setMargin(const ::jxx::Ptr<::jxx::awt::Insets>& value)
	{
		margin_ = value;
	} ::jxx::Ptr<::jxx::awt::Insets> JToolBar::getMargin() const
	{
		return margin_;
	}
}
