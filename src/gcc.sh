# set your opencv install directory
IS_USE_GPU=1
IS_USE_CUDNN=1
OPENCV_INCLUDE_PATH=/home/m/local_install/include
OPENCV_LIB_PATH=/home/m/local_install/lib
#OPENCV_INCLUDE_PATH=/usr/local/include
#OPENCV_LIB_PATH=/usr/local/lib

DETECTOR_BIN_PATH=../bin
DETECTOR_LIB_PATH=../lib
DARKNET_ALEXEY_SRC_PATH=../../darknet_Alexey/src
DARKNET_ALEXEY_LIB_PATH=../../darknet_Alexey
CUDA_PATH=/usr/local/cuda

ln -s detector_Alexey.cpp detector_Alexey.c

if [ $IS_USE_GPU == 1 ];then
    if [ $IS_USE_CUDNN == 1 ];then
        g++ -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector.so detector.cpp -I. -I$DARKNET_ALEXEY_SRC_PATH -I$OPENCV_INCLUDE_PATH -I$CUDA_PATH/include -L$DARKNET_ALEXEY_LIB_PATH -L$CUDA_PATH/lib64 -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS -DGPU -DCUDNN
        gcc -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector_c.so detector.c -I. -I$DARKNET_ALEXEY_SRC_PATH -I$OPENCV_INCLUDE_PATH -I$CUDA_PATH/include -L$DARKNET_ALEXEY_LIB_PATH -L$CUDA_PATH/lib64 -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS -DGPU -DCUDNN
    else
        g++ -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector.so detector.cpp -I. -I$DARKNET_ALEXEY_SRC_PATH -I$OPENCV_INCLUDE_PATH -I$CUDA_PATH/include -L$DARKNET_ALEXEY_LIB_PATH -L$CUDA_PATH/lib64 -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS -DGPU
        gcc -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector_c.so detector.c -I. -I$DARKNET_ALEXEY_SRC_PATH -I$OPENCV_INCLUDE_PATH -I$CUDA_PATH/include -L$DARKNET_ALEXEY_LIB_PATH -L$CUDA_PATH/lib64 -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS -DGPU
    fi
else
        g++ -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector.so detector.cpp -I. -I$DARKNET_ALEXEY_SRC_PATH -I$OPENCV_INCLUDE_PATH -L$DARKNET_ALEXEY_LIB_PATH -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS
        gcc -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector_c.so detector.c -I. -I$DARKNET_ALEXEY_SRC_PATH -I$OPENCV_INCLUDE_PATH -L$DARKNET_ALEXEY_LIB_PATH -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS
fi

rm detector.c
ln -s detector.cpp detector.c

g++ -std=c++11 -O3  -o $DETECTOR_BIN_PATH/demo demo.cpp -I. -I$OPENCV_INCLUDE_PATH -L$OPENCV_LIB_PATH -L$DARKNET_ALEXEY_LIB_PATH -L$DETECTOR_LIB_PATH -ldetector -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -fopenmp -pthread -lgomp -ldarknet -DOPENCV

rm $DETECTOR_BIN_PATH/libdarknet.so
rm $DETECTOR_BIN_PATH/libdetector.so
ln -s $DARKNET_ALEXEY_LIB_PATH/libdarknet.so $DETECTOR_BIN_PATH/libdarknet.so
ln -s $DETECTOR_LIB_PATH/libdetector.so $DETECTOR_BIN_PATH/libdetector.so
