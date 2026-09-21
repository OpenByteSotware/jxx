#pragma once
#include <algorithm>
#include <vector>
#include "swing/jxx.swing.ListModel.h"
#include "swing/event/jxx.swing.event.ListDataEvent.h"
#include "swing/event/jxx.swing.event.ListDataListener.h"
namespace jxx::swing
{
    template<typename E>
    class AbstractListModel : public ::jxx::lang::ClassBase<AbstractListModel<E>, ::jxx::lang::Object, ListModel<E>>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<AbstractListModel<E>, JxxSuper, ListModel<E>>;
        ~AbstractListModel() override = default;
        void addListDataListener(const ::jxx::Ptr<::jxx::swing::event::ListDataListener>& listener)
        { if (listener) listeners_.push_back(listener); }
        void removeListDataListener(const ::jxx::Ptr<::jxx::swing::event::ListDataListener>& listener)
        { listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener), listeners_.end()); }
        std::vector<::jxx::Ptr<::jxx::swing::event::ListDataListener>> getListDataListeners() const
        { return listeners_; }
    protected:
        void fireContentsChanged(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint index0, ::jxx::lang::jint index1)
        { fire(source, ::jxx::swing::event::ListDataEvent::CONTENTS_CHANGED, index0, index1); }
        void fireIntervalAdded(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint index0, ::jxx::lang::jint index1)
        { fire(source, ::jxx::swing::event::ListDataEvent::INTERVAL_ADDED, index0, index1); }
        void fireIntervalRemoved(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint index0, ::jxx::lang::jint index1)
        { fire(source, ::jxx::swing::event::ListDataEvent::INTERVAL_REMOVED, index0, index1); }
    private:
        void fire(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint type, ::jxx::lang::jint index0, ::jxx::lang::jint index1)
        {
            auto event = ::jxx::NEW<::jxx::swing::event::ListDataEvent>(source, type, index0, index1);
            const auto copy = listeners_;
            for (const auto& listener : copy) {
                if (!listener) continue;
                if (type == ::jxx::swing::event::ListDataEvent::CONTENTS_CHANGED) listener->contentsChanged(event);
                else if (type == ::jxx::swing::event::ListDataEvent::INTERVAL_ADDED) listener->intervalAdded(event);
                else listener->intervalRemoved(event);
            }
        }
        std::vector<::jxx::Ptr<::jxx::swing::event::ListDataListener>> listeners_;
    };
}
