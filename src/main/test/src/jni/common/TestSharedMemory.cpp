#include "catch.hpp"
#include "fakeit.hpp"
#include "SharedMemory.h"

TEST_CASE("Creates the first shared memory.") {
    int parent_native_data[100];

    SharedMemory shm1 = SharedMemory::createSharedMemory(parent_native_data, sizeof(parent_native_data));

    THEN("finds shared memory successfully.") {
        REQUIRE(shm1.getShmId().size() > 0);
        REQUIRE(shm1.getRemoteAddressForRef() == parent_native_data);
        REQUIRE(shm1.getAddress() != nullptr);
        REQUIRE(shm1.getSize() == sizeof(parent_native_data));
    }

    SECTION("finds shared memory failed.") {
        REQUIRE_THROWS(SharedMemory::findSharedMemory(parent_native_data + 10));
    }

    SECTION("finds shared memory successfully.") {
        SharedMemory shm2 = SharedMemory::findSharedMemory(shm1.getAddress());
        REQUIRE(shm2.getShmId() == shm1.getShmId());
        REQUIRE(shm2.getRemoteAddressForRef() == parent_native_data);
        REQUIRE(shm2.getAddress() != nullptr);
        REQUIRE(shm2.getSize() == sizeof(parent_native_data));
    }

    SECTION("free shared memory successfully.") {
        shm1.freeSharedMemory();
        REQUIRE_THROWS(SharedMemory::findSharedMemory(shm1.getAddress()));
    }

}

TEST_CASE("Creates shared memories and access its content by shared memory key.") {
    int parent_native_data[100];

    SharedMemory shm1 = SharedMemory::createSharedMemory(parent_native_data, sizeof(parent_native_data));
    SharedMemory shm2 = SharedMemory::loadSharedMemory(shm1.getShmId(), shm1.getAddress(), sizeof(parent_native_data));

    int* shm1Buffer = (int*) shm1.getAddress();
    int* shm2Buffer = (int*) shm2.getAddress();

    REQUIRE_FALSE(shm1Buffer == shm2Buffer);

    for (unsigned int i = 0; i < sizeof(parent_native_data); i ++) {
        shm1Buffer[i] = i;
        REQUIRE(shm2Buffer[i] == i);
        shm1Buffer[i] = i + 1;
        REQUIRE(shm2Buffer[i] == i + 1);
    }
}

TEST_CASE("Creates more than one shared memory and find it by address") {
    int parent_native_data[100];

    SharedMemory shm1 = SharedMemory::createSharedMemory(parent_native_data, sizeof(parent_native_data));
    SharedMemory shm2 = SharedMemory::createSharedMemory(parent_native_data, sizeof(parent_native_data));
    SharedMemory shm3 = SharedMemory::findSharedMemory(shm1.getAddress());

    REQUIRE(shm3.getShmId() == shm1.getShmId());
    REQUIRE(shm3.getRemoteAddressForRef() == parent_native_data);
    REQUIRE(shm3.getAddress() != nullptr);
    REQUIRE(shm3.getSize() == sizeof(parent_native_data));

    REQUIRE(shm2.getRemoteAddressForRef() == parent_native_data);
    REQUIRE(shm3.getAddress() != shm2.getAddress());
}
