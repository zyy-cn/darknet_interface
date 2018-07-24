# this program only works under opencv2 because IplImage is unsupported by opencv3
# this program is a work-in-progress now.
import os
import sys
import cv2
import cv2.cv as cv
from ctypes import *



def main(argv):
    if len(argv) < 5:
        print('USAGE:')
        print('  $ python2 detect_dir_img.py ${cfg} ${weights} ${thresh} ${input_video}:')
    else:
        # ====== init ======
        detector = cdll.LoadLibrary('../lib/libdetector_c.so')
        detector.test_detector_cv.restype = POINTER(c_float)
        detector.what_is_the_time_now.restype = c_double
        cfgfile = argv[1]
        weightfile = argv[2]
        thresh = argv[3]
        input_video = argv[4]
        detector.detector_init(cfgfile, weightfile)
        cap = cv2.VideoCapture(input_video)

        # ====== detect ======
        cv2.namedWindow("show", 0)
        while(cap.isOpened()):
            print('==== detect begin ====   ')
            # ====== load frames from video ======
            ret, frame = cap.read()
            if ret == True:
                # ====== do detect ======
                num_output_class = pointer(c_int(0))
                time = detector.what_is_the_time_now()
                ipl = cv.CreateImageHeader((frame.shape[1], frame.shape[0]), cv.IPL_DEPTH_8U, 3)
                cv.SetData(ipl, frame.tostring(), frame.dtype.itemsize * 3 * frame.shape[1])
                detections = detector.test_detector_cv(ipl, c_float(thresh), c_float(0.9), num_output_class)
                # ====== show detections ======
                for i in range(0, num_output_class[0]):
                    category = int(detections[i * 6 + 0])
                    confidence = float(detections[i * 6 + 1])
                    x_lt = int(detections[i * 6 + 2])
                    y_lt = int(detections[i * 6 + 3])
                    width = int(detections[i * 6 + 4])
                    height = int(detections[i * 6 + 5])
                    if detections[i * 6 + 0] != -1:
                        print('--' + str(i) + 'th detection--')
                        print('  category:  ' + str(category))
                        print('  confidence:' + str(confidence * 100) + '%')
                        print('  x_lt:' + str(x_lt) + '  y_lt:' + str(y_lt) + '  width:' + str(width) + '  height:' + str(height))
                        cv2.rectangle(img, (int(x_lt), int(y_lt)), (int(x_lt) + int(width), int(y_lt ) + int(height)), (255, 0, 0), 2)
                        cv2.putText(img,'cls:'+str(category)+', conf:'+str(round(confidence*100,2))+'%',(x_lt,y_lt-10),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,0),2,8)
                print('detected image:  ' + filename)
                print('num_output_class:' + str(num_output_class[0]))
                print('frame_rate:   ' + str(1/(detector.what_is_the_time_now()-time)) + ' frame/s')
                print(' ')
                cv2.imshow("show", img)
                cv2.waitKey(1)
                # ====== save result image ======
                output_img = output_dir_name + '/' + filename
                cv2.imwrite(output_img, img)
            else:
                break
            cap.release()
            cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
