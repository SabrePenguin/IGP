# Add source to this project's executable.
set(CMAKE_AUTOUIC ON)

qt_add_executable(IGP "main.cpp")

add_subdirectory(app)

add_subdirectory(logic)

target_link_libraries(
	IGP
	PRIVATE
	app
	logic
	Qt6::Core
	Qt6::Widgets
)
