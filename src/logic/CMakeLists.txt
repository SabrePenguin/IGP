qt_add_library(
	logic
	STATIC
	ColorCountDialog.cpp
	Pattern.cpp
	Renderer.cpp
	ResizeDialog.cpp
)

target_link_libraries(
	logic
	PRIVATE
	Qt6::Core
	Qt6::Widgets
)

target_include_directories(
	logic
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)