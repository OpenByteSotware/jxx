#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::swing::event {
class TableModelEvent;
class TableModelListener : public ::jxx::lang::InterfaceBase<TableModelListener, ::jxx::util::EventListener> {
public:
    ~TableModelListener() override = default;
    virtual void tableChanged(const ::jxx::Ptr<TableModelEvent>& event) = 0;
};
}
