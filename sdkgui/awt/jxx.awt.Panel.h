#pragma once

#include "awt/jxx.awt.Container.h"

namespace jxx::awt
{
    class Panel :
        public ::jxx::lang::ClassBase<Panel, Container>
    {
    public:
        using JxxSuper = Container;
        using Super = ::jxx::lang::ClassBase<Panel, JxxSuper>;

        Panel();
        ~Panel() override = default;
    };
}
