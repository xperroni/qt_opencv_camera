# Qt6 + OpenCV Webcam Example

A minimal example showing how to open a webcam with OpenCV and display frames in a Qt6 GUI.

## Prerequisites

- CMake >= 3.16
- A C++17-capable compiler (g++, clang, MSVC)
- Qt 6 (development packages, e.g. `qt6-base-dev` on Debian/Ubuntu or installed via Homebrew on macOS)
- OpenCV (development packages, `libopencv-dev` on Debian/Ubuntu or `brew install opencv` on macOS)

On Ubuntu (example):
```bash
sudo apt update
sudo apt install build-essential cmake libopencv-dev qt6-base-dev
```

On macOS with Homebrew:
```bash
brew install cmake opencv qt
# You might need to set CMAKE_PREFIX_PATH to Qt6 installation (e.g., /usr/local/opt/qt or /opt/homebrew/opt/qt)
```

## Build

From the project root:
```bash
mkdir build
cd build
# If Qt is not auto-detected, set CMAKE_PREFIX_PATH to your Qt installation path:
# cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6/<version>/lib/cmake
cmake ..
cmake --build . --config Release
```

## Run
From `build`:
```bash
./qt_opencv_camera
```

The app will attempt to open camera index 0. If you want a different device index, you can modify the source where `startCamera()` is called.

## Notes

- The QImage construction uses .copy() to ensure the QImage owns its own memory. Without copying, the QImage would reference cv::Mat memory that may be re-used or freed.
- Adjust m_timer interval for desired framerate (and be mindful of capture performance).
- For production code, consider moving capture to a worker thread and emitting signals to the GUI thread to avoid blocking the UI for heavy processing.
- If you need to show additional UI controls (start/stop, device selection), add QPushButton/QComboBox and wire up signals/slots.
