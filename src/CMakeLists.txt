set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

option(BUILD_WITH_QT5 "Build with Qt5 instead of Qt6" OFF)

if(${BUILD_WITH_QT5})
    set(QT_MIN_VERSION "5.7")
    set(QT_MAJOR_VERSION "5")
else()
    set(QT_MIN_VERSION "6.5")
    set(QT_MAJOR_VERSION "6")
endif()

set(EXECUTABLE_NAME "de_casteljau_demo")

find_package("Qt${QT_MAJOR_VERSION}Core" ${QT_MIN_VERSION} REQUIRED)
find_package("Qt${QT_MAJOR_VERSION}Widgets" ${QT_MIN_VERSION} REQUIRED)

file(GLOB SRCS *.cpp *.h)

add_executable(${EXECUTABLE_NAME} main.cpp ${SRCS})

target_link_libraries(${EXECUTABLE_NAME} "Qt${QT_MAJOR_VERSION}::Core" "Qt${QT_MAJOR_VERSION}::Widgets")
