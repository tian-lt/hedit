project(libtext)

set(LIBTEXT_HEADER_FILES
    inc/stringblock.hpp
    inc/stringpool.hpp
    inc/text.hpp
    inc/textdoc.hpp
    inc/textline.hpp
    inc/textmem.hpp
)

set(LIBTEXT_SOURCE_FILES
    src/dummy.cpp
)

add_library(text STATIC
    ${LIBTEXT_SOURCE_FILES}
    ${LIBTEXT_HEADER_FILES} # for IDE searching convienence
)
add_library(libtext::text ALIAS text)

target_include_directories(text PUBLIC inc)

target_precompile_headers(text
    PUBLIC
        <algorithm>
        <array>
        <cmath>
        <cassert>
        <regex>
        <sstream>
        <stdexcept>
        <string>
        <string_view>
        <unordered_map>
        <vector>
)

if(MSVC)
  target_compile_options(text PRIVATE /W4 /WX)
else()
  target_compile_options(text PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
