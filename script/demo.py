import os
import sys
import cv2
import numpy as np
from ctypes import *

def main(argv):
    if len(argv) < 6:
        print('USAGE:')
        print(' detect video :')
        print('     $ python demo.py video ${cfg} ${weights} ${thresh} ${video_path}')
        print(' detect webcam :')
        print('     $ python demo.py webcam ${cfg} ${weights} ${thresh} ${camera_index}')
    else:
        # ====== init ======
        detector = cdll.LoadLibrary('../lib/libdetector_c.so')
        detector.test_detector_uchar.restype = POINTER(c_float)
        detector.what_is_the_time_now.restype = c_double
        cfgfile = argv[2]
        weightfile = argv[3]
        thresh = argv[4]
        detector.detector_init(cfgfile.encode('ascii'), weightfile.encode('ascii'))
        if(argv[1] == 'video'):
            cap = cv2.VideoCapture(argv[5])
        elif(argv[1] == 'webcam'):
            cap = cv2.VideoCapture(int(argv[5]))

        # ====== detect ======
        cv2.namedWindow("show", 0)
        while True:
            ret, frame_bgr = cap.read()
            frame = cv2.cvtColor(frame_bgr, cv2.COLOR_BGR2RGB)
            print('==== detect begin ====   ')
            # ====== load frames from video ======
            if ret == True:
                time = detector.what_is_the_time_now()
                # ====== convert input parameters into ctypes ======
                input_data = frame.flatten()
                p_input_data = input_data.ctypes.data_as(POINTER(c_ubyte))
                h = c_int(frame.shape[0])
                w = c_int(frame.shape[1])
                c = c_int(frame.shape[2])
                num_output_class = pointer(c_int(0))
                # ====== do detect using darknet interface ======
                detections = detector.test_detector_uchar(p_input_data, w, h, c, c_float(float(thresh)), c_float(0.9), num_output_class)
                # ====== show detections ======
                for i in range(0, num_output_class[0]):
                    category = int(detections[i * 6 + 0])
                    confidence = float(detections[i * 6 + 1])
                    x_lt = int(detections[i * 6 + 2])
                    y_lt = int(detections[i * 6 + 3])
                    width = int(detections[i * 6 + 4])
                    height = int(detections[i * 6 + 5])
                    print(str(category)+':'+str(int(round(confidence*100)))+'%'+'  (left_x:' + str(x_lt)+'  top_y:'+str(y_lt)+'  width:'+str(width)+'  height:'+str(height)+')')
                    if detections[i * 6 + 0] == 0: # person, red
                        cv2.rectangle(frame_bgr, (int(x_lt), int(y_lt)), (int(x_lt) + int(width), int(y_lt ) + int(height)), (0, 0, 255), 2)
                    elif detections[i * 6 + 0] == 62 or detections[i * 6 + 0] == 63 : # tvmonitor && laptop, blue
                        cv2.rectangle(frame_bgr, (int(x_lt), int(y_lt)), (int(x_lt) + int(width), int(y_lt ) + int(height)), (255, 0, 0), 2)
                    elif detections[i * 6 + 0] == 28 or detections[i * 6 + 0] == 56 : # suitcase && chair, green
                        cv2.rectangle(frame_bgr, (int(x_lt), int(y_lt)), (int(x_lt) + int(width), int(y_lt ) + int(height)), (0, 255, 0), 2)
                    else:
                        cv2.rectangle(frame_bgr, (int(x_lt), int(y_lt)), (int(x_lt) + int(width), int(y_lt ) + int(height)), (0, 0, 0), 2)
                    cv2.putText(frame_bgr,str(category)+':'+str(int(round(confidence*100)))+'%',(x_lt,y_lt-10),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,0),2,8)
                print('frame_rate:   ' + str(1/(detector.what_is_the_time_now()-time)) + ' frame/s')
                print(' ')
                cv2.imshow("show", frame_bgr)
                cv2.waitKey(1)
            else:
                break
        cap.release()
        cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
