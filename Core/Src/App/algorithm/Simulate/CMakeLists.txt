

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
include(../FusionEngin.cmake)
file(GLOB_RECURSE
        SOURCE
        ../../algorithm/*.c
        ../../algorithm/*.cpp
)
include_directories(
        ../../algorithm/
        ../../algorithm/Mahony
        ../../algorithm/Simulate
        )
add_executable(FusionSim ${SOURCE})