cmake_minimum_required( VERSION 3.7 )

# Use ccache if posible...
find_program( CCACHE_PROGRAM ccache )
if( CCACHE_PROGRAM )
	message( STATUS "Using ccache." )
	set( CMAKE_C_COMPILER_LAUNCHER ${CCACHE} )
	set( CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE} )
endif()

set( PROJECT_NAME "HighLoadFun_TestSuite" )

project( ${PROJECT_NAME} CXX )

# Check platform...
if( NOT CMAKE_SYSTEM_NAME STREQUAL "Linux" )
	message( STATUS " Platform: ${CMAKE_SYSTEM_NAME}" )
	message( FATAL_ERROR " Only Linux platform is supported." )
endif()

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_executable( ${PROJECT_NAME}
	3rd/fmt/color.h
	3rd/fmt/core.h
	3rd/fmt/format-inl.h
	3rd/fmt/format.h

	3rd/bytell_hash_map.hpp
	3rd/flat_hash_map.hpp
	3rd/xxhash.h

	ts_main.cpp
	ts_pch.hpp
	ts_memory_view.hpp
	ts_scoped_fd_subst.hpp
	ts_shm.hpp
	ts_task.hpp
	ts_timer.hpp
	ts_task_01.hpp
	ts_task_02.hpp
	ts_task_05.hpp
	ts_task_08.hpp
	ts_task_09.hpp
	ts_task_10.hpp
	ts_task_11.hpp
	ts_task_13.hpp
	ts_task_17.hpp
	ts_task_18.hpp
	ts_task_19.hpp )

target_precompile_headers( ${PROJECT_NAME} PUBLIC
	"$<$<COMPILE_LANGUAGE:CXX>:<ts_pch.hpp$<ANGLE-R>>" )

set( GNU_AND_CLANG_COMMON_COMP_OPTS -Wall -march=native -fno-rtti ) # -fno-exceptions
set( GNU_AND_CLANG_COMMON_LINK_OPTS -mconsole -s )

target_compile_options( ${PROJECT_NAME} PRIVATE ${GNU_AND_CLANG_COMMON_COMP_OPTS} )
target_link_options   ( ${PROJECT_NAME} PRIVATE ${GNU_AND_CLANG_COMMON_LINK_OPTS} )

# Get compilation options...
get_target_property( COMPILE_OPTS ${PROJECT_NAME} COMPILE_OPTIONS )

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	set( COMPILE_OPTS "${CMAKE_CXX_FLAGS_DEBUG} ${COMPILE_OPTS}" )
else()
	set( COMPILE_OPTS "${CMAKE_CXX_FLAGS_RELEASE} ${COMPILE_OPTS}" )
endif()

string( REPLACE ";" " " COMPILE_OPTS_STR "${COMPILE_OPTS}" )

# Output some vars...
message( STATUS "----------------------------------------------------------------" )
message( STATUS " Compiler ID: ${CMAKE_CXX_COMPILER_ID}" )
message( STATUS "  Build type: ${CMAKE_BUILD_TYPE}" )
message( STATUS "  Dbg. flags: ${CMAKE_CXX_FLAGS_DEBUG}" )
message( STATUS "  Rel. flags: ${CMAKE_CXX_FLAGS_RELEASE}" )
message( STATUS "----------------------------------------------------------------" )
message( STATUS " Curr. flags: ${COMPILE_OPTS_STR}" )
message( STATUS "----------------------------------------------------------------" )

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR} )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rd )

set_target_properties     ( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )
set_target_properties     ( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "ts_main" )
