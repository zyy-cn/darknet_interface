g++ -fPIC -shared -O3 -o libdetector.so detector.cpp -I. -I../../darknet_AlexeyAB/src -I/home/pi/local_install/include -L/home/pi/Code/darknet_AlexeyAB/ -ldarknet -fopenmp -lgomp -DOPENCV -DOPENBLAS
