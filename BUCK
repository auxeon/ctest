# constants
COMPATIBLE_WITH = [
    "config//os:macos",
    "config//os:linux",
    "config//os:windows",
]

# cxx
cxx_library(
  name="libctest",
  compatible_with=COMPATIBLE_WITH,
  headers=["ctest.h"],
  link_style="static",
  deps=[],
)

cxx_test(
  name="example_ctest",
  compatible_with=COMPATIBLE_WITH,
  link_style="static",
  srcs=["example_ctest.cpp"],
  deps=[":libctest"],
)

cxx_test(
  name="example_gtest",
  compatible_with=COMPATIBLE_WITH,
  link_style="static",
  srcs=["example_gtest.cpp"],
  deps=["//deps:gtest"],
)
