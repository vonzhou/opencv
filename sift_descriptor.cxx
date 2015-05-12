#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, const char *argv[]){
    const cv::Mat input = cv::imread("input.jpg", 0);// load as grayscale
    cv::Mat descriptors;
    Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SIFT");

    cv::SiftFeatureDetector detector;
    vector<cv::KeyPoint> keypoints;
    detector.detect(input, keypoints);
    
    extractor->compute(input, keypoints, descriptors);
    cout << descriptors.rows << ":" << descriptors.cols << endl;
    // too many bits
    //cout << descriptors << endl;

    return 0;
}
