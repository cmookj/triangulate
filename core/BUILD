load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "core",
    srcs = [
        "fileio.cc",
        "geometry.cc",
        "numeric.cc",
        "polygon.cc",
        "types.cc",
    ],
    hdrs = [
        "fileio.h",
        "geometry.h",
        "numeric.h",
        "polygon.h",
        "random.h",
        "types.h",
    ],
    deps = [],
    visibility = ["//visibility:public"],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"],
    }),
)

