#include <gtest/gtest.h>
#include "text.hpp"
#include "test-utils.hpp"

namespace tx = text;

TEST(StringBlockTests, Append) {
    auto sb = tx::make_string_block<char>();
    sb.append(std::string("hello"));
}

TEST(StringBlockTests, Str) {
    auto sb = make_test_string_block<char, 3, 100>();

    sb.append(std::string("hello string block"));

    const auto& r1 = sb.str();
    EXPECT_STREQ(r1.c_str(), "hello string block");

    const auto& r2 = sb.str(6, 6);
    EXPECT_STREQ(r2.c_str(), "string");

    const auto& r3 = sb.str(7, 10);
    EXPECT_STREQ(r3.c_str(), "tring bloc");
}



