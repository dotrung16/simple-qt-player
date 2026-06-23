Simple Music Player 

A lightweight audio player built with C++ and Qt framework.

To build and run this, you need the following things:
- Qt6 framework (Core, Gui, Widgets and Multimedia modules)
- CMake (3.19+)

Linux (Ubuntu/Debian) requirements: On Linux, Qt Multimedia relies on GStreamer for its audio backend. You will need to install the development files and the GStreamer plugins: 
    sudo apt install build-essential cmake ninja-build \ qt6-base-dev qt6-multimedia-dev libqt6multimedia6 \ libgstreamer1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good \ gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav