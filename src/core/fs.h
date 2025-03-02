#pragma once
#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <StringUtils.h>

#ifdef ESP8266
#include <SDFS.h>
#define SD_FS SDFS
#else  // ESP32
#include <SD.h>
#define SD_FS SD
#endif

namespace sets {

class FSWrapper {
public:
    virtual ~FSWrapper() = default;

    FS& fs;

    explicit FSWrapper(FS& fs) : fs(fs) {}

    void listDir(String& str, const char* path, bool withSize = false) {
#ifdef ESP8266
        Dir dir = fs.openDir(path);
        while (dir.next()) {
            if (dir.isDirectory()) {
                String p(path);
                p += dir.fileName();
                p += '/';
                Dir sdir = fs.openDir(p);
                listDir(str, p.c_str(), withSize);
            }
            if (dir.isFile() && dir.fileName().length()) {
                str += path;
                str += dir.fileName();
                if (withSize) {
                    str += ':';
                    str += dir.fileSize();
                }
                str += ';';
            }
        }

#else  // ESP32
        File root = fs.open(path);
        if (!root || !root.isDirectory()) return;
        File file;
        while (file = root.openNextFile()) {
            if (file.isDirectory()) {
                listDir(str, file.path(), withSize);
            } else {
                str += '/';
                str += file.name();
                if (withSize) {
                    str += ':';
                    str += file.size();
                }
                str += ';';
            }
        }
#endif
    }

    [[nodiscard]] bool remove(const Text& path) const {
        return fs.remove(path.c_str());
    }

    [[nodiscard]] File openRead(const Text& path) const {
        return fs.open(path.c_str(), "r");
    }

    [[nodiscard]] File openWrite(const Text& path) const {
        mkdir(path);
        return fs.open(path.c_str(), "w");
    }

    void mkdir(const Text& path) const {
#ifdef ESP32
        Text::Cstr path = tpath.c_str();
        if (!fs.exists(path)) {
            if (strchr(path, '/')) {
                char* pathStr = strdup(path);
                if (pathStr) {
                    char* ptr = strchr(pathStr, '/');
                    while (ptr) {
                        *ptr = 0;
                        fs.mkdir(pathStr);
                        *ptr = '/';
                        ptr = strchr(ptr + 1, '/');
                    }
                }
                free(pathStr);
            }
        }
#else
#endif
    }

    void rmdir(const Text& path) const {
#ifdef ESP32
        char* pathStr = strdup(path.c_str());
        if (pathStr) {
            char* ptr = strrchr(pathStr, '/');
            while (ptr) {
                *ptr = 0;
                fs.rmdir(pathStr);
                ptr = strrchr(pathStr, '/');
            }
            free(pathStr);
        }
#else
#endif
    }

    virtual uint64_t freeSpace() = 0;
    virtual uint64_t totalSpace() = 0;
    virtual uint64_t usedSpace() = 0;
};

class LittleFSWrapperClass final : public FSWrapper {
public:
    LittleFSWrapperClass() : FSWrapper(LittleFS) {}

    uint64_t freeSpace() override {
#ifdef ESP8266
        FSInfo64 fs_info;
        LittleFS.info64(fs_info);
        return fs_info.totalBytes - fs_info.usedBytes;
#else
        return LittleFS.totalBytes() - LittleFS.usedBytes();
#endif
    }

    uint64_t totalSpace() override {
#ifdef ESP8266
        FSInfo64 fs_info;
        LittleFS.info64(fs_info);
        return fs_info.totalBytes;
#else
        return LittleFS.totalBytes();
#endif
    }

    uint64_t usedSpace() override {
#ifdef ESP8266
        FSInfo64 fs_info;
        LittleFS.info64(fs_info);
        return fs_info.usedBytes;
#else
        return LittleFS.usedBytes();
#endif
    }
};

extern LittleFSWrapperClass LittleFSWrapper;

class SDFSWrapperClass final : public FSWrapper {
public:
    SDFSWrapperClass() : FSWrapper(SD_FS) {}

    uint64_t freeSpace() override {
#ifdef ESP8266
        FSInfo info;
        SD_FS.info(info);
        return info.totalBytes - info.usedBytes;
#else
        return SD_FS.totalBytes() - SD_FS.usedBytes();
#endif
    }

    uint64_t totalSpace() override {
#ifdef ESP8266
        FSInfo info;
        SD_FS.info(info);
        return info.totalBytes;
#else
        return SD_FS.totalBytes();
#endif
    }

    uint64_t usedSpace() override {
#ifdef ESP8266
        FSInfo info;
        SD_FS.info(info);
        return info.usedBytes;
#else
        return SD_FS.usedBytes();
#endif
    }
};

extern SDFSWrapperClass SDFSWrapper;

}  // namespace sets