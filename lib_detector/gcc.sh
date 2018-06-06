g++ -fPIC -shared -O3 -o libdetector.so detector.cpp -I. -I../../darknet_Alexey/src -I/home/pi/local_install/include -L/home/pi/Code/darknet_Alexey/ -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS
