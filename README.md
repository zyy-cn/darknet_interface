## Darknet Interface

This repository is an INTERFACE for darknet, which allow you to use darknet detector in your own program to do something interesting (like object detect in Raspberry Pi) by link with "libdarknet.so" and "libdetector.so".

## How to use
#### Step 1. Compile your darknet
**Note that I use [This version of Darknet](https://github.com/AlexeyAB/darknet) forked by Alexey**, which I think is faster than the original one.
```
$ git clone https://github.com/AlexeyAB/darknet darknet_Alexey
$ cd darknet_Alexey
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
after do this you can find "libdarknet.so" in darknet repository folder.If the name of this shared library is "darknet.so", you should change it to "libdarknet.so" manually:
```
$ mv darknet.so libdarknet.so
or
$ ln -s darknet.so libdarknet.so
```

#### Step 2. Clone this repository beside the darknet you just cloned
```
$ git clone https://github.com/zyy-cn/darknet_library.git darknet_library
$ cd darknet_library/src
$ chmod 777 *
$ ./gcc.sh
$ cd ..
```
after do this you can get "libdetector.so" in darknet_library/lib. Note that your must compile and install OPENCV firstly and set $OPENCV_INCLUDE_PATH and $OPENCV_LIB_PATH variants in "gcc.sh" currectly if you want to use it.

#### step 3. Run demo
```
$ cd bin
$ ./darknet_detector_test
```

## TODO
- [ ] Add GPU support
