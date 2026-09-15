#include "awt/jxx.awt.Container.h"
#include <algorithm>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::awt
{
	Container::Container() = default; Container::~Container()
	{
		removeAll();
	}
	::jxx::Ptr<Component> Container::add(const ::jxx::Ptr<Component>& c)
	{
		return add(c, getComponentCount());
	}
	::jxx::Ptr<Component> Container::add(const ::jxx::Ptr<Component>& c, ::jxx::lang::jint index)
	{
		if (!c)throw ::jxx::lang::NullPointerException("component"); if (index<0 || index>getComponentCount())throw ::jxx::lang::IllegalArgumentException("index"); if (auto old = c->getParent())old->remove(c); components_.insert(components_.begin() + index, c); c->setParentInternal(::jxx::CAST<Container>(thisPtr())); if (layout_)layout_->addLayoutComponent(nullptr, c); invalidate(); return c;
	}
	::jxx::Ptr<Component> Container::add(const ::jxx::Ptr<::jxx::lang::String>& name, const ::jxx::Ptr<Component>& c)
	{
		auto result = add(c); if (layout_)layout_->addLayoutComponent(name, c); return result;
	}
	void Container::remove(::jxx::lang::jint index)
	{
		if (index < 0 || index >= getComponentCount())throw ::jxx::lang::IndexOutOfBoundsException("index"); auto c = components_[static_cast<std::size_t>(index)]; if (layout_)layout_->removeLayoutComponent(c); c->setParentInternal(nullptr); components_.erase(components_.begin() + index); invalidate();
	}
	void Container::remove(const ::jxx::Ptr<Component>& c)
	{
		auto it = std::find(components_.begin(), components_.end(), c); if (it != components_.end())remove(static_cast<::jxx::lang::jint>(std::distance(components_.begin(), it)));
	}
	void Container::removeAll()
	{
		while (!components_.empty())remove(getComponentCount() - 1);
	} ::jxx::lang::jint Container::getComponentCount() const
	{
		return static_cast<::jxx::lang::jint>(components_.size());
	} ::jxx::Ptr<Component> Container::getComponent(::jxx::lang::jint index) const
	{
		if (index < 0 || index >= getComponentCount())throw ::jxx::lang::IndexOutOfBoundsException("index"); return components_[static_cast<std::size_t>(index)];
	}
	void Container::setLayout(const ::jxx::Ptr<LayoutManager>& l)
	{
		layout_ = l; invalidate();
	} ::jxx::Ptr<LayoutManager> Container::getLayout() const
	{
		return layout_;
	} void Container::validate()
	{
		doLayout(); Component::validate();
	} void Container::doLayout()
	{
		if (layout_)layout_->layoutContainer(::jxx::CAST<Container>(thisPtr()));
	}
}
