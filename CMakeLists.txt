cmake_minimum_required(VERSION 3.6)
project(RecursiveRWSpinlock)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(SOURCES RecursiveRWSpinlock.h RecursiveRWSpinlock.inl Test.h Test.cpp)

if(DEFINED ANDROID_PLATFORM)
    set (SOURCES ${SOURCES} TestJni.h TestJni.cpp)
        if(NOT DEFINED CMAKE_BUILD_TYPE)
            set (CMAKE_BUILD_TYPE RelWithDebInfo)
        endif()
endif()

add_library(${PROJECT_NAME}_LIB SHARED ${SOURCES})

if(NOT DEFINED ANDROID_PLATFORM)
    add_executable(${PROJECT_NAME}  Test.h main_x64.cpp )
    target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_LIB Threads::Threads)
endif()
