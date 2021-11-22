add_library(cil ImageInfo.cpp)

target_include_directories(cil PUBLIC ${CMAKE_SOURCE_DIR}/include)

if(LIBPNG_DIR)
    add_subdirectory(PNG)
endif()

install(TARGETS cil 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)