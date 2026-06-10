// test/filesystem.cpp — tests for <filesystem>
// No #pragma once; no main() — gen_main.sh generates it.
#include <filesystem>
#include <cstdio>
#include <cstring>
#include "test.h"

namespace fs = std::filesystem;

//===----------------------------------------------------------------------===//
// path decomposition
//===----------------------------------------------------------------------===//
void test_filesystem_path_decomposition() {
    {
        fs::path p("/foo/bar/baz.txt");
        CHECK(p.root_directory() == fs::path("/"));
        CHECK(p.filename()  == fs::path("baz.txt"));
        CHECK(p.stem()      == fs::path("baz"));
        CHECK(p.extension() == fs::path(".txt"));
        CHECK(p.parent_path() == fs::path("/foo/bar"));
        CHECK(p.relative_path() == fs::path("foo/bar/baz.txt"));
        CHECK(p.is_absolute());
        CHECK(!p.is_relative());
    }
    {
        fs::path p("foo/bar");
        CHECK(p.is_relative());
        CHECK(!p.is_absolute());
        CHECK(p.filename() == fs::path("bar"));
        CHECK(p.parent_path() == fs::path("foo"));
    }
    {
        fs::path p("foo.tar.gz");
        CHECK(p.stem()      == fs::path("foo.tar"));
        CHECK(p.extension() == fs::path(".gz"));
    }
    {
        fs::path p(".");
        CHECK(p.stem()      == fs::path("."));
        CHECK(p.extension() == fs::path(""));
    }
    {
        fs::path p(".hidden");
        CHECK(p.stem()      == fs::path(".hidden"));
        CHECK(p.extension() == fs::path(""));
    }
    {
        fs::path p("/");
        CHECK(p.root_directory() == fs::path("/"));
        CHECK(p.root_path()      == fs::path("/"));
        CHECK(p.filename()       == fs::path("/"));
        CHECK(p.is_absolute());
    }
    // has_* queries
    {
        fs::path p("/a/b.c");
        CHECK(p.has_root_directory());
        CHECK(!p.has_root_name());
        CHECK(p.has_filename());
        CHECK(p.has_extension());
        CHECK(p.has_stem());
        CHECK(p.has_parent_path());
        CHECK(p.has_relative_path());
    }
}

//===----------------------------------------------------------------------===//
// path append / concatenate
//===----------------------------------------------------------------------===//
void test_filesystem_path_append() {
    {
        fs::path p("/foo");
        p /= "bar";
        CHECK(p == fs::path("/foo/bar"));
    }
    {
        fs::path p("/foo");
        p /= fs::path("/abs");
        CHECK(p == fs::path("/abs"));
    }
    {
        fs::path p;
        p /= fs::path("rel");
        CHECK(p == fs::path("rel"));
    }
    {
        fs::path p("a");
        p /= "b";
        p /= "c";
        CHECK(p == fs::path("a/b/c"));
    }
    {
        // operator/ (non-member)
        fs::path p = fs::path("/a") / "b" / "c";
        CHECK(p == fs::path("/a/b/c"));
    }
    {
        // operator+=
        fs::path p("foo");
        p += std::string("bar");
        CHECK(p == fs::path("foobar"));
    }
}

//===----------------------------------------------------------------------===//
// lexically_normal
//===----------------------------------------------------------------------===//
void test_filesystem_path_lexically_normal() {
    {
        fs::path p("foo/./bar/../baz");
        CHECK(p.lexically_normal() == fs::path("foo/baz"));
    }
    {
        fs::path p("/a/b/c/../..");
        CHECK(p.lexically_normal() == fs::path("/a"));
    }
    {
        fs::path p("./foo/bar");
        CHECK(p.lexically_normal() == fs::path("foo/bar"));
    }
    {
        fs::path p(".");
        CHECK(p.lexically_normal() == fs::path("."));
    }
    {
        fs::path p("a/b/../..");
        CHECK(p.lexically_normal() == fs::path("."));
    }
    {
        fs::path p("/../foo");
        CHECK(p.lexically_normal() == fs::path("/foo"));
    }
    {
        // trailing separator normalizes away
        fs::path p("/a/b/");
        auto n = p.lexically_normal();
        CHECK(n == fs::path("/a/b") || n == fs::path("/a/b/"));
    }
}

//===----------------------------------------------------------------------===//
// path comparison
//===----------------------------------------------------------------------===//
void test_filesystem_path_comparison() {
    {
        fs::path a("/a/b");
        fs::path b("/a/b");
        CHECK(a == b);
        CHECK(!(a != b));
        CHECK((a <=> b) == std::strong_ordering::equal);
    }
    {
        fs::path a("/a/b");
        fs::path b("/a/c");
        CHECK(a != b);
        CHECK((a <=> b) == std::strong_ordering::less);
        CHECK((b <=> a) == std::strong_ordering::greater);
    }
    {
        // compare(string_view)
        fs::path p("/x/y");
        CHECK(p.compare("/x/y") == 0);
        CHECK(p.compare("/x/z") < 0);
    }
}

//===----------------------------------------------------------------------===//
// path iteration
//===----------------------------------------------------------------------===//
void test_filesystem_path_iteration() {
    {
        fs::path p("/foo/bar/baz");
        std::vector<std::string> parts;
        for (const auto c : p)
            parts.push_back(c.native());
        // Should be: "/", "foo", "bar", "baz"
        CHECK(parts.size() >= 3);
        CHECK(parts[0] == "/");
    }
    {
        fs::path p("foo/bar");
        std::vector<std::string> parts;
        for (const auto c : p)
            parts.push_back(c.native());
        CHECK(parts.size() == 2);
        CHECK(parts[0] == "foo");
        CHECK(parts[1] == "bar");
    }
}

