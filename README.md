## Darknet Interface

This reposity is an INTERFACE for darknet, which allow you to use darknet detector in your own CPP program to do something interesting (like object detect) by link with "libdarknet.so" and "libdetector.so".

## How to use
#### Step 1. Compile your darknet first
**Note that I use [This version of Darknet](https://github.com/AlexeyAB/darknet) forked by Alexey**, which I think is faster than the original one.
```
$ git clone https://github.com/AlexeyAB/darknet
$ cd darknet
$ vim Makefile
```
modify the value of "LIBSO" from "0" to "1":
```
LIBSO=1
```
and then:
```
$ make -j4
```
after do this you can find "libdarknet.so" in darknet reposity folder.If the name of this shared library is "darknet.so", you should change it to "libdarknet.so" Manually:
```
$ mv darknet.so libdarknet.so
or
$ ln -s darknet.so libdarknet.so
```

#### Step 2. Clone this reposity beside the darknet you just cloned
```
$ git clone https://github.com/zyy-cn/darknet_library.git
$ cd darknet_library/lib_detector
$ g++ -fPIC -shared -o libdetector.so detector.cpp -I. -I../../darknet/src -I/PATH/TO/YOUR/OPENCV/include -L../../darknet/ -L/PATH/TO/YOUR/OPENCV/lib -ldarknet -fopenmp -lgomp -DOPENCV
$ cd ..
```
after do this you can get "libdetector.so".Note that your must compile OPENCV firstly if you want to use it.

#### step 3. Run demo
```
$ cd examples
$ g++ -std=c++11 -o darknet_detector_test darknet_detector_test.cpp -I../lib_detector -I/PATH/TO/YOUR/OPENCV/include -L../lib_detector -L/PATH/TO/YOUR/OPENCV/lib -L../../darknet_AlexeyAB -ldetector -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -fopenmp -pthread -lgomp -ldarknet -DOPENCV
$ ln -s ../lib_detector/libdetector.so libdetector.so
$ ln -s ../../darknet/libdarknet.so libdarknet.so
$ ./darknet_detector_test
```
