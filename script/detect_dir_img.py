import os
import sys
import cv2
from ctypes import *



def main(argv):
    if len(argv) < 5:
        print('USAGE:')
        print('  $ python2 detect_dir_img.py ${cfg} ${weights} ${input directory} ${output directory}:')
    else:
        # ====== init ======
        detector = cdll.LoadLibrary('../lib/libdetector_c.so')
        detector.test_detector.restype = POINTER(c_float)
        detector.what_is_the_time_now.restype = c_double
        detector.say_hello()
        # cfgfile = "/home/m/Code/darknet_Alexey/cfg/yolov3.cfg"
        # weightfile = "/home/m/Code/darknet_Alexey/weights/yolov3.weights"
        cfgfile = argv[1]
        weightfile = argv[2]
        detector.detector_init(cfgfile, weightfile)

        # ====== detect ======
        cv2.namedWindow("show", 0)
        input_dir_name = argv[3]
        output_dir_name = argv[4]
        for filename in os.listdir(input_dir_name):
            print('==== detect begin ====   ')
            # ====== load images from directory ======
            img_path = input_dir_name + '/' + filename
            img = cv2.imread(img_path)
            # ====== do detect ======
            num_output_class = pointer(c_int(0))
            time = detector.what_is_the_time_now()
            detections = detector.test_detector(img_path, c_float(0.5), c_float(0.9), num_output_class)
            # ====== show detections ======
            for i in range(0, num_output_class[0]):
                category = int(detections[i * 6 + 0])
                confidence = float(detections[i * 6 + 1])
                x_lt = int(detections[i * 6 + 2])
                y_lt = int(detections[i * 6 + 3])
                width = int(detections[i * 6 + 4])
                height = int(detections[i * 6 + 5])
                if detections[i * 6 + 0] == 0:
                    print('--' + str(i) + 'th detection--')
                    print('  category:  ' + str(category))
                    print('  confidence:' + str(confidence * 100) + '%')
                    print('  x_lt:' + str(x_lt) + '  y_lt:' + str(y_lt) + '  width:' + str(width) + '  height:' + str(height))
                    cv2.rectangle(img, (int(x_lt), int(y_lt)), (int(x_lt) + int(width), int(y_lt ) + int(height)), (255, 0, 0), 2)
            print('detected image:  ' + filename)
            print('num_output_class:' + str(num_output_class[0]))
            print('time_comsumed:   ' + str(detector.what_is_the_time_now() - time) + 's')
            print(' ')
            cv2.imshow("show", img)
            cv2.waitKey(1)
            # ====== save result image ======
            output_img = output_dir_name + '/' + filename
            cv2.imwrite(output_img, img)



if __name__ == '__main__':
    main(sys.argv)
