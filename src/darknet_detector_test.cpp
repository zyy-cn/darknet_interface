#include "detector.h"

#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 

#ifdef OPENCV
#include "opencv2/core/types_c.h"
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp> 
#include <thread>
int FRAME_WINDOW_WIDTH = 320;
int FRAME_WINDOW_HEIGHT = 240;
using namespace std;
using namespace cv;  

void detect_in_thread(Mat frame_detect, float* detections_output, int* num_output_class, double* time_consumed, float thresh, float hier_thresh)
{
        IplImage input = IplImage(frame_detect);
        float* detections = test_detector_cv(&input, thresh, hier_thresh, num_output_class, time_consumed);
        printf("Predicted in %f seconds.\n", *time_consumed);
        for(int i = 0; i < *num_output_class; i++)
        {
            printf("%.0f: %.0f%%", detections[i*6+0],	detections[i*6+1] * 100);
            printf("\t(left_x: %4.0f   top_y: %4.0f   width: %4.0f   height: %4.0f)\n",
            detections[i*6+2], detections[i*6+3], detections[i*6+4], detections[i*6+5]);
            detections_output[i*6+0] = detections[i*6+0];
            detections_output[i*6+1] = detections[i*6+1];
            detections_output[i*6+2] = detections[i*6+2];
            detections_output[i*6+3] = detections[i*6+3];
            detections_output[i*6+4] = detections[i*6+4];
            detections_output[i*6+5] = detections[i*6+5];
        }
}
#endif

int main()
{
    int model_select = 1; // model 0 for original yolo model and 1 for tiny model 
    char  *cfgfile;
    char *weightfile;
    double detect_interval;
    float thresh;
    float hier_thresh;
    int num_output_class = 0;
    double time_consumed = -1;
    float *detections;
    if(model_select == 0)
    {
        cfgfile = "/home/m/Code/darknet_Alexey/cfg/yolov3.cfg";
        weightfile = "/home/m/Code/darknet_Alexey/weights/yolov3.weights";
        thresh = 0.5;
        hier_thresh = 0.9;
        detect_interval = 30;
    }
    else if(model_select == 1)
    {
        cfgfile = "/home/m/Code/darknet_Alexey/cfg/yolov3-tiny.cfg";
        weightfile = "/home/m/Code/darknet_Alexey/weights/yolov3-tiny.weights";
        detect_interval = 5;
        thresh = 0.2;
        hier_thresh = 0.5;
        detect_interval = 5;
    }

    detector_init(cfgfile, weightfile);

#ifndef OPENCV
    char *filename1 = "/home/m/Code/darknet_Alexey/data/person.jpg";
    detections = test_detector(filename1, thresh, hier_thresh, &num_output_class, &time_consumed);
    printf("Predicted in %f seconds.\n", time_consumed);
    detections = test_detector(filename1, thresh, hier_thresh, &num_output_class, &time_consumed);
    printf("Predicted in %f seconds.\n", time_consumed);
    detections = test_detector(filename1, thresh, hier_thresh, &num_output_class, &time_consumed);
    printf("Predicted in %f seconds.\n", time_consumed);
    detections = test_detector(filename1, thresh, hier_thresh, &num_output_class, &time_consumed);
    printf("Predicted in %f seconds.\n", time_consumed);
    detections = test_detector(filename1, thresh, hier_thresh, &num_output_class, &time_consumed);
    printf("Predicted in %f seconds.\n", time_consumed);
    detections = test_detector(filename1, thresh, hier_thresh, &num_output_class, &time_consumed);
    printf("Predicted in %f seconds.\n", time_consumed);
    detections = test_detector(filename1, thresh, hier_thresh, &num_output_class, &time_consumed);
    printf("Predicted in %f seconds.\n", time_consumed);

    printf("num_output_class:%d\n", num_output_class);
    for(int i = 0; i < num_output_class; i++)
    {
        printf("%.0f: %.0f%%", detections[i*6+0],	detections[i*6+1] * 100);
        printf("\t(left_x: %4.0f   top_y: %4.0f   width: %4.0f   height: %4.0f)\n",
            detections[i*6+2], detections[i*6+3], detections[i*6+4], detections[i*6+5]);
    }
#else

    bool is_show_frame = true;
    bool is_show_detections = true;
    bool is_detect_in_thread = true;
    bool is_print_bboxes_info = false;

    VideoCapture cap(0);
    bool isOpen = true;  
    if(!cap.isOpened())  
        isOpen = false;
    if(!isOpen)
    {
    printf("No camera found \n");
    return -1;
    }
    Mat frame; 
    detections = (float*)calloc(255*6, sizeof(float));
    time_t t0, t1;
    t0 = -1;
    Rect detections_rect;
    bool stop = false;  
    while(!stop)  
    {
        if(isOpen)
        {
            cap >> frame;
            // ====== detect objects ======
            if(!is_detect_in_thread)
                detect_in_thread(frame, detections, &num_output_class, &time_consumed, thresh, hier_thresh);
            else
            {
                t1 = time(NULL);
                if((t1 - t0 ) > detect_interval || !(t0 > 0))
                {
                    printf("begin detect, detect_interval is:%f \n", detect_interval);
                    t0 = time(NULL);
                    thread t(detect_in_thread, frame, detections, &num_output_class, &time_consumed, thresh, hier_thresh);
                    t.detach();
                    if(((time_consumed - detect_interval) > 0.2 || (time_consumed - detect_interval) < -0.3) 
                    && time_consumed >0)
                    {
                        detect_interval = time_consumed;
                        printf("detect_interval modified to: %f \n", detect_interval);
                        printf("time_consumed: %f \n", (float)time_consumed);
                    }
                }
            }
            
            // ====== show detections ======
            if(is_show_frame)
            {
                if(is_show_detections)
                {
                    if(is_print_bboxes_info)
                        printf("num_output_class:%d\n", num_output_class);
                    for(int i = 0; i < num_output_class; i++)
                    {
                        if(is_print_bboxes_info)
                        {
                            printf("%.0f: %.0f%%", detections[i*6+0],	detections[i*6+1] * 100);
                            printf("\t(left_x: %4.0f   top_y: %4.0f   width: %4.0f   height: %4.0f)\n",
                            detections[i*6+2], detections[i*6+3], detections[i*6+4], detections[i*6+5]);
                        }
                        detections_rect.x = detections[i*6+2];
                        detections_rect.y = detections[i*6+3];
                        detections_rect.width = detections[i*6+4];
                        detections_rect.height = detections[i*6+5];
                        if(detections[i*6+0] == 0) // person
                            rectangle(frame, detections_rect, CV_RGB(255, 0, 0), 4, 8, 0);
                        else if(detections[i*6+0] == 63) // tvmonitor
                            rectangle(frame, detections_rect, CV_RGB(0, 0, 255), 4, 8, 0);
                        else if(detections[i*6+0] == 28)// chair
                            rectangle(frame, detections_rect, CV_RGB(0, 255, 0), 4, 8, 0);
                        else if(detections[i*6+0] == 56)
                            rectangle(frame, detections_rect, CV_RGB(255, 255, 0), 4, 8, 0);
                        else
                            rectangle(frame, detections_rect, CV_RGB(0, 0, 0), 4, 8, 0);
                    }
                }

                namedWindow("camera 0", cv::WINDOW_AUTOSIZE);
                imshow("camera 0", frame);
                if(waitKey(30) >=0)  
                    stop = true;  
            }
        }
    }  
#endif

    detector_uninit();

    return 0;
}

