// Copyright (c) Abhikalp Unakal

#pragma once
  
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define EXIT_ON_TIMEOUT 0

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define STDOUT_FILENO _fileno(stdout)
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#endif

#ifdef _MSC_VER
  #pragma section(".CRT$XCU", read)
  typedef void (__cdecl *init_func)(void);
  #define CONSTRUCTOR_IMPL(func, line) \
    static void func(void); \
    __declspec(allocate(".CRT$XCU")) init_func _##func##_##line = func; \
    static void func(void)
  #define CONSTRUCTOR(func) CONSTRUCTOR_IMPL(func, __LINE__)
#else
  #define CONSTRUCTOR_IMPL(func) \
    __attribute__((constructor)) static void func(void)
  #define CONSTRUCTOR(func) CONSTRUCTOR_IMPL(func)
#endif

static void __ctest_assert(const char* funcname, const char* fname, int lineno,
                           const char* reason);
static void __ctest_register_test(const char* testname, void (*test)(void),
                                  int skip);
static void __ctest_print_result(void);
static void __handle_sig_alarm(int signal);
static int __ctest_run_all_tests(void);
static uint64_t __time_now(void);

#ifndef EXIT_DEFAULT
#define EXIT_DEFAULT -1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef EXIT_TIMEOUT
#define EXIT_TIMEOUT 2
#endif

#ifndef EXIT_SKIP
#define EXIT_SKIP 3
#endif

#ifndef MAXTESTS
#define MAXTESTS 20
#endif

#ifndef TIMEOUT
#define TIMEOUT -1
#endif

#ifndef COLORS
#define COLORS 1
#endif

#ifndef EPSILON
#define EPSILON 0.001
#endif

#define TOSTR(x) #x

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

#ifndef NS2S
#define NS2S(t) (t)/1e9
#endif

#ifndef TIC
#define TIC() (__time_now())
#endif

#ifndef TOC
#define TOC(t) (__time_now() - (t))
#endif

#ifndef EXIT_ON_TIMEOUT
#define EXIT_ON_TIMEOUT 1
#endif

#ifdef EXIT_ON_ASSERT
#include <assert.h>
#define __ASSERT_CMP(__x, __y, __cmp)                                     \
  {                                                                       \
    if(!((__x)__cmp(__y))) {                                              \
      gTestStatuses[gLastTestId] = EXIT_FAILURE;                          \
      __assert_rtn(__func__, __FILE__, __LINE__, TOSTR((__x)__cmp(__y))); \
      return;                                                             \
    }                                                                     \
  }
#else
#define __ASSERT_CMP(__x, __y, __cmp)                                     \
  {                                                                       \
    if(!(__x __cmp __y)) {                                                \
      gTestStatuses[gLastTestId] = EXIT_FAILURE;                          \
      __ctest_assert(__func__, __FILE__, __LINE__, TOSTR(__x __cmp __y)); \
      return;                                                             \
    }                                                                     \
  }
#endif

#define ASSERT_EQ(x, y) __ASSERT_CMP((x), (y), ==)
#define ASSERT_GT(x, y) __ASSERT_CMP((x), (y), >)
#define ASSERT_LT(x, y) __ASSERT_CMP((x), (y), <)
#define ASSERT_NE(x, y) __ASSERT_CMP((x), (y), !=)
#define ASSERT_APPROX_EQ(x, y) __ASSERT_CMP(fabs((x) - (y)), EPSILON, <)
#define ASSERT_APPROX_EQE(x, y, epsilon) __ASSERT_CMP(fabs((x) - (y)), epsilon, <)

#if COLORS == 1
#define __LOG(type, color, ...)                                               \
  ___t = time(NULL);                                                          \
  ___lt = (___t > 0) ? ___t : ___t * 1u;                                      \
  if (!isatty(STDOUT_FILENO)) {                                               \
    printf("[%s][%ld] %s:%s:%d ", (type), ___lt, __FILE__,                    \
         __func__, __LINE__);                                                 \
  }                                                                           \
  else {                                                                      \
    printf("%s[%s][%ld] %s:%s:%d%s ", (color), (type), ___lt, __FILE__,       \
         __func__, __LINE__, COLORS_NIL);                                     \
  }                                                                           \
  printf(__VA_ARGS__);                                                        \
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

