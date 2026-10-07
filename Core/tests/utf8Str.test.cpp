#include <bitset>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "utf8Str.h"
TEST(UTF8STRING, LENGTH) {
  auto utf8 = Core::Utf8String("meow");
  EXPECT_EQ(utf8.length(), 4);
}

TEST(UTF8STRING, SUBSCRIPT) {
  auto utf8 = Core::Utf8String("meow");
  Core::rune r = utf8[0];
  EXPECT_EQ(r, Core::rune(U'm'));
}

TEST(UTF8STRING, NOTASCII) {
  auto utf8 = Core::Utf8String("नगर");
  Core::rune r = utf8[0];
  EXPECT_EQ(r, U'न');
  r = utf8[1];
  EXPECT_EQ(r, U'ग');
}

TEST(UTF8STRING, CJK) {
  auto utf8 = Core::Utf8String("臺金");
  Core::rune r = utf8[1];
  std::cerr << "my bitset: " << std::bitset<32>(r) << "\n\n"
            << "actual bitset: " << std::bitset<32>(U'金');
  EXPECT_EQ(r, U'金');
}

TEST(UTF8STRING, EMPTY) {
  auto utf8 = Core::Utf8String("");
  EXPECT_EQ(utf8.length(), 0);
}

TEST(UTF8STRING, ASCII_LENGTH) {
  auto utf8 = Core::Utf8String("hello world");
  EXPECT_EQ(utf8.length(), 11);
}

TEST(UTF8STRING, ASCII_SUBSCRIPT) {
  auto utf8 = Core::Utf8String("hello");

  EXPECT_EQ(utf8[0], U'h');
  EXPECT_EQ(utf8[1], U'e');
  EXPECT_EQ(utf8[2], U'l');
  EXPECT_EQ(utf8[3], U'l');
  EXPECT_EQ(utf8[4], U'o');
}

TEST(UTF8STRING, TWO_BYTE_UTF8) {
  auto utf8 = Core::Utf8String("é");

  EXPECT_EQ(utf8.length(), 1);
  EXPECT_EQ(utf8[0], U'é');
}

TEST(UTF8STRING, THREE_BYTE_UTF8) {
  auto utf8 = Core::Utf8String("€");

  EXPECT_EQ(utf8.length(), 1);
  EXPECT_EQ(utf8[0], U'€');
}

TEST(UTF8STRING, FOUR_BYTE_UTF8) {
  auto utf8 = Core::Utf8String("😀");

  EXPECT_EQ(utf8.length(), 1);
  EXPECT_EQ(utf8[0], U'😀');
}

TEST(UTF8STRING, MIXED) {
  auto utf8 = Core::Utf8String("Aé€😀");

  EXPECT_EQ(utf8.length(), 4);
  EXPECT_EQ(utf8[0], U'A');
  EXPECT_EQ(utf8[1], U'é');
  EXPECT_EQ(utf8[2], U'€');
  EXPECT_EQ(utf8[3], U'😀');
}

TEST(UTF8STRING, MIXED_TEXT) {
  auto utf8 = Core::Utf8String("Hello 世界!");

  EXPECT_EQ(utf8.length(), 9);
  EXPECT_EQ(utf8[0], U'H');
  EXPECT_EQ(utf8[6], U'世');
  EXPECT_EQ(utf8[7], U'界');
  EXPECT_EQ(utf8[8], U'!');
}

TEST(UTF8STRING, DEVANAGARI_CODEPOINTS) {
  auto utf8 = Core::Utf8String("नमस्ते");

  EXPECT_EQ(utf8.length(), 6);

  EXPECT_EQ(utf8[0], U'न');
  EXPECT_EQ(utf8[1], U'म');
  EXPECT_EQ(utf8[2], U'स');
  EXPECT_EQ(utf8[3], U'्');
  EXPECT_EQ(utf8[4], U'त');
  EXPECT_EQ(utf8[5], U'े');
}
