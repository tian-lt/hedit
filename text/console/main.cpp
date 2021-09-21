#include "text.hpp"

int main()
{
    using spool_t = text::basic_string_pool<1, char>;
    using sblock_t = text::basic_string_block<char, 1>;

    spool_t pool;
    sblock_t sb(&pool, "hello");

    return 0;
}