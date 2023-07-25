cmake_minimum_required(VERSION 3.10)
project(proto_context)

if(WIN32)
  add_compile_options(/fsanitize=address)
  add_compile_options(/Zi)
else()
  find_package(Threads REQUIRED)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utility.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utility.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/TaskRunner.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/TaskRunner.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/ProtoContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ProtoContext.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/ProtoSharedContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ProtoSharedContext.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/ProtoClient.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ProtoClient.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/ProtoDataCache.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ProtoDataCache.hpp
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)

if(MSVC)
	string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif(MSVC)
