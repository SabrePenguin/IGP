qt_add_library(
	app
	STATIC
	MainWindow.cpp
        mainwindow.ui
        MainWindow.h
)

target_link_libraries(
	app
	PRIVATE
	Qt6::Core
	Qt6::Widgets
)

target_include_directories(
	app
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../logic
)
