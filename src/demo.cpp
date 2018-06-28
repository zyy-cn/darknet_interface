#include "detector.h"

#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 


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
        hier_thresh = 0.9;
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
    char *filename1 = "../../darknet_Alexey/data/person.jpg";
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

#endif

    detector_uninit();
    return 0;
}

