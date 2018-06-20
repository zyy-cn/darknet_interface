# set your opencv install 
OPENCV_INCLUDE_PATH=/home/pi/local_install/include
OPENCV_LIB_PATH=/home/pi/local_install/lib

DETECTOR_BIN_PATH=../bin
DETECTOR_LIB_PATH=../lib
DARKNET_SRC_PATH=../../darknet_Alexey/src
DARKNET_LIB_PATH=../../darknet_Alexey

g++ -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector.so detector.cpp -I. -I$DARKNET_SRC_PATH -I$OPENCV_INCLUDE_PATH -L$DARKNET_LIB_PATH -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS

rm detector.c
ln -s detector.cpp detector.c

gcc -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector_c.so detector.c -I. -I$DARKNET_SRC_PATH -I$OPENCV_INCLUDE_PATH -L$DARKNET_LIB_PATH -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS

g++ -std=c++11 -O3  -o $DETECTOR_BIN_PATH/darknet_detector_test darknet_detector_test.cpp -I. -I$OPENCV_INCLUDE_PATH -L$OPENCV_LIB_PATH -L$DARKNET_LIB_PATH -L$DETECTOR_LIB_PATH -ldetector -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -fopenmp -pthread -lgomp -ldarknet -DOPENCV

rm $DETECTOR_BIN_PATH/libdarknet.so
rm $DETECTOR_BIN_PATH/libdetector.so
ln -s $DARKNET_LIB_PATH/libdarknet.so $DETECTOR_BIN_PATH/libdarknet.so
ln -s $DETECTOR_LIB_PATH/libdetector.so $DETECTOR_BIN_PATH/libdetector.so
