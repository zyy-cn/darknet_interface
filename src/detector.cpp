#include "detector.h"


#ifdef OPENCV
#ifdef __cplusplus
#include <opencv2/core/core.hpp>
#else
#include <opencv2/core/types_c.h>
#endif
#endif

#ifdef __cplusplus
#ifdef GPU
#include "cuda_runtime.h"
#endif
extern "C" {
#endif
#include "utils.h"
#ifdef __cplusplus
}
#endif


#include <stdio.h>

static network *net;

void detector_init(char *cfgfile, char *weightfile)
{
    net = load_network(cfgfile, weightfile, 0);
    set_batch_network(net, 1);
    srand(2222222);
    return;
}

double what_is_the_time_now()
{
    return what_time_is_it_now();
}
void detector_uninit()
{
    free_network(net);
}


float* select_detections(image im, detection *dets, int num, float thresh , int classes, int* result_num)
{
    int i,j,k=0;
    float* result = (float*)calloc( num*6, sizeof(float));

    for(i = 0; i < num; ++i){

        for(j = 0; j < classes; ++j){
            if (dets[i].prob[j] > thresh){
			
	            box b = dets[i].bbox;
	            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

	            int x1 = (b.x-b.w/2.)*im.w;
	            int y1 = (b.y-b.h/2.)*im.h;
	            int w1 = b.w*im.w;
	            int h1 = b.h*im.h;
	            if(x1 < 0) x1 = 0;
	            if(y1 < 0) y1 = 0;
            
                //printf("%d: %.0f%%\n",j, dets[i].prob[j]*100);		
			    result[k*6+0]=(float)j;result[k*6+1]=dets[i].prob[j]*100;
			    result[k*6+2]=x1;result[k*6+3]=y1;
			    result[k*6+4]=w1;result[k*6+5]=h1;
			    ++k;
            }
        }
	            
    }
    float* slect_result = (float*)calloc( k*6, sizeof(float));
    for(int i=0;i < k;i++){
	    slect_result[i*6+0]=result[i*6+0];slect_result[i*6+1]=result[i*6+1];
	    slect_result[i*6+2]=result[i*6+2];slect_result[i*6+3]=result[i*6+3];
	    slect_result[i*6+4]=result[i*6+4];slect_result[i*6+5]=result[i*6+5];
	    //printf("%f %f %f %f\n", result[i*6+2],result[i*6+3], result[i*6+4], result[i*6+5]);		
    }
    if(result_num) *result_num=k;
    return slect_result;
}

float* detect(image im, float thresh, float hier_thresh, int* result_num)
{
    float nms=.45;
    //image im = load_image_color(filename,0,0);
    image sized = letterbox_image(im, net->w, net->h);

    layer l = net->layers[net->n-1];
    float *X = sized.data;
    network_predict(net, X);
    int nboxes = 0;
    detection *dets = get_network_boxes(net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes);

    if (nms) do_nms_sort(dets, nboxes, l.classes, nms);

    float* detections=select_detections(im, dets, nboxes, thresh, l.classes, result_num);
    free_detections(dets, nboxes);

    free_image(im);
    free_image(sized);
    return detections;
}


float* test_detector(char *filename, float thresh, float hier_thresh, int* num_output_class)
{
    char buff[256];
    char *input = buff;
    if(filename){
        strncpy(input, filename, 256);
        if(strlen(input) > 0)
            if (input[strlen(input) - 1] == 0x0d) input[strlen(input) - 1] = 0;
    } else {
        printf("Enter Image Path: ");
        fflush(stdout);
        input = fgets(input, 256, stdin);
        if(!input) return NULL;
        strtok(input, "\n");
    }
    image im = load_image_color(filename,0,0);
    return detect(im, thresh, hier_thresh, num_output_class);
}





