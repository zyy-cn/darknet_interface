// This header is used for both detector.cpp(the version of original darknet)
// and detector_Alexey.cpp(the version of Alexey) 

#ifdef OPENCV
#ifdef __cplusplus
#include <opencv2/core/core.hpp>
#else
#include <opencv2/core/types_c.h>
#endif
float* test_detector_cv(IplImage* im, float thresh, float hier_thresh, int* num_output_class);
#endif





void detector_init(char *cfgfile, char *weightfile);

float* test_detector(char *filename, float thresh, float hier_thresh, int* num_output_class);

void detector_uninit();

double what_is_the_time_now();
