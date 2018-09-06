## Darknet Interface
[This repository](https://github.com/zyy-cn/darknet_interface) is an INTERFACE for darknet, which allow you to use darknet detector in your own program(C, C++, Python, etc...) to do something interesting (like object detect using YOLO in your PC, Raspberry PI, Nvidia TX1, etc...) by linking with "libdarknet.so" and "libdetector.so".

## Demo GIF
[Click here to view demo](https://github.com/zyy-cn/demo_image/tree/master/darknet_interface)

## Interface Functions Discription
#### void detector_init(char *cfgfile, char *weightfile)
- brief:        initiate an detector into the memory by loading specific network configuration and pretrained weight file.
- param[in1]:   path to the network configuration file
- param[in2]:   path to the network pretrained weight file
- retval:       void

#### float* test_detector_file(char *filename, float thresh, float hier_thresh, int* num_output_class)
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

#### float* test_detector_uchar(unsigned char* data, int w, int h, int c, float thresh, float hier_thresh, int* num_output_class);
- brief:        detect objects in an image data buffer
- param[in1]:   a pointer to the image data buffer. In such buffer an image was arranged as bgrbgr...bgr by rows (the same as cv::Mat::data or IplImage.imageData) with format unsigned char.
- param[in2]:   width of image
- param[in3]:   height of image
- param[in4]:   channels of image
- param[in5~7]: the same as "test_detector_file" described before
- retval:       the same as "test_detector_file" described before

#### void detector_uninit()
- brief:        free memory when all detect task finished.
- retval:       void

#### double what_is_the_time_now()
- brief:        grab the current time
- retval:       an double representing the current time(In seconds)

## How to use
### Step 1. Compile your darknet
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

### Step 2. Clone and compile this repository beside the darknet repository you just cloned
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
Note that your must compile and install OPENCV(version 3.3.1 or before) firstly if you want to use it, and **macro definition "-DOPENCV" should be added** by setting "IS_USE_OPENCV" to "1" in gcc.sh. If you decide to use GPU and CUDNN on step 1, **don't forget to add "-DGPU" and "-DCUDNN" correspondly** by setting "IS_USE_GPU" and "IS_USE_CUDNN" to "1", And "0" if you don't mean to.

### step 3. Run demo
you can run c++ demo, which allow you to do detect in webcam, video or single image file:
```
$ cd bin
$ export LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
$ ./demo ${detect_type} ${cfg} ${weights} ${thresh} ${image_path|video_path|webcam_index}
```
in which ${detect_type} can be 'image', 'video' or 'webcam'. For instance, you can do detect object using model trained on MSCOCO(can be downloaded [here](https://pjreddie.com/darknet/yolo/)) with thresh 0.5 through camera 0 by running:
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

you can also run python demo which is similar to its c++ counterpart:
```
$ cd script
$ export LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
$ python demo.py ${detect_type} ${cfg} ${weights} ${thresh} ${video_path|webcam_index}
```
in which ${detect_type} can be 'video' or 'webcam'. If you want to detect multi images contanted in a directory:
```
$ python detect_dir_img.py ${cfg} ${weights} ${thresh} ${input directory} ${output directory}
```

### step 4. use this interface in your own programs
- in C/CPP:
    - include "detector.h";
    - link with "libdarknet.so", "libdetector.so"(or "libdetector_c.so") compiled in step 1 and 2;
    - add macro definition ("-DOPENCV", "-DGPU" and "-DCUDNN") and set path to the libraries and headers if you want to use them;
    - **make sure your "libdarknet.so" is compiled with GPU and CUDNN in step 1 before add "-DGPU" and "-DCUDNN" when your program compiled**;
    - view "demo.cpp" and "gcc.sh" for more details.
- in python:
    - view "demo.py" and "detect_dir_img.py" for more details

## Darknet 接口
[本项目](https://github.com/zyy-cn/darknet_interface) 是一个用于darknet的接口, 可以让你在你自己的程序中（C，C++，Python等）通过链接“libdarknet.so”和“libdetector.so”这两个库，就可以使用darknet检测器来做一些很酷的事情（比如在你的个人电脑，树莓派，英伟达TX1等设备上运行YOLO目标检测算法）。

## 演示动画
[点击这里观看演示](https://github.com/zyy-cn/demo_image/tree/master/darknet_interface)

## 接口函数描述
#### void detector_init(char *cfgfile, char *weightfile)
- 简介:        通过载入指定模型配置文件和预训练权重文件，将一个检测器变量在内存中初始化。
- 参数[in1]:   网络配置文件路径
- 参数[in2]:   网络预训练权重文件路径
- 返回值:      无

#### float* test_detector_file(char *filename, float thresh, float hier_thresh, int* num_output_class)
- 简介:        在指定图像文件中进行目标检测
- 参数[输入1]:   你想要进行检测的图像文件的路径
- 参数[输入2]:   检测阈值
- 参数[输入3]:   默认设置为0.5就行，在yolo第3版中用不到（好像只在yolo9000中有用？）
- 参数[输入4]:   在图像中检测到的物体的总数量
- 返回值:       所有在图像中检测到的物体的包围盒信息。每个物体用一组由六个浮点数组成的集合来依次描述：
    - 类别号
    - 置信度分数
    - 包围盒左上角x坐标
    - 包围盒左上角y坐标
    - 包围盒宽度
    - 包围盒高度

#### float* test_detector_uchar(unsigned char* data, int w, int h, int c, float thresh, float hier_thresh, int* num_output_class);
- 简介:        在一个图像数据缓存里进行目标检测
- 参数[输入1]:   指向图像数据缓存的指针。在这样一个缓存里，图像数据以bgrbgr...bgr的顺序按行排列 (跟Opencv中的cv::Mat::data或IplImage.imageData的排列顺序相同)，数据类型为unsigned char。
- 参数[输入2]:   图像宽度
- 参数[输入3]:   图像高度
- 参数[输入4]:   图像通道数
- 参数[输入5~7]: 跟之前在“test_detector_file”中描述的相同
- 返回值:       跟之前在“test_detector_file”中描述的相同

#### void detector_uninit()
- 简介:        在所有检测任务完成后释放内存。
- 返回值:       无

#### double what_is_the_time_now()
- 简介:        抓取当前时间
- 返回值:       一个代表当前时间的双精度浮点型数据（精确到秒）

## 用法
### 步骤 1. 编译你的darknet
 **注意这个接口在[原版darknet](https://github.com/pjreddie/darknet)和[AlexeyAB版本的darknet](https://github.com/AlexeyAB/darknet)上都管用**，你可以任意选择你喜欢的版本来编译.  
顺便说一句，我比较喜欢用AlexeyAB的那个版本，因为我觉得它比原版快一点，特别是在树莓派上（强烈推荐把OpenBLAS一起编进去！！！）。

如果你选择使用原版的darknet：
```
$ git clone https://github.com/pjreddie/darknet.git darknet
$ cd darknet
```
或者你可以用AlexeyAB的版本：
```
$ git clone https://github.com/AlexeyAB/darknet.git darknet_AlexeyAB
$ cd darknet_AlexeyAB
```
在Makefile中修改这下面列出的这些变量：
```
$ vim Makefile
    GPU=1           (可选)
    CUDNN=1         (可选)
    CUDNN_HALF=1    (可选, 只有AlexeyAB版本才需要修改)
    LIBSO=1         (只有AlexeyAB版本才需要修改)
```
然后:
```
$ make -j4
$ cd ..
```
做完这些以后你会在darknet目录文件夹下找到“libdarknet.so”这个库文件。要是这个共享库文件的名字是“darknet.so”，你得手动把它改成“libdarknet.so”：
```
$ mv darknet.so libdarknet.so
或者
$ ln -s darknet.so libdarknet.so
```

### 步骤 2. 在你刚刚克隆下来的darknet项目文件夹旁边，克隆这个接口项目
```
$ git clone https://github.com/zyy-cn/darknet_interface.git darknet_interface
```
干完这步以后，你的目录结构应该长这样：
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
继续：
```
$ cd darknet_interface/src
$ chmod 777 *
$ vim gcc.sh
```
根据你自己的系统环境，修改**列在gcc.sh最上面的**那些变量，然后：
```
$ ./gcc.sh
$ cd ..
```
完成以后，你可以在darknet_interface/lib目录下找到“libdetector.so”（用于C++）和“libdetector_c.so”（用于C）这两个库文件。  
注意你如果想用opencv的话，你得先编译并安装好它（3.3.1或更低的版本），并且通过在“gcc.sh”中设置“IS_USE_OPENCV”为1来添加宏定义“-DOPENCV”。如果在步骤1里，你设置了使用GPU和CUDNN，那别忘了把“IS_USE_GPU”和“IS_USE_CUDNN”相应地设置成“1”来添加宏定义“-DGPU”和“-DCUDNN”，要是你不想用它们，就设置成“0”。

### 步骤 3. 运行示例
你可以运行C++示例, 这个示例可以让你在摄像头画面，视频文件或图像文件中进行目标检测：
```
$ cd bin
$ export LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
$ ./demo ${detect_type} ${cfg} ${weights} ${thresh} ${image_path|video_path|webcam_index}
```
其中${detect_type}可以是'image','video'或'webcam'。举个例子，你可以用在MSCOCO上预训练好的模型(从[这里](https://pjreddie.com/darknet/yolo/)可以下载到)以阈值0.5在摄像头0的画面里进行目标检测，命令如下：
```
$ ./demo webcam ../../darknet/cfg/yolov3.cfg ../../darknet/weights/yolov3.weights 0.5 0
```

AI相机也提供了，用法为:
```
$ ./AI_camera ${cfg_path} ${weight_path} ${thresh} ${webcam_index} ${target_class_index_list}
```
其中${target_class_index_list}是一个整型数组，其中存的是你想要检测的类的类别号。拍摄下来的图像以拍摄时间为文件名，依次存储在bin/cap文件夹中。比如，你可以用在MSCOCO上预训练好的模型，在摄像头1的画面中，以阈值0.5对人、猫和狗这三个类别（类别号分别是0，15，16）进行抓拍，命令如下：
```
$ ./AI_camera ../../darknet/cfg/yolov3.cfg ../../darknet/weights/yolov3.weights 0.5 1 0 15 16
```

你也可以运行python示例，跟上面的C++版本的示例用法很相似：
```
$ cd script
$ export LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
$ python demo.py ${detect_type} ${cfg} ${weights} ${thresh} ${video_path|webcam_index}
```
其中${detect_type}可以是'video'或'webcam'。如果你想要批量检测存储在一个目录之下的多张图片：
```
$ python detect_dir_img.py ${cfg} ${weights} ${thresh} ${input directory} ${output directory}
```

### 步骤 4. 在你自己的程序中使用darknet接口
- 在C/C++中:
    - 包含头文件"detector.h";
    - 链接在步骤1和2中编译出来的库文件“libdarknet.so”和“libdetector.so”(或“libdetector_c.so”);
    - 如果你想要用OpenCV、GPU或CUDNN的话，就添加宏定义(“-DOPENCV”，“-DGPU”和“-DCUDNN”)并添加相应的库文件和头文件的路径，。
    - **你在编译自己的程序时，添加宏定义“-DGPU”和“-DCUDNN”之前，得先确保在步骤1中编译出的“libdarknet.so”本身是可以使用GPU和CNDUU的**;
    - 更多细节请查看“demo.cpp”和“gcc.sh”。
- 在python中:
    - 更多细节请查看“demo.py”和“detect_dir_img.py”。

## TODO
- [x] Add GPU support
- [x] python interface for video and webcam detection
- [x] Add demo gif
- [ ] Windows version
- [ ] Add GUI

## License
This project is released under the [WTFPL LICENSE](http://www.wtfpl.net/).
