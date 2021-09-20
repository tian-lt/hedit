#include "text.hpp"

int main()
{
    text::StringPool sp;
    int n = 10000;
    while(n-->0)
        auto frag = sp.allocate();

    sp.deallocate(0);
    sp.deallocate(2);
    auto frag = sp.allocate();

    return 0;
}