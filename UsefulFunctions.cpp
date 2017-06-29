//
// Created by benji on 29.06.17.
//

#include "UsefulFunctions.h"
bool fileExists(std::string filename) {
    struct stat buf;
    return (stat(filename.c_str(), &buf) == 0);

}

bool directoryExists(std::string dirname) {
    struct stat buf;
    return (stat(dirname.c_str(), &buf) == 0 && S_ISDIR(buf.st_mode));
}
