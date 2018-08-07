## Darknet Interface

[This repository](https://github.com/zyy-cn/darknet_interface) is an INTERFACE for darknet, which allow you to use darknet detector in your own program(C, C++, Python, etc...) to do something interesting (like object detect using YOLO in your PC, Raspberry PI, Nvidia TX1, etc...) by linking with "libdarknet.so" and "libdetector.so".

## Interface Functions Discription
#### void detector_init(char *cfgfile, char *weightfile)
- brief:        initiate an detector into the memory by loading specific network configuration and pretrained weight file.
- param[in1]:   path to the network configuration file
- param[in2]:   path to the network pretrained weight file
- retval:       void

#### float* test_detector(char *filename, float thresh, float hier_thresh, int* num_output_class)
- brief:        detect objects in a specific image file.
- param[in1]:   path to the image file which want to be detected
- param[in2]:   detect thresh
- param[in3]:   set 0.5 as default and don't care in yolov3 (maybe used only for yolo9000?)
- param[in4]:   the number of objects being detected in image
- retval:       information about boundary box according to all objects detected in image. Each object is described as a set of six float data in sequence:
    - class index
    - confidence score
    - top-left x-coordinate of bbox
    - top-left y-coordinate of bbox
    - width of bbox
    - height of bbox

#### float* test_detector_cv(IplImage* im, float thresh, float hier_thresh, int* num_output_class);
- brief:        detect objects in an IplImage encoded by OpenCV.
- param[in1]:   an IplImage which want to be detected
- param[in2~4]: the same as "test_detector" described before
- retval:       the same as "test_detector" described before

#### void detector_uninit()
- brief:        free memory when all detect task finished.
- retval:       void

#### double what_is_the_time_now()
- brief:        grab the current time
- retval:       an double representing the current time(In seconds)

## How to use
#### Step 1. Compile your darknet
**Note that this interface can be used both for the [original darknet](https://github.com/pjreddie/darknet) and [AlexeyAB version](https://github.com/AlexeyAB/darknet)**, you can choose any one you like to compile.  
Btw, I perfer to use AlexeyAB version because I think its faster than the original one, especially in Raspberry PI(OpenBLAS is highly recommended!!).  

If you choose to use the original darknet:
```
$ git clone https://github.com/pjreddie/darknet.git darknet
$ cd darknet
```
Or you can choose AlexeyAB version:
```
$ git clone https://github.com/AlexeyAB/darknet.git darknet_AlexeyAB
$ cd darknet_AlexeyAB
```
modify those variants listed below:
```
$ vim Makefile
    GPU=1           (optional)
    CUDNN=1         (optional)
    CUDNN_HALF=1    (optional, AlexeyAB version only)
    LIBSO=1         (AlexeyAB version only)
```
and then:
```
$ make -j4
$ cd ..
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
```
After do this your directory structure should be looks like this:
```
|-- darknet (or darknet_AlexeyAB)
    |-- cfg
        |-- ...
    |-- src
        |-- ...
    |-- ...
    |-- Makefile
|-- darknet_interface
    |-- src
        |-- ...
    |-- script
        |-- ...
    |-- README.md
```
go on:
```
$ cd darknet_interface/src
$ chmod 777 *
$ vim gcc.sh
```
Modify those variants **listed on the top of gcc.sh** according to your own environment, and then:
```
$ ./gcc.sh
$ cd ..
```
after do this you can get "libdetector.so"(for CPP) and "libdetector_c.so"(for C) in darknet_interface/lib.   
Note that your must compile and install OPENCV(version 3.3.1 or before) firstly if you want to use it, and **macro definition "-DOPENCV" should be added** when those two shared libraries compiled by setting "IS_USE_OPENCV" to "1" in gcc.sh. If you decide to use GPU and CUDNN on step 1, **don't forget to add "-DGPU" and "-DCUDNN" correspondly** by setting "IS_USE_GPU" and "IS_USE_CUDNN" to "1", And "0" if you don't mean to.

#### step 3. Run demo
you can run c++ demo, which allow you to do detect in webcam, video or single image file:
```
$ cd bin
$ export LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
$ ./demo ${detect_type} ${cfg} ${weights} ${thresh} ${image_path|video_path|webcam_index}
```
in which ${detect_type} can be 'image', 'video' or 'webcam'. For instance, you can do detect object using model trained on MSCOCO(can be downloaded in https://pjreddie.com/darknet/yolo/) with thresh 0.5 through camera 0 by running:
```
$ ./demo webcam ../../darknet/cfg/yolov3.cfg ../../darknet/weights/yolov3.weights 0.5 0
```

AI camera is also provided, here is the usage:
```
$ ./AI_camera ${cfg_path} ${weight_path} ${thresh} ${webcam_index} ${target_class_index_list}
```
${target_class_index_list} is an integer vector in which class index of targets you want to detected is stored. Image captured is stored in bin/cap renamed by time. For instance, you can detect person, cat and dog(class_id is 0, 15 and 16) using model trained on MSCOCO with thresh 0.5 through camera 1 by running:
```
$ ./AI_camera ../../darknet/cfg/yolov3.cfg ../../darknet/weights/yolov3.weights 0.5 1 0 15 16
```

if you want to detect multi images contanted in a directory, run python demo:
```
$ cd script
$ export LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
$ python2 detect_dir_img.py ${cfg} ${weights} ${thresh} ${input directory} ${output directory}
```

#### step 4. use this interface in your own programs
- in C/CPP:
    - include "detector.h";
    - link with "libdarknet.so", "libdetector.so"(or "libdetector_c.so") compiled in step 1 and 2;
    - add macro definition ("-DOPENCV", "-DGPU" and "-DCUDNN") and set path to the libraries and headers if you want to use them;
    - **make sure your "libdarknet.so" is compiled with GPU and CUDNN in step 1 before add "-DGPU" and "-DCUDNN" when your program compiled**;
    - view "demo.cpp" and "gcc.sh" for more details.
- in python:
    - view "detect_dir_img.py" for more details

## TODO
- [x] Add GPU support
- [ ] Windows version
- [ ] Add GUI
- [ ] Add demo gif

## License
This project is released under the [WTFPL LICENSE](http://www.wtfpl.net/).
