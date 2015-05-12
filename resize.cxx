#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //
#include <opencv2/imgproc/imgproc.hpp> // resize()

#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, const char *argv[]){
    Mat car1 = imread("car1.jpeg", 0);// load as grayscale
    Mat car2 = imread("car2.jpeg", 0);
    //cv::resize(car1,car1,car2.size());

    SiftFeatureDetector detector;
    vector<KeyPoint> keypoints1, keypoints2;
    detector.detect(car1, keypoints1);
    detector.detect(car2, keypoints2);
    cout << "# keypoints of car1 :" << keypoints1.size() << endl;
    cout << "# keypoints of car2 :" << keypoints2.size() << endl;
   
    Mat descriptors1,descriptors2;
    Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SIFT");
    extractor->compute(car1,keypoints1,descriptors1);
    extractor->compute(car2,keypoints2,descriptors2);
    

    //Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");// BruteForce L2
    BFMatcher bfmatcher(NORM_L2, true);
    vector<DMatch> matches;
    bfmatcher.match(descriptors1, descriptors2, matches);
    cout << "# matches : " << matches.size() << endl;


    // show it on an image
    Mat output;
    drawMatches(car1, keypoints1, car2, keypoints2, matches, output);
    imshow("car matches result",output);
    waitKey(0);

    return 0;
}
