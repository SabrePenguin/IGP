# CMakeList.txt : CMake project for WarBinder, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(IGP)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(
	HEADER_FILES
	ColorCountDialog.h
	MainWindow.h
	Pattern.h
	Renderer.h
	ResizeDialog.h
)

set(
	SOURCES
	main.cpp
	ColorCountDialog.cpp
	MainWindow.cpp
	Pattern.cpp
	Renderer.cpp
	ResizeDialog.cpp
)
qt_standard_project_setup()

# Add source to this project's executable.
qt_add_executable(IGP ${SOURCES} ${HEADER_FILES})

target_link_libraries(
	IGP
	PRIVATE
	Qt6::Core
	Qt6::Widgets
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET IGP PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
