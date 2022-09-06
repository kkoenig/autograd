load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_library(
    name = "autograd",
    include_prefix = "autograd",
    strip_include_prefix = "include",
    srcs = [
        "lib/graph.cc",
    ],
    hdrs = [
        "include/autograd.h",
    ],
    deps = ["@fmt//:lib"],
)

cc_binary(
    name = "demo",
    srcs = [
        "main.cc",
    ],
    deps = [
        "autograd",
        "@fmt//:lib",
    ],
)

cc_test(
    name = "basic",
    srcs = ["tests/basic.cc"],
    deps = [
        "autograd",
        "@com_google_googletest//:gtest_main",
    ],
)
