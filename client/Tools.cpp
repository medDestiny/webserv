#include "Client.hpp"
#include <sys/stat.h>

size_t stringToInt(const std::string& str) {
	std::istringstream iss(str);
	size_t result;
	iss >> result;
	return result;
}

std::string intToString(size_t num) {
	std::ostringstream oss;
	oss << num;
	return oss.str();
}

size_t get_size_fd(std::string fileD) {
	struct stat fileStat;

	if (lstat(fileD.c_str(), &fileStat) == -1) {
		std::cout << "error getSize\n";
		return (-1);
	}
	return (fileStat.st_size);
}

bool isDirectory(std::string path) {
    struct stat pathStat;
    if (lstat(path.c_str(), &pathStat) != 0) {
		std::cout << "error isDirectory\n";
        return false;
    }
    return S_ISDIR(pathStat.st_mode);
}

bool isRegularFile(const char* path) {
    struct stat pathStat;
    if (stat(path, &pathStat) != 0) {
		std::cout << "error isFile\n";
        return false;
    }
    return S_ISREG(pathStat.st_mode);
}