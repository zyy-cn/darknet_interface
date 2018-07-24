# ====== set your variants here firstly ======

IS_USE_DARKNET_ALEXEYAB=0 # '0' for the original darknet and '1' for the AlexeyAB version

IS_USE_GPU=1
IS_USE_CUDNN=1
CUDA_PATH=/usr/local/cuda # cudnn's header and libs should be found in $CUDA_PATH/include and $CUDA_PATH/lib64 correspondly

IS_USE_OPENCV=1
OPENCV_INCLUDE_PATH=/home/m/local_install/include
OPENCV_LIB_PATH=/home/m/local_install/lib
#OPENCV_INCLUDE_PATH=/usr/local/include
#OPENCV_LIB_PATH=/usr/local/lib




# ====== compile libraries and demo program automaticly and DO NOT make any change if not necessary ======
DETECTOR_BIN_PATH=../bin
DETECTOR_LIB_PATH=../lib
if [ $IS_USE_DARKNET_ALEXEYAB == 1 ];then
    DARKNET_SUFFIX=_AlexeyAB
fi
DARKNET_SRC_PATH=../../darknet$DARKNET_SUFFIX/src
DARKNET_LIB_PATH=../../darknet$DARKNET_SUFFIX
if [ $IS_USE_DARKNET_ALEXEYAB == 1 ];then
    DARKNET_INCLUDE_PATH=$DARKNET_SRC_PATH
else
    DARKNET_INCLUDE_PATH=../../darknet$DARKNET_SUFFIX/include
fi
DARKNET_SRC_PATH=../../darknet$DARKNET_SUFFIX/src
DARKNET_LIB_PATH=../../darknet$DARKNET_SUFFIX
if [ $IS_USE_OPENCV == 1 ];then
    CFLAGS+=\ -DOPENCV
    OPENCV+=-I$OPENCV_INCLUDE_PATH\ -L$OPENCV_LIB_PATH\ -lopencv_core\ -lopencv_imgproc\ -lopencv_highgui\ -lopencv_imgcodecs\ -lopencv_videoio
fi
if [ $IS_USE_GPU == 1 ];then
    CFLAGS+=\ -DGPU
    if [ $IS_USE_CUDNN == 1 ];then
        CFLAGS+=\ -DCUDNN
    fi
else
    CFLAGS+=\ -DOPENBLAS
fi
rm $DETECTOR_LIB_PATH/libdetector.so
rm $DETECTOR_LIB_PATH/libdetector_c.so
rm $DETECTOR_LIB_PATH/libnet.so
cp $DARKNET_LIB_PATH/libdarknet.so $DETECTOR_LIB_PATH/libnet.so

ln -s detector$DARKNET_SUFFIX.cpp detector.c

# compile shared libraries
g++ -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector.so detector$DARKNET_SUFFIX.cpp -I. -I$DARKNET_INCLUDE_PATH -I$DARKNET_SRC_PATH -I$CUDA_PATH/include -L$DETECTOR_LIB_PATH -L$CUDA_PATH/lib64 -lnet -fopenmp -lgomp $CFLAGS $OPENCV
gcc -fPIC -shared -O3 -o $DETECTOR_LIB_PATH/libdetector_c.so detector.c -I. -I$DARKNET_INCLUDE_PATH -I$DARKNET_SRC_PATH -I$CUDA_PATH/include -L$DETECTOR_LIB_PATH -L$CUDA_PATH/lib64 -lnet -fopenmp -lgomp $CFLAGS $OPENCV

# compile demo
g++ -std=c++11 -O3  -o $DETECTOR_BIN_PATH/demo demo.cpp -I. -L$DETECTOR_LIB_PATH -L$DETECTOR_LIB_PATH -ldetector -fopenmp -pthread -lgomp -lnet $OPENCV $CFLAGS

rm detector.c
