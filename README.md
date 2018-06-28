## Darknet Interface

This repository is an INTERFACE for darknet, which allow you to use darknet detector in your own program(C, C++, Python, etc...) to do something interesting (like object detect use YOLO in Raspberry PI) by linking with "libdarknet.so" and "libdetector.so".

## How to use
#### Step 1. Compile your darknet
**Note that this interface can be used both for the [original darknet](https://github.com/pjreddie/darknet) and [Alexey version](https://github.com/AlexeyAB/darknet)**, you can choose any one you like to compile.  
Btw, I perfer to use Alexey version because I think its faster than the original one, especially in Raspberry PI.  

If you choose to use the original darknet:
```
$ git clone https://github.com/pjreddie/darknet.git darknet
$ cd darknet
```
Or you can choose Alexey version:
```
$ git clone https://github.com/AlexeyAB/darknet.git **darknet_Alexey**
$ cd **darknet_Alexey**
```
modify those variants listed below:
```
$ vim Makefile
    GPU=1           (optional)
    CUDNN=1         (optional)
    CUDNN_HALF=1    (optional, Alexey version only)
    LIBSO=1         (Alexey version only)
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

#### Step 2. Clone and compile this repository beside the darknet repository you just cloned
```
$ git clone https://github.com/zyy-cn/darknet_interface.git darknet_interface
$ cd darknet_interface/src
$ chmod 777 *
$ vim gcc.sh
```
**Modify those variants listed on the top of gcc.sh according to your own environment**, and then:
```
$ ./gcc.sh
$ cd ..
```
after do this you can get "libdetector.so" and "libdetector_c.so" in darknet_interface/lib.   
Note that your must compile and install OPENCV firstly if you want to use it. Furthermore, If you decide to use GPU and CUDNN on step 1, "IS_USE_GPU" and "IS_USE_CUDNN" must set to "1" correspondingly, and "0" if you don't.

#### step 3. Run demo
you can run c++ demo by:
```
$ cd bin
$ ./demo
```
or you can run python demo by:
```
$ cd script
$ python2 detect_dir_img.py ${cfg} ${weights} ${input directory} ${output directory}
```

## TODO
- [x] Add GPU support
- [ ] Add GUI
