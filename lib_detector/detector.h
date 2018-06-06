#ifdef OPENCV
#ifdef __cplusplus
#include <opencv2/core/core.hpp>
#else
#include <opencv2/core/types_c.h>
#endif
float* test_detector_cv(IplImage* im, float thresh, float hier_thresh, int* num_output_class, double* time_comsumed);
#endif


void detector_init(char *cfgfile, char *weightfile);

float* test_detector(char *filename, float thresh, float hier_thresh, int* num_output_class, double* time_comsumed);

void detector_uninit();

