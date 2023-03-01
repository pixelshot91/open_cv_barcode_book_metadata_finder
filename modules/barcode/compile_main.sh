#!/bin/bash

clang++ -Ofast -iquote ../include -I /usr/local/include/opencv4/ -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio -lopencv_dnn book_metadata_finder/main.cpp src/*.cpp src/decoder/*.cpp src/decoder/common/*.cpp  src/detector/*.cpp -o book_barcode_detect.release.out
#  clang++ -g -iquote ../include -I /usr/local/include/opencv4/ -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio -lopencv_dnn book_metadata_finder/main.cpp src/*.cpp src/decoder/*.cpp src/decoder/common/*.cpp  src/detector/*.cpp -o book_barcode_detect.debug.out