#define TEST(classname, testname)                                           \
  static void classname##_##testname(void);                                 \
  CONSTRUCTOR(__register_##classname##_##testname) {                        \
    __ctest_register_test(TOSTR(classname##_##testname),                    \
                          classname##_##testname, 0);                       \
  }                                                                         \
  static void classname##_##testname(void)

#define SKIPTEST(classname, testname)                                       \
  static void classname##_##testname(void);                                 \
  CONSTRUCTOR(__register_##classname##_##testname) {                        \
    __ctest_register_test(TOSTR(classname##_##testname),                    \
                          classname##_##testname, 1);                       \
  }                                                                         \
  static void classname##_##testname(void)

static int gTestId = 0;
static void (*gTests[MAXTESTS])(void) = {};
static const char* gTestNames[MAXTESTS] = {};
static int gTestStatuses[MAXTESTS] = {};
static int gTestSkip[MAXTESTS] = {};
static uint64_t gTestTimes[MAXTESTS] = {};
static int gTestNPass = 0;
static int gTestNTime = 0;
static int gTestNFail = 0;
static int gTestNSkip = 0;
static int gLastTestId = -1;
static time_t ___t;
static time_t ___lt;

typedef enum Gstatus {
  Gstatus_Success,
  Gstatus_MaxTestLimitExceeded,
} Gstatus;

static uint64_t __time_now(void) {
  struct timespec now;
  #if defined(_WIN32)
  timespec_get(&now, TIME_UTC);
  #else
  clock_gettime(CLOCK_MONOTONIC, &now);
  #endif
  return now.tv_sec * 1000000000ull + now.tv_nsec;
}

static void __ctest_assert(const char* funcname, const char* filename, int lineno,
                           const char* reason) {
  fprintf(stderr, "Assertion failed: %s, function %s, file %s, line %d.\n",
          reason, funcname, filename, lineno);
}

static void __ctest_register_test(const char* testname, void (*test)(void),
                                  int skip) {
  if(gTestId >= MAXTESTS) {
    fprintf(stderr, "%s: please increase MAXTESTS limit or delete tests\n",
            TOSTR(Gstatus_MaxTestLimitExceeded));
    exit(Gstatus_MaxTestLimitExceeded);
  }
  gTests[gTestId] = test;
  gTestNames[gTestId] = testname;
  gTestSkip[gTestId] = skip;
  ++gTestId;
}

static void __ctest_print_result(void) {
  printf("[RESULTS]\n");
  const char* CRED = "";
  const char* CGREEN = "";
  const char* CNIL = "";
  const char* CBLUE = "";
  const char* CYELLOW = "";
  if(isatty(STDOUT_FILENO)) {
#if COLORS == 1
    CRED = COLORS_RED;
    CGREEN = COLORS_GREEN;
    CNIL = COLORS_NIL;
    CBLUE = COLORS_BLUE;
    CYELLOW = COLORS_YELLOW;
#endif
  }

  printf("STATUS, ID, NAME, TIME\n");
  for(int i = 0; i < gTestId; ++i) {
    const char* color = "";
    const char* status = "";
    switch(gTestStatuses[i]) {
      case EXIT_SUCCESS:
        color = CGREEN;
        status = "PASS";
        break;
      case EXIT_FAILURE:
        color = CRED;
        status = "FAIL";
        break;
      case EXIT_TIMEOUT:
        color = CYELLOW;
        status = "TIME";
        break;
      case EXIT_SKIP:
        color = CNIL;
        status = "SKIP";
        break;
      default:
        break;
    }
    printf("%s[%-4s]%s, (%d/%d), %s, %lf\n", color, status, CNIL, i + 1, gTestId,
           gTestNames[i], NS2S(gTestTimes[i]));
  }
  printf(
      "Ran %s%d tests%s, %s%d Failed%s, %s%d Passed%s, %s%d Timed out%s, %s%d "
      "Skipped%s.\n",
      CBLUE, gTestId, CNIL,
      CRED, gTestNFail, CNIL,
      CGREEN, gTestNPass, CNIL,
      CYELLOW, gTestNTime, CNIL,
      CNIL, gTestNSkip, CNIL);
}

static void __handle_sig_alarm(int signal) {
  gTestStatuses[gLastTestId] = EXIT_TIMEOUT;
}

static int __ctest_run_all_tests(void) {
#if TIMEOUT > 0 && EXIT_ON_TIMEOUT == 1
  signal(SIGALRM, __handle_sig_alarm);
#endif
  for(int i = 0; i < gTestId; ++i) {
    printf("[TEST %s]\n", gTestNames[i]);
    gLastTestId = i;
#if TIMEOUT > 0 && EXIT_ON_TIMEOUT == 1
    alarm(TIMEOUT);
#endif
    int status = EXIT_SUCCESS;
    if(!gTestSkip[i]) {
      gTestTimes[i] = TIC();
      gTests[i]();
      gTestTimes[i] = TOC(gTestTimes[i]);
    } else {
      status = EXIT_SKIP;
      }
    if (NS2S(gTestTimes[i]) > TIMEOUT && TIMEOUT > 0) {
      status = EXIT_TIMEOUT;
    }
    if(!gTestStatuses[i]) {
      gTestStatuses[i] = status;
    }
    gTestNPass += (gTestStatuses[i] == EXIT_SUCCESS);
    gTestNFail += (gTestStatuses[i] == EXIT_FAILURE);
    gTestNTime += (gTestStatuses[i] == EXIT_TIMEOUT);
    gTestNSkip += (gTestStatuses[i] == EXIT_SKIP);
  }
  __ctest_print_result();
  return !(gTestNPass == gTestId);
}

#define CTESTRUN()                                                    \
  printf("-------- %s --------\n", __FILE__);                         \
  __ctest_run_all_tests();                                            \
  printf("-------- %s --------\n", __FILE__);
