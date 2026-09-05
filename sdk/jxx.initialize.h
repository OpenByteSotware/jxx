#pragma once

namespace jxx {

void initialize();

template <typename T>
void initializeClass()
{
    (void)T::Class();
}

} // namespace jxx
