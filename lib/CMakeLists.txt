add_library(cil ImageHandler.cpp ImageInfo.cpp ImageMatrix.cpp Pixel.cpp Transformations.cpp Drawing.cpp ThreadHandler.cpp Color.cpp)

target_include_directories(cil PUBLIC ${CMAKE_SOURCE_DIR}/include)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(cil PUBLIC Threads::Threads)

if (CIL_ENABLE_JPEG)
    target_compile_definitions(cil PUBLIC CIL_JPEG_ENABLED)
endif()

if (CIL_ENABLE_PNG)
    target_compile_definitions(cil PUBLIC CIL_PNG_ENABLED)
endif()

add_subdirectory(PPM)

if(CIL_ENABLE_PNG)
    add_subdirectory(PNG)
endif()

if(CIL_ENABLE_JPEG)
    add_subdirectory(JPEG)
endif()

add_subdirectory(Core)

install(TARGETS cil
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
