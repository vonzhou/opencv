#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //

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
    

    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);
    cout << "# matches : " << matches.size() << endl;

    //calc max and min keypoints distance
    double maxdist=0, mindist=100;
    for(int i=0;i<descriptors1.rows;i++){
        double dist = matches[i].distance;
        //cout << "test" << dist << endl;
        if(dist<mindist)
            mindist = dist;
        if(dist > maxdist)
            maxdist = dist;
    }
    cout << "Max distance:" << maxdist << "Min distance:" << mindist << endl;
    // Get good matches (? distance is lesss than 2*min)
    vector<DMatch> goods;
    for(int i=0;i<descriptors1.rows;i++){
        if(matches[i].distance < 2*mindist)
            goods.push_back(matches[i]);
    }
    cout << "# good matches : " << goods.size() << endl;

    // show it on an image
    Mat output;
    drawMatches(car1, keypoints1, car2, keypoints2, 
            goods, output, Scalar::all(-1), Scalar::all(-1),
            vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    imshow("good matches result",output);
    waitKey(0);

    return 0;
}
