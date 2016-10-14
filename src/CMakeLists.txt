# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QT_MIN_VERSION "5.7")
set(EXECUTABLE_NAME "de_casteljau_demo")

# Find the QtWidgets library
find_package(Qt5Core ${QT_MIN_VERSION} REQUIRED)
find_package(Qt5Widgets ${QT_MIN_VERSION} REQUIRED)

file(GLOB SRCS 
*.cxx *.cpp)
# Tell CMake to create the helloworld executable
add_executable(${EXECUTABLE_NAME} main.cpp ${SRCS})

# Use the Widgets module from Qt 5.
target_link_libraries(${EXECUTABLE_NAME} Qt5::Core Qt5::Widgets)
