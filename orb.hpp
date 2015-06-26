#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //

#include <iostream>
#include <algorithm> // sort()
#include <dirent.h>
#include <ctime>
using namespace cv;
using namespace std;


std::vector<string> searchImages_ORB(string input_image, string img_dir, int k);



