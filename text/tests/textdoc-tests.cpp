#include <gtest/gtest.h>
#include "text.hpp"

namespace tx = text;

TEST(TextDocTests, AppendText) {
    tx::text_doc doc;
    doc.append("hello text.\n");
    doc.append("there should be \n three lines");
    EXPECT_EQ(doc.line_count(), 3);
    auto txt = doc.str();
    EXPECT_STREQ(txt.c_str(), "hello text.\nthere should be \n three lines");
}



