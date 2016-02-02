cmake_minimum_required(VERSION 3.0)
project(Crypto_Project)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Source Files
set(TESTING_SOURCE_FILES ap_int_tests.c ap_int.c ap_int.h utils.c utils.h)
set(SOURCE_FILES crypto_project.c ap_int.c ap_int.h utils.c utils.h)
#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

#Normal executable
add_executable(Crypto_Project ${SOURCE_FILES})
target_link_libraries(Crypto_Project ${OPENSSL_LIBRARIES})

#Testing executable
find_package(CMocka REQUIRED)
add_executable(Test_Crypto_Project ${TESTING_SOURCE_FILES})
target_link_libraries(Test_Crypto_Project ${CMOCKA_LIBRARIES} ${OPENSSL_LIBRARIES})