#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <blpconn_observer.h>

const char* datadir = "./data";

void processFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> fileBuffer(fileSize);
    file.read(reinterpret_cast<char*>(fileBuffer.data()), fileSize);
    std::cout << ">>> " << filename << std::endl;
    BlpConn::defaultObserver(fileBuffer.data(), fileSize);
    file.close();
}

int main() {
    if (chdir(datadir) == -1) {
        throw std::runtime_error("Error changing directory to debug directory");
    }
    DIR* dir = opendir(".");
    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL) {
        std::string filename(ent->d_name);
        if (filename.find("fb_") == 0 && filename.find(".bin") != std::string::npos) {
            processFile(filename); 
        }
    }
    closedir(dir);
}
