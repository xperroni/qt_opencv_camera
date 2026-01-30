cmake_minimum_required(VERSION 3.16)
project(QtOpenCVCam LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic moc/uic/rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(OpenCV REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(qt_opencv_camera
    src/main.cpp
    src/CameraWidget.cpp
)

target_include_directories(qt_opencv_camera PRIVATE src)
target_link_libraries(qt_opencv_camera PRIVATE Qt6::Widgets ${OpenCV_LIBS})

# On multi-config generators (Visual Studio), optionally set RUNTIME output dir for built executable:
if(MSVC)
    set_target_properties(qt_opencv_camera PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>"
    )
endif()