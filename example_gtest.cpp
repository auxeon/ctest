#include <gtest/gtest.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#define sleep Sleep
#endif
static time_t ___t;
static time_t ___lt;
#define COLORS 1
#if COLORS == 1
#define COLORS_RED "\033[0;31m"
#define COLORS_NIL "\033[0m"
#define COLORS_GREEN "\033[0;32m"
#define COLORS_BLUE "\033[0;34m"
#define COLORS_YELLOW "\033[0;33m"
#else
#define COLORS_RED ""
#define COLORS_NIL ""
#define COLORS_GREEN ""
#define COLORS_BLUE ""
#define COLORS_YELLOW ""
#endif
#if COLORS == 1
#define __LOG(type, color, ...)                                       \
  ___t = time(NULL);                                                  \
  ___lt = (___t > 0) ? ___t : ___t * 1u;                              \
  printf("%s[%s][%ld] %s:%s:%d%s ", (color), (type), ___lt, __FILE__, \
         __func__, __LINE__, COLORS_NIL);                             \
  printf(__VA_ARGS__);                                                \
  printf("\n");
#else
#define __LOG(type, color, ...)                                               \
  ___t = time(NULL);                                                          \
  ___lt = (___t > 0) ? ___t : ___t * 1u;                                      \
  printf("[%s][%ld] %s:%s:%d ", (type), ___lt, __FILE__, __func__, __LINE__); \
  printf(__VA_ARGS__);                                                        \
  printf("\n");
#endif
#define LOG(...) __LOG("LOG", COLORS_NIL, __VA_ARGS__)
#define LOGINFO(...) __LOG("INFO", COLORS_GREEN, __VA_ARGS__)
#define LOGWARN(...) __LOG("WARN", COLORS_YELLOW, __VA_ARGS__)
#define LOGERROR(...) __LOG("ERROR", COLORS_RED, __VA_ARGS__)

TEST(example, assert_eq) {
  printf("%s : hello there\n", __func__);
  ASSERT_EQ(0, 0);
}

TEST(example, pythagoras) {
  LOG("%s", __func__);
  double a = 8, o = 6, h = 10;
  printf("%lf, %lf\n", h, hypot(o, a));
  LOGINFO("%s : %lf, %lf\n", __func__,  h, hypot(o, a));
  EXPECT_FLOAT_EQ(hypot(o, a), h);
}

TEST(example, timeout) {
  sleep(2);
}

TEST(example, assert_lt) {
  EXPECT_LT(1, 2);
}

TEST(example, nothing) {
  
}

TEST(example, skip) {
  GTEST_SKIP();
  printf("this won't get printed since it's skipped\n");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest();
  (void)RUN_ALL_TESTS();
}
