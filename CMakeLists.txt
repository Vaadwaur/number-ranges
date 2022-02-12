cmake_minimum_required(VERSION 3.12...3.20)
project(range_tools VERSION "" LANGUAGES CXX)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(MY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# First for the generic no-config case (e.g. with mingw)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MY_OUTPUT_DIRECTORY}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MY_OUTPUT_DIRECTORY}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MY_OUTPUT_DIRECTORY}/lib)
# Second, for multi-config builds (e.g. msvc)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${MY_OUTPUT_DIRECTORY}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${MY_OUTPUT_DIRECTORY}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${MY_OUTPUT_DIRECTORY}/lib")
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	list(APPEND EXTRA_COMPILE_WARNINGS
		"-Wall"
		"-Wextra"
		"-pedantic"
		"-Wdeprecated"
		"-Wnull-dereference"
		"-Wdouble-promotion"
		"-Wshadow"
		"-Wformat=2"
		"-Wold-style-cast"
		"-Wcast-align"
		"-Wunused"
		"-Wunused-const-variable"
		"-Woverloaded-virtual"
		"-Wconversion"
		"-Wsign-conversion"
	)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		list(APPEND EXTRA_COMPILE_WARNINGS
			"-Wglobal-constructors"
			"-Weffc++"
		)
		add_compile_options(-stdlib=libc++)
		set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++")
	endif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

    add_compile_options(-fno-rtti)
endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

set(SOURCE_DIR src)
set(IMPLODE_SRC
	${SOURCE_DIR}/implode.cpp
	${SOURCE_DIR}/common.hpp
)
source_group(src FILES ${IMPLODE_SRC})
add_executable(implode ${IMPLODE_SRC})
target_compile_features(implode PRIVATE cxx_std_17)
if(MSVC)
	target_link_libraries(implode ws2_32)
endif(MSVC)
target_compile_options(implode PRIVATE ${EXTRA_COMPILE_WARNINGS})

set(EXPLODE_SRC
	${SOURCE_DIR}/explode.cpp
	${SOURCE_DIR}/common.hpp
)
source_group(src FILES ${EXPLODE_SRC})
add_executable(explode ${EXPLODE_SRC})
target_compile_features(explode PRIVATE cxx_std_17)
target_compile_options(explode PRIVATE ${EXTRA_COMPILE_WARNINGS})