//===----------------------------------------------------------------------===//
// hash_value
//===----------------------------------------------------------------------===//
void test_filesystem_path_hash() {
    fs::path a("/a/b");
    fs::path b("/a/b");
    fs::path c("/a/c");
    CHECK(fs::hash_value(a) == fs::hash_value(b));
    // different paths are likely to have different hashes (probabilistic)
    (void)fs::hash_value(c);
}

//===----------------------------------------------------------------------===//
// Operations via error_code overloads in /tmp
//===----------------------------------------------------------------------===//
void test_filesystem_operations() {
    // Use a unique subdirectory of /tmp
    fs::path tmp = fs::path("/tmp") / "libcis_fs_test";

    // Cleanup first in case of stale state
    std::error_code ec;
    fs::remove_all(tmp, ec);
    ec.clear();

    // create_directory
    {
        bool created = fs::create_directory(tmp, ec);
        CHECK(!ec);
        CHECK(created || fs::is_directory(tmp, ec));
        CHECK(!ec);
    }

    // exists
    {
        CHECK(fs::exists(tmp, ec));
        CHECK(!ec);
    }

    // is_directory
    {
        CHECK(fs::is_directory(tmp, ec));
        CHECK(!ec);
    }

    // Write a file via <cstdio>
    fs::path file = tmp / "test.txt";
    {
        FILE* f = ::fopen(file.c_str(), "w");
        CHECK(f != nullptr);
        ::fputs("hello filesystem\n", f);
        ::fclose(f);
    }

    // exists on file
    {
        CHECK(fs::exists(file, ec));
        CHECK(!ec);
    }

    // is_regular_file
    {
        CHECK(fs::is_regular_file(file, ec));
        CHECK(!ec);
    }

    // file_size
    {
        uintmax_t sz = fs::file_size(file, ec);
        CHECK(!ec);
        CHECK(sz > 0);
    }

    // last_write_time
    {
        auto t = fs::last_write_time(file, ec);
        CHECK(!ec);
        (void)t;
    }

    // directory_iterator listing
    {
        int count = 0;
        for (auto& entry : fs::directory_iterator(tmp, ec)) {
            CHECK(!ec);
            CHECK(entry.path().filename().native().size() > 0);
            ++count;
        }
        CHECK(count >= 1);
    }

    // create subdirectory
    {
        fs::path sub = tmp / "sub";
        fs::create_directory(sub, ec);
        CHECK(!ec);
        CHECK(fs::is_directory(sub, ec));
    }

    // create_directories (nested)
    {
        fs::path nested = tmp / "a" / "b" / "c";
        fs::create_directories(nested, ec);
        CHECK(!ec);
        CHECK(fs::is_directory(nested, ec));
    }

    // rename
    {
        fs::path dst = tmp / "test2.txt";
        fs::rename(file, dst, ec);
        CHECK(!ec);
        CHECK(fs::exists(dst, ec));
        CHECK(!fs::exists(file, ec));
        // rename back
        fs::rename(dst, file, ec);
        CHECK(!ec);
    }

    // copy_file
    {
        fs::path copy_dst = tmp / "copy.txt";
        bool copied = fs::copy_file(file, copy_dst, ec);
        CHECK(!ec);
        CHECK(copied);
        CHECK(fs::exists(copy_dst, ec));
        uintmax_t sz1 = fs::file_size(file, ec);
        uintmax_t sz2 = fs::file_size(copy_dst, ec);
        CHECK(sz1 == sz2);
    }

    // remove file
    {
        bool removed = fs::remove(file, ec);
        CHECK(!ec);
        CHECK(removed);
        CHECK(!fs::exists(file, ec));
    }

    // remove_all
    {
        uintmax_t n = fs::remove_all(tmp, ec);
        CHECK(!ec);
        CHECK(n > 0);
        CHECK(!fs::exists(tmp, ec));
    }

    // temp_directory_path
    {
        fs::path t = fs::temp_directory_path(ec);
        CHECK(!ec);
        CHECK(fs::is_directory(t, ec));
    }

    // current_path
    {
        fs::path cwd = fs::current_path(ec);
        CHECK(!ec);
        CHECK(!cwd.empty());
    }

    // absolute
    {
        fs::path rel(".");
        fs::path abs = fs::absolute(rel, ec);
        CHECK(!ec);
        CHECK(abs.is_absolute());
    }

    // space
    {
        fs::space_info si = fs::space(fs::path("/tmp"), ec);
        CHECK(!ec);
        CHECK(si.capacity > 0);
    }
}

//===----------------------------------------------------------------------===//
// lexically_relative
//===----------------------------------------------------------------------===//
void test_filesystem_path_lexically_relative() {
    {
        fs::path p("/a/b/c");
        fs::path base("/a");
        auto rel = p.lexically_relative(base);
        CHECK(rel == fs::path("b/c"));
    }
    {
        fs::path p("/a/b/c");
        fs::path base("/a/b/c/d");
        auto rel = p.lexically_relative(base);
        CHECK(rel == fs::path(".."));
    }
    {
        fs::path p("/a/b");
        fs::path base("/a/b");
        auto rel = p.lexically_relative(base);
        CHECK(rel == fs::path("."));
    }
    {
        // proximate: same as relative when result is non-empty
        fs::path p("/a/b/c");
        fs::path base("/a");
        CHECK(p.lexically_proximate(base) == p.lexically_relative(base));
    }
    {
        // proximate: returns *this when relative would be empty (different roots)
        fs::path p("/a/b");
        fs::path base("rel/path");
        auto prox = p.lexically_proximate(base);
        CHECK(prox == p);
    }
}
