add_library(cil SHARED ImageInfo.cpp)
add_library(cil_static STATIC ImageInfo.cpp)

set_target_properties(cil_static PROPERTIES OUTPUT_NAME cil)

if(${LIBPNG_DIR})
    add_subdirectory(PNG)
endif()

set(HEADERS ${CMAKE_SOURCE_DIR}/include)

target_include_directories(cil PUBLIC ${HEADERS})
target_include_directories(cil_static PUBLIC ${HEADERS})

install(TARGETS cil LIBRARY)
install(TARGETS cil_static ARCHIVE)