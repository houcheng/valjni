//
// Created by houcheng on 07/10/17.
//

#ifndef PROJECT_SHAREDMEMORY_H
#define PROJECT_SHAREDMEMORY_H

#include <string>
#include <set>
#include <unordered_map>
#include "json.hpp"

using std::string;
using std::set;
using std::unordered_map;
using JSON = nlohmann::json;

class SharedMemory {
public:
    static const SharedMemory createSharedMemory(void* remoteAddressForRef1, int size);
    static const SharedMemory loadSharedMemory(string shmId1, void* remoteAddressForRef1, int size);
    static const SharedMemory& findSharedMemory(void* address);

    void freeSharedMemory();

    SharedMemory(const JSON& json);
    SharedMemory() = default;
    ~SharedMemory() = default;
    const JSON getJson() const;
private:
    static set<string> shmidSet;
    static unordered_map<void*, SharedMemory> sharedMemoryRepository;

    string shmId = "";
    int size = 0;
    void* address = nullptr;
    void* remoteAddressForRef = nullptr;

    static string createShmId();
    static void* loadUnixSharedMemory(string shmId1, int size1);
    static void *createUnixSharedMemory(string shmId1, int size1);
    static void freeUnixSharedMemory(string shmId1);

    SharedMemory(string shmid1, void* remoteAddressForRef1, void* address, int size1);

public:
    const string &getShmId() const;
    int getSize() const;
    void* getAddress() const;
    void* getRemoteAddressForRef() const;
};

#endif //PROJECT_SHAREDMEMORY_H
