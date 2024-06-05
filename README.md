# CTEST

A simple Header only test framework in c99 with the following features

- [x] Timeouts
- [x] Asserts
- [x] Logging
- [x] Colors 
- [x] Skippable Tests

## TO GET STARTED
Since it is header only you can just drag and drop ```ctest.h``` into your project and use it with your preferred build system

If you're planning on using BUCK2 use the instructions below

## TO GET STARTED WITH BUCK2
```git clone --recurse-submodules --remote-submodules git@github.com:auxeon/ctest.git```

## TO RUN USING BUCK
```buck2 run //:example_ctest```

## TO COMPILE
Just include the header file ```ctest.h``` in your project and make sure the ```ctest.h``` file's location is in your include paths. all the tests will have static scope meaning tests that you write in other files and include, will not be
run by default - you will need to setup a non static function inside that invokes the static ctest_run_all_tests()
check


## USAGE EXAMPLES
Look at ```example_ctest.cpp``` for example usage patterns

### 1. TO WRITE A TEST
```cpp
TEST(classname, testname) {
  ... // test body
}
```
### 2. TO WRITE ASSERTS
```cpp
ASSERT_EQ(x, y) // to fail if x != y
ASSERT_GT(x, y) // to fail if x <= y
ASSERT_LT(x, y) // to fail if x >= y
ASSERT_NE(x, y) // to fail if x == y
ASSERT_APPROX_EQ(x, y) // to fail if x and y differ by more than EPSILON
ASSERT_APPROX_EQE(x, y, 0.0001) // to fail if x and y differ by more than EPSILON
```
### 3. TO DEFINE MAX TESTS
Pass the below argument to your compiler, default MAXTESTS is 20
```bash
-DMAXTESTS=30 // if you want to set the max tests to 30
```
OR
```cpp
#define MAXTESTS 30  // define this before including tests.h
#include <ctest.h>
```
### 4. TO DEFINE TEST TIMEOUT 
Time is in unsigned int seconds (default: -1 no timeout)
```bash
-DTIMEOUT 2 // if you want test to timeout after 2 sec
```
OR
```cpp
#define TIMEOUT 2  // define this before including tests.h
#include <ctest.h>
```
### 5. TO SUPPRESS COLOR OUTPUT
Pass the below argument to your compiler, default is COLORS=1
```bash
-DCOLORS=0
```
OR
```cpp
#define COLORS 0 // define this before including tests.h
#include <tests.h>
```
### 6. TO SKIP TEST
Change the TEST macro to SKIPTEST like so
```cpp
TEST(classname, testname) {
}
```
TO
```cpp
SKIPTEST(classname, testanme) {
}
```

### 7. TO LOG
Use printfstyle args with below macros for different levels
```Cpp
LOG(...)
LOGINFO(...)
LOGWARN(...)
LOGERROR(...)
```
