# include sub directories
include_directories(${PROJECT_SOURCE_DIR}/include/base)
include_directories(${PROJECT_SOURCE_DIR}/include/config)
include_directories(${PROJECT_SOURCE_DIR}/include/database)
include_directories(${PROJECT_SOURCE_DIR}/include/device)
include_directories(${PROJECT_SOURCE_DIR}/include/log)
include_directories(${PROJECT_SOURCE_DIR}/include/peripherals)
include_directories(${PROJECT_SOURCE_DIR}/include/network)
include_directories(${PROJECT_SOURCE_DIR}/include/utility)
include_directories(${PROJECT_SOURCE_DIR}/include/core/upperport)
include_directories(${PROJECT_SOURCE_DIR}/include/core)

# add compile tree
add_subdirectory(config)
add_subdirectory(database)
add_subdirectory(device)
add_subdirectory(log)
add_subdirectory(peripherals)
add_subdirectory(network)
add_subdirectory(utility)

# all source files
file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c")

#set(ARM ON)
if (${ARM})
    message(STATUS "----arm")
    #set(ARM_CROSS /home/mach/tools/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin)
    set(ARM_CROSS /tool/gcc_linaro/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin)
    set(CMAKE_CXX_COMPILER ${ARM_CROSS}/aarch64-linux-gnu-g++)
    set(THIRD_LIB_PATH ${PROJECT_SOURCE_DIR}/lib/arm)
else ()
    message(STATUS "----x86")
    set(THIRD_LIB_PATH ${PROJECT_SOURCE_DIR}/lib/x86)
endif ()
message(STATUS "third_lib_path:" ${THIRD_LIB_PATH})

include_directories(${THIRD_LIB_PATH}/rapidjson-1.1.0/include)

# create the executable
set(EXECUTABLE_NAME SimulatorMidware)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/output)

# execute file 
add_executable(${EXECUTABLE_NAME} ${ALL_SOURCE})