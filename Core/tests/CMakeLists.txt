add_executable(CoreTests
  intersection.test.cpp
  physicsObject.test.cpp
  utf8Str.test.cpp
)

target_link_libraries(CoreTests
    PRIVATE
        engine.core
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(CoreTests)
