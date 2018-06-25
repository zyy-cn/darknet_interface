# set your opencv install 
OPENCV_INCLUDE_PATH=/home/m/local_install/include
OPENCV_LIB_PATH=/home/m/local_install/lib
IS_USE_GPU=0

DETECTOR_BIN_PATH=../bin
DETECTOR_LIB_PATH=../lib
DARKNET_SRC_PATH=../../darknet_Alexey/src
DARKNET_LIB_PATH=../../darknet_Alexey
CUDA_PATH=/usr/local/cuda/


if [ $IS_USE_GPU == 0 ]
    then
        g++ -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector.so detector.cpp -I. -I$DARKNET_SRC_PATH -I$OPENCV_INCLUDE_PATH -I/usr/local/cuda/include -L$DARKNET_LIB_PATH -L/usr/local/cuda/lib64 -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS -DGPU
        gcc -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector_c.so detector.c -I. -I$DARKNET_SRC_PATH -I$OPENCV_INCLUDE_PATH -I$CUDA_PATH/include -L$DARKNET_LIB_PATH -L$CUDA_PATH/lib64 -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS -DGPU
    else
        g++ -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector.so detector.cpp -I. -I$DARKNET_SRC_PATH -I$OPENCV_INCLUDE_PATH -L$DARKNET_LIB_PATH -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS
        gcc -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector_c.so detector.c -I. -I$DARKNET_SRC_PATH -I$OPENCV_INCLUDE_PATH -L$DARKNET_LIB_PATH -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS
fi

rm detector.c
ln -s detector.cpp detector.c

g++ -std=c++11 -O3  -o $DETECTOR_BIN_PATH/darknet_detector_test darknet_detector_test.cpp -I. -I$OPENCV_INCLUDE_PATH -L$OPENCV_LIB_PATH -L$DARKNET_LIB_PATH -L$DETECTOR_LIB_PATH -ldetector -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -fopenmp -pthread -lgomp -ldarknet -DOPENCV

rm $DETECTOR_BIN_PATH/libdarknet.so
rm $DETECTOR_BIN_PATH/libdetector.so
ln -s $DARKNET_LIB_PATH/libdarknet.so $DETECTOR_BIN_PATH/libdarknet.so
ln -s $DETECTOR_LIB_PATH/libdetector.so $DETECTOR_BIN_PATH/libdetector.so
