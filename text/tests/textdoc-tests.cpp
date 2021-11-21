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

TEST(TextDocTests, AccessLine) {
    tx::text_doc doc;
    doc.append("line1\n");
    doc.append("line2");
    EXPECT_STREQ(doc[0].str().c_str(), "line1");
    EXPECT_STREQ(doc[1].str().c_str(), "line2");
}



