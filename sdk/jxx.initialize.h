#pragma once

namespace jxx {

using ClientInitializer = void (*)();

void initialize();
void initialize(ClientInitializer clientInitializer);

template <typename T>
void initializeClass()
{
    (void)T::Class();
}

} // namespace jxx
