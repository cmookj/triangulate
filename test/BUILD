load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "random_gen",
    hdrs = ["random_gen.h"],
    visibility = ["//visibility:public"],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"],
    }),
)

cc_test(
    name = "basic_test",
    size = "small",
    srcs = ["basic_test.cc"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//core:core",
        "//test:random_gen",
    ],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"],
    }),
)

