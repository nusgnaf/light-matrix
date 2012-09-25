# The CMakeLists file for Light-Matrix (Testing)

cmake_minimum_required(VERSION 2.8)
project(LightArray)

enable_testing()

include_directories(..)
include_directories($ENV{LIGHT_TEST_HOME})

# Compiler configuration

set(CMAKE_BUILD_TYPE "Release")

if (MSVC)
	set(LANG_FLAGS "/arch:SSE2 /EHsc")
	set(WARNING_FLAGS "/W4")
else (MSVC)
	set(LANG_FLAGS "-std=c++0x -pedantic -march=native ")
	set(WARNING_FLAGS "-Wall -Wextra -Wconversion -Wformat -Wno-unused-parameter ")
endif (MSVC)

if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	set(LANG_FLAGS "${LANG_FLAGS} -stdlib=libc++ -Qunused-arguments")
	set(CMAKE_CXX_COMPILER "clang++")
endif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")

set(TEST_CXX_FLAGS "${LANG_FLAGS} ${WARNING_FLAGS}")

set(CMAKE_CXX_FLAGS "${TEST_CXX_FLAGS}")


# Header file groups (to be used as dependencies)

set(INC ../light_array)
set(EXECUTABLE_OUTPUT_PATH bin)

set(COMMON_BASIC_HS 
    ${INC}/config/user_config.h 
    ${INC}/config/platform_config.h
    ${INC}/config/config.h
    ${INC}/common/prim_types.h
    ${INC}/common/lang_base.h
    ${INC}/common/metaprog_base.h
    ${INC}/common/type_traits.h
    ${INC}/common/arg_check.h
    ${INC}/common/range.h
    ${INC}/common/basic_defs.h)
    
set(COMMON_BLOCK_HS
    ${INC}/common/mem_op.h
    ${INC}/common/mem_alloc.h
    ${INC}/common/block.h)
    
set(COMMON_HS
    ${COMMON_BASIC_HS}
    ${COMMON_ARRAY_HS})
       
    
# Executables

add_library(test_main STATIC test_main.cpp)

add_executable(test_arrays ${CORE_HS} test_blocks.cpp)

# Link to test_main

set(LMAT_TEST_NAMES
	test_arrays
)

	
foreach(tname ${LMAT_TEST_NAMES})
	target_link_libraries(${tname} test_main)
endforeach(tname)	

# Add tests

foreach(tname ${LMAT_TEST_NAMES})
	add_test(NAME ${tname} COMMAND ${tname})
endforeach(tname)


