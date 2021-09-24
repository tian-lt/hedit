#include "text.hpp"

int main()
{
    using spool_t = text::basic_string_pool<1, char>;
    using sblock_t = text::basic_string_block<char>;

    spool_t pool(100);
    sblock_t sb(
        [&pool]() {return pool.allocate(); },
        [&pool](auto&& frag) {pool.deallocate(std::move(frag)); },
        [&pool]() {return pool.fragment_size; },
        "hello");

    return 0;
}