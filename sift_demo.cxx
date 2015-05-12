#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //
using namespace cv;


int main(int argc, const char *argv[]){
    Mat input = imread("input.jpg", 0);// load as grayscale
    
    SiftFeatureDetector detector;
    vector<KeyPoint> keypoints;
    detector.detect(input, keypoints);
    
    // show the keypoints on an image
    Mat output;
    drawKeypoints(input, keypoints, output);
    imwrite("sift_result.jpg",output);

    return 0;
}
