#include "awt/jxx.awt.Panel.h"
#include "awt/jxx.awt.FlowLayout.h"

namespace jxx::awt
{
    Panel::Panel()
        : Super()
    {
        setLayout(::jxx::NEW<FlowLayout>());
    }
}
