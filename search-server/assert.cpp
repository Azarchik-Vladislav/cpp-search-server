#include "assert.h"

using namespace std::literals::string_literals;

void AssertImpl(bool value, const std::string& expr_str, const std::string& file, const std::string& func, unsigned line,
                const std::string& hint) {
    if(!value) {
        std::cerr << file << "("s << line << "): "s << func << ": "s;
        std::cerr << "Assert("s << expr_str << ") failed."s;
        if(!hint.empty()) {
            std::cerr << " Hint: "s << hint;
        }
        std::cerr << std::endl;
        abort();
    }
}




