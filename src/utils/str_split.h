#ifndef WOW_STR_SPLIT_H
#define WOW_STR_SPLIT_H

#include <vector>
#include <string>
#include <algorithm>

std::vector<std::string> split(std::string str) {
    std::vector<std::string> ans;
    int last = 0;
    while (last < str.size()) {
        int now = std::find(str.begin() + last, str.end(), '\n') - str.begin();
        ans.push_back(str.substr(last, now - last));
        last = now + 1;
    }
    return ans;
}

#endif //WOW_STR_SPLIT_H
