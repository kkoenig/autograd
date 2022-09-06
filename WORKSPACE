load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "fmt",
    build_file_content = """
cc_library(
    name = "lib",
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    hdrs = [
        "include/fmt/core.h",
        "include/fmt/format.h",
        "include/fmt/format-inl.h",
        "include/fmt/color.h",
        "include/fmt/ostream.h",
    ],
    srcs = ["src/format.cc"],
)
""",
    sha256 = "cceb4cb9366e18a5742128cb3524ce5f50e88b476f1e54737a47ffdf4df4c996",
    strip_prefix = "fmt-9.1.0",
    url = "https://github.com/fmtlib/fmt/releases/download/9.1.0/fmt-9.1.0.zip",
)

http_archive(
    name = "com_google_googletest",
    strip_prefix = "googletest-0e0d9feefab1b51aaab9dfd70132e93c0b6964e5",
    urls = ["https://github.com/google/googletest/archive/0e0d9feefab1b51aaab9dfd70132e93c0b6964e5.zip"],
)
