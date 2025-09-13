#pragma once
#include <string>
namespace Security {
    // NOTE: This is a placeholder hashing function using std::hash.
    // Replace with a real SHA-256 implementation (picosha2) before production/submission.
    std::string hash_password(const std::string &pwd);
}
