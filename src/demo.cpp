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

void detect_mat(Mat frame_detect, float* detections_output, int* num_output_class, double* time_consumed, float thresh, float hier_thresh)
{
    double time = what_is_the_time_now();
    IplImage input = IplImage(frame_detect);// convert image format from mat to iplimage

    // do detect in an iplimage converted from mat
    float* detections = test_detector_cv(&input, thresh, hier_thresh, num_output_class);
    for(int i = 0; i < *num_output_class; i++)
    {
        printf("%.0f: %.0f%%", detections[i*6+0],	detections[i*6+1] * 100);
        printf("\t(left_x: %4.0f   top_y: %4.0f   width: %4.0f   height: %4.0f)\n",
        detections[i*6+2], detections[i*6+3], detections[i*6+4], detections[i*6+5]);
        detections_output[i*6+0] = detections[i*6+0];// ith detection's category
        detections_output[i*6+1] = detections[i*6+1];// ith detection's confidence score
        detections_output[i*6+2] = detections[i*6+2];// ith detection's top-left x-coordinate of bbox
        detections_output[i*6+3] = detections[i*6+3];// ith detection's top-left y-coordinate of bbox
        detections_output[i*6+4] = detections[i*6+4];// ith detection's width of bbox
        detections_output[i*6+5] = detections[i*6+5];// ith detection's height of bbox
    }
    if(time_consumed)
        *time_consumed = (what_is_the_time_now() - time);
}
#endif

int main()
{
    int model_select = 0; // model 0 for the original yolov3 model and 1 for its tiny counterpart 
    char  *cfgfile;
    char *weightfile;
    double time_consumed = 0;
    float thresh;
    float hier_thresh;
    int num_output_class = 0;
    float *detections;
    if(model_select == 0)
    {
        cfgfile = "../../darknet_Alexey/cfg/yolov3.cfg";
        weightfile = "../../darknet_Alexey/weights/yolov3.weights";
        thresh = 0.5;
        hier_thresh = 0.5;
    }
    else if(model_select == 1)
    {
        cfgfile = "../../darknet_Alexey/cfg/yolov3-tiny.cfg";
        weightfile = "../../darknet_Alexey/weights/yolov3-tiny.weights";
        thresh = 0.2;
        hier_thresh = 0.5;
    }

    detector_init(cfgfile, weightfile);

#ifndef OPENCV
    //char *filename1 = "../../darknet_Alexey/data/person.jpg";
    char *filename1;
    double time;
    for(int i = 0; i < 10; i++)
    {
        time = what_is_the_time_now();
        // do detect in an image file
        detections = test_detector(filename1, thresh, hier_thresh, &num_output_class);
        printf("Predicted in %f seconds.\n", what_is_the_time_now() - time);
        printf("num_output_class:%d\n", num_output_class);
        for(int i = 0; i < num_output_class; i++)
        {
            printf("%.0f: %.0f%%", detections[i*6+0],	detections[i*6+1] * 100);
            printf("\t(left_x: %4.0f   top_y: %4.0f   width: %4.0f   height: %4.0f)\n",
                detections[i*6+2], detections[i*6+3], detections[i*6+4], detections[i*6+5]);
        }
    }

#else
    bool is_show_frame = true;
    bool is_show_detections = true;
    bool is_detect_in_thread = true;
    // ====== init camera ======
    VideoCapture cap(1);// set your camera index
    bool isCameraOpened = true;  
    if(!cap.isOpened())  
        isCameraOpened = false;
    if(!isCameraOpened)
    {
        printf("No camera found \n");
        return -1;
    }
    Mat frame; 
    detections = (float*)calloc(255*6, sizeof(float));
    Rect detections_rect;
    bool stop = false;  
    while(!stop)  
    {
        if(isCameraOpened)
        {
            cap >> frame;
            // ====== detect objects ======
            if(!is_detect_in_thread)
                detect_mat(frame, detections, &num_output_class, &time_consumed, thresh, hier_thresh);
            else
            {
                // do detect in a background thread
                if(time_consumed >= 0)
                {
                    thread t(detect_mat, frame, detections, &num_output_class, &time_consumed, thresh, hier_thresh);
                    t.detach();
                    printf("time_consumed: %f \n", (float)time_consumed);
                    time_consumed = -1;
                }
            }
            
            // ====== show detections ======
            if(is_show_frame)
            {
                if(is_show_detections)
                {
                    for(int i = 0; i < num_output_class; i++)
                    {
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

                namedWindow("detections", cv::WINDOW_AUTOSIZE);
                imshow("detections", frame);
                if(waitKey(30) >=0)  
                    stop = true;  
            }
        }
    }  
#endif

    detector_uninit();

    return 0;
}

