load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
  name = "triangulate",
  srcs = ["main.cc"],
  deps = [
    "//core:core",
  ],
  copts = select({
    "@bazel_tools//src/conditions:windows": ["/std:c++20"],
    "//conditions:default": ["-std=c++20"],
  }),
)
