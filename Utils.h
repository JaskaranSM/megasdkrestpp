//
// Created by zerocool on 12/11/22.
//

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include <string>
#include <unistd.h>


class Utils {
public:
    bool IsMegaFolder(const char* link);
    std::string GenerateRandomID(const int len);
};



#endif //PROJECT_UTILS_H
