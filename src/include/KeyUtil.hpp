#pragma once

#include <string>
#include <vector>
#include <algorithm>

inline std::string MakeKey(const std::string& op, std::vector<int> args){
    if ((op == "+" || op == "*") && args.size() == 2) {
        if (args[0] > args[1]) {
            std::swap(args[0], args[1]);
        }
    }
    std::string key = std::to_string(args[0]) +op;
    if (args.size() > 1) {
        key += std::to_string(args[1]);
    }
    return key;
}