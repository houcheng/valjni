#include "SharedMemory.h"
#include "JniRawValue.h"
#include <sys/mman.h>


set<string> SharedMemory::shmidSet;
unordered_map<void*, SharedMemory> SharedMemory::sharedMemoryRepository;

string SharedMemory::createShmId() {
    std::string newShmid;
    do {
        newShmid = std::to_string(getpid()) + "::" + std::to_string(rand());
    } while ( shmidSet.find(newShmid) != shmidSet.end());

    shmidSet.insert(newShmid);
    return newShmid;
}

const SharedMemory SharedMemory::createSharedMemory(void *remoteAddressForRef1, int size1) {
    string shmId1 = createShmId();
    void* address = createUnixSharedMemory(shmId1, size1);
    SharedMemory sharedMemory(shmId1, remoteAddressForRef1, address, size1);
    sharedMemoryRepository[address] = sharedMemory;
    return sharedMemory;
}

const SharedMemory SharedMemory::loadSharedMemory(string shmId1, void *remoteAddressForRef1, int size1) {
    void* address = loadUnixSharedMemory(shmId1, size1);
    SharedMemory sharedMemory(shmId1, remoteAddressForRef1, address, size1);
    sharedMemoryRepository[address] = sharedMemory;
    return sharedMemory;
}

const SharedMemory &SharedMemory::findSharedMemory(void* address) {
    return sharedMemoryRepository.at(address);
}

void SharedMemory::freeSharedMemory() {
    sharedMemoryRepository.erase(address);
    shmidSet.erase(shmId);
    freeUnixSharedMemory(shmId);
}


SharedMemory::SharedMemory(string shmid1, void* remoteAddressForRef1, void* address1, int size1) {
    shmId = shmid1;
    remoteAddressForRef = remoteAddressForRef1;
    address = address1;
    size = size1;
}

void *SharedMemory::getAddress() const {
    return address;
}

void *SharedMemory::getRemoteAddressForRef() const {
    return remoteAddressForRef;
}

int SharedMemory::getSize() const {
    return size;
}

const string &SharedMemory::getShmId() const {
    return shmId;
}

void* SharedMemory::loadUnixSharedMemory(string shmId1, int size1) {
    int shmFd = shm_open(shmId1.c_str(), O_RDWR, 0666);
    ftruncate(shmFd, size1);
    void* sharedMemoryPointer = mmap(0, size1, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if (sharedMemoryPointer == MAP_FAILED) {
        throw JniException("Load share memory " + shmId1 + " and the mmap failed.");
    }
    return sharedMemoryPointer;
}

void* SharedMemory::createUnixSharedMemory(string shmId1, int size1) {
    int shm_fd = shm_open(shmId1.c_str(), O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        throw JniException("Open share memory " + shmId1 + " failed.");
    }
    ftruncate(shm_fd, size1);
    void* sharedMemoryPointer = mmap(0, size1, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedMemoryPointer == MAP_FAILED) {
        throw JniException("Create share memory " + shmId1 + " and the mmap failed.");
    }
    if (sharedMemoryPointer == nullptr) {
        throw JniException("Create share memory " + shmId1 + " and the mmap got null.");
    }

    return sharedMemoryPointer;
}

void SharedMemory::freeUnixSharedMemory(string shmId1) {
    shm_unlink(shmId1.c_str());
}

SharedMemory::SharedMemory(const JSON& json) : SharedMemory() {
    shmId = json["shmId"];
    size = json["size"].get<int>();
    address = ConvertToNativePointer(json["address"].get<string>());
    remoteAddressForRef = ConvertToNativePointer(json["remoteAddressForRef"].get<string>());
}

const JSON SharedMemory::getJson() const {
    const JSON json = {
            { "address", ConvertPointerToString(address) },
            { "shmId", shmId },
            { "size", size },
            { "remoteAddressForRef", ConvertPointerToString(remoteAddressForRef) },
    };
    return json;
}
