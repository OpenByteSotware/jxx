#include "swing/jxx.swing.JPanel.h"
#include "awt/jxx.awt.FlowLayout.h"
namespace jxx::swing
{
    JPanel::JPanel() : JPanel(::jxx::NEW<::jxx::awt::FlowLayout>(), true) {}
    JPanel::JPanel(::jxx::lang::jbool value) : JPanel(::jxx::NEW<::jxx::awt::FlowLayout>(), value) {}
    JPanel::JPanel(const ::jxx::Ptr<::jxx::awt::LayoutManager>& layout) : JPanel(layout, true) {}
    JPanel::JPanel(const ::jxx::Ptr<::jxx::awt::LayoutManager>& layout, ::jxx::lang::jbool value)
    { setLayout(layout); setDoubleBuffered(value); setOpaque(true); }
}
