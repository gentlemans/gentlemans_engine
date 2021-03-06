cmake_minimum_required(VERSION 3.0)

project(gentlemanly_engine)

set(PROJECT_VERSION 0.1.0)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

option(GE_BUILD_SAMPLES "Should the samples be built?" ON)


# update the module path so cmake can find libraries that don't have built in CMake support
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)


set(GE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include(cmake/gentlemanly_engine.cmake)

find_package(Doxygen)

if(DOXYGEN_FOUND)

	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/doxygen/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)

	add_custom_target(doc
		COMMAND ${DOXYGEN_EXECUTABLE}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMENT "Generating Doxygen documentation..."
		DEPENDS ${GE_PRIVATE_FILES} ${GE_PUBLIC_FILES}
	)

else()
	message("If you want the target \"doc\" to be avaliable to build doxygen documentation,
	then install Doxygen or if it is already installed, set DOXYGEN_EXECUTABLE with a path to doxygen.")
endif()

add_subdirectory(deps)

add_subdirectory(ge)
add_subdirectory(modules)

# Samples

if(GE_BUILD_SAMPLES)
	add_subdirectory(samples)
endif()
