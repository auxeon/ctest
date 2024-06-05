#define COLORS 1
#define TIMEOUT 1
#define MAXTESTS 10
#include "ctest.h"

TEST(example, assert_eq) {
  printf("%s : hello there\n", __func__);
  ASSERT_EQ(0, 0);
}

TEST(example, pythagoras) {
  LOGERROR("%s", __func__);
  double a = 8, o = 6, h = 10;
  printf("%lf, %lf\n", h, hypot(o, a));
  LOGINFO("%s : %lf, %lf\n", __func__,  h, hypot(o, a));
  ASSERT_APPROX_EQ(hypot(o, a), h);
}

TEST(example, timeout) {
  sleep(2);
}

TEST(example, assert_lt) {
  ASSERT_LT(1, 2);
}

TEST(example, nothing) {
  
}

SKIPTEST(example, skip) {
  printf("this won't get printed since it's skipped\n");
}

int main(int argc, char** argv) {
  CTESTRUN(); 
}
