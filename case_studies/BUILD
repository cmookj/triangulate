load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
  name = "random_polygon",
  srcs = ["random_polygon.cc"],
  deps = [
    "//core:core",
  ],
  copts = select({
    "@bazel_tools//src/conditions:windows": ["/std:c++20"],
    "//conditions:default": ["-std=c++20"],
  }),
)

cc_binary(
  name = "triangulate",
  srcs = ["triangulate.cc"],
  deps = [
    "//core:core",
  ],
  copts = select({
    "@bazel_tools//src/conditions:windows": ["/std:c++20"],
    "//conditions:default": ["-std=c++20"],
  }),
)
