// This interface for the version of darknet forked by AlexeyAB
// github:https://github.com/AlexeyAB/darknet

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
#include "parser.h"
#ifdef __cplusplus
}
#endif

// set time limitation
#include "time.h" 
int start_time = 1532056386;
int trial_month = 3; // Three month for probation

static network net;

// compare to sort detection** by bbox.x
int compare_by_lefts(const void *a_ptr, const void *b_ptr) {
    const detection_with_class* a = (detection_with_class*)a_ptr;
    const detection_with_class* b = (detection_with_class*)b_ptr;
    const float delta = (a->det.bbox.x - a->det.bbox.w/2) - (b->det.bbox.x - b->det.bbox.w/2);
    return delta < 0 ? -1 : delta > 0 ? 1 : 0;
}

int say_hello()
{
    printf("hello, this is a detector!");
    return 0;
}

void detector_init(char *cfgfile, char *weightfile)
{
#ifdef GPU
    cuda_set_device(0);
#endif
    net = parse_network_cfg_custom(cfgfile, 1); // set batch=1

    if(weightfile){
        load_weights(&net, weightfile);
    }
    layer l = net.layers[net.n - 1];

    fuse_conv_batchnorm(net);
    srand(2222222);
    return;
}

void detector_uninit()
{
    free_network(net);
}

double what_is_the_time_now()
{
    return what_time_is_it_now();
}

float* detect(image im, float thresh, float hier_thresh, int* num_output_class)
{
    // set time limitation
    int trial_time = trial_month * 31 * 24 * 60 * 60;
    int trial_remain = start_time + trial_time - (int)time(NULL);
    // printf("current time:%d\n", (int)time(NULL));
    if (trial_remain > 0)
        printf("remaining %d days for your probation\n", trial_remain / (24 * 60 * 60));
    else
    {
        printf("your probation is over!\n");
        return NULL;
    }

    float nms=.45;	// 0.4F
    int letterbox = 0;
    image sized = letterbox_image(im, net.w, net.h); letterbox = 1;
    layer l = net.layers[net.n-1];

    float *X = sized.data;
    network_predict(net, X);
    // printf("Predicted in %f seconds.\n", (what_time_is_it_now()-time));
    int nboxes = 0;
    detection *dets = get_network_boxes(&net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes, letterbox);
    if (nms) do_nms_sort(dets, nboxes, l.classes, nms);

    int selected_detections_num;
    detection_with_class* selected_detections = get_actual_detections(dets, nboxes, thresh, &selected_detections_num);

    // save output
    qsort(selected_detections, selected_detections_num, sizeof(*selected_detections), compare_by_lefts);
    int i;
    float* detections = (float*)calloc(selected_detections_num * 6, sizeof(float));
    for (i = 0; i < selected_detections_num; ++i) {
        const int best_class = selected_detections[i].best_class;
        detections[i*6+0] = best_class;
        detections[i*6+1] = selected_detections[i].det.prob[best_class];
        detections[i*6+2] = (selected_detections[i].det.bbox.x - selected_detections[i].det.bbox.w / 2)*im.w;
        detections[i*6+3] = (selected_detections[i].det.bbox.y - selected_detections[i].det.bbox.h / 2)*im.h;
        detections[i*6+4] = selected_detections[i].det.bbox.w*im.w;
        detections[i*6+5] = selected_detections[i].det.bbox.h*im.h;
    }
    if(num_output_class)
        *num_output_class = selected_detections_num;

    // free memory;
    free_detections(dets, nboxes);
    free_image(im);
    free_image(sized);
    // free_ptrs(names, net.layers[net.n - 1].classes);
    
    return detections;
}

float* test_detector(char *filename, float thresh, float hier_thresh, int* num_output_class)
{
    char buff[256];
    char *input = buff;
    // while(1){
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
    image im = load_image(input,0,0,net.c);
    return detect(im, thresh, hier_thresh, num_output_class);
}

#ifdef OPENCV
void ipl_into_image(IplImage* src, image im)
{
    unsigned char *data = (unsigned char *)src->imageData;
    int h = src->height;
    int w = src->width;
    int c = src->nChannels;
    int step = src->widthStep;
    int i, j, k;

    for(i = 0; i < h; ++i){
        for(k= 0; k < c; ++k){
            for(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
}

image ipl_to_image(IplImage* src)
{
    int h = src->height;
    int w = src->width;
    int c = src->nChannels;
    image out = make_image(w, h, c);
    ipl_into_image(src, out);
    return out;
}

float* test_detector_cv(IplImage* im, float thresh, float hier_thresh, int* num_output_class)
{
    return detect(ipl_to_image(im), thresh, hier_thresh, num_output_class);
}
#endif


