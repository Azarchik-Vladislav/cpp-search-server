#pragma once

#include <iostream>
#include <string>
#include <vector>

void AssertImpl(bool value, const std::string& expr_str, const std::string& file, const std::string& func, unsigned line,
                const std::string& hint);

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const std::string& t_str, const std::string& u_str, const std::string& file,
                     const std::string& func, unsigned line, const std::string& hint = "") {
    using namespace std::string_literals;

    if (t != u) {
        std::cerr << std::boolalpha;
        std::cerr << file << "("s << line << "): "s << func << ": "s;
        std::cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        std::cerr << t << " != "s << u << "."s;
        if (!hint.empty()) {
            std::cerr << " Hint: "s << hint;
        }
        std::cerr << std::endl;
        std::abort();
    }
}

template <typename TestFunc>
void RunTestImpl(const TestFunc& func, const std::string& test_name) {
    using namespace std::string_literals;

    func();
    std::cerr << test_name << " OK"s << std::endl;
}

template <typename T>
std::ostream& operator<<(std::ostream& output, const std::vector<T>& items) {
    using namespace std::string_literals;

    output << "["s;
    bool first_item = true;
    for (const T& item : items) {
        if (!first_item) {
            output << ", "s;
        }
        output << item;
        first_item = false;
    }
    output << "]"s;
    return output;
}

#define RUN_TEST(func) RunTestImpl(func, #func)

#define ASSERT(a) AssertImpl((a), #a, __FILE__, __FUNCTION__, __LINE__, "")

#define ASSERT_HINT(a, hint) AssertImpl((a), #a, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, "")

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))