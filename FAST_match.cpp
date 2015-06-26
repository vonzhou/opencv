#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //
#include <opencv2/nonfree/nonfree.hpp>

#include <iostream>
#include <dirent.h>
#include <ctime>
using namespace cv;
using namespace std;

int main(int argc, const char *argv[]){

	if(argc != 3){
        cout << "usage:match <image1> <image2>\n" ;
        exit(-1);
    }

    string image1_name=string(argv[1]), image2_name = string(argv[2]);
    // getline(cin, image1_name);
    // getline(cin, image2_name);

    Mat leftImageGray = imread(image1_name, 0);
    Mat rightImageGray = imread(image2_name, 0);

    vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1,descriptors_2;

	Ptr<FeatureDetector> detector;
	detector = new DynamicAdaptedFeatureDetector ( new FastAdjuster(10,true), 5000, 10000, 10);
	detector->detect(leftImageGray, keypoints_1);
	detector->detect(rightImageGray, keypoints_2);
	cout << "# keypoints of image1 :" << keypoints_1.size() << endl;
    cout << "# keypoints of image2 :" << keypoints_2.size() << endl;

    initModule_nonfree();//NB. need this, otherwise get coredump ,oops !!!!!
	Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SIFT");
	extractor->compute( leftImageGray, keypoints_1, descriptors_1 );
	extractor->compute( rightImageGray, keypoints_2, descriptors_2 );

	vector< vector<DMatch> > matches;
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
	matcher->knnMatch( descriptors_1, descriptors_2, matches, 500 );

	//look whether the match is inside a defined area of the image
	//only 25% of maximum of possible distance
	double tresholdDist = 0.25 * sqrt(double(leftImageGray.size().height*leftImageGray.size().height + leftImageGray.size().width*leftImageGray.size().width));

	vector< DMatch > good_matches2;
	good_matches2.reserve(matches.size());  
	for (size_t i = 0; i < matches.size(); ++i){ 
    	for (int j = 0; j < matches[i].size(); j++)    {
        	Point2f from = keypoints_1[matches[i][j].queryIdx].pt;
        	Point2f to = keypoints_2[matches[i][j].trainIdx].pt;

        	//calculate local distance for each possible match
        	double dist = sqrt((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));

        	//save as best match if local distance is in specified area and on same height
        	if (dist < tresholdDist && abs(from.y-to.y)<5)  {
        	    good_matches2.push_back(matches[i][j]);
            	j = matches[i].size();
        	}
    	}
	}

	cout << "Good matches :" << good_matches2.size() << endl;

	// show it on an image
    Mat output;
    drawMatches(leftImageGray, keypoints_1, rightImageGray, keypoints_2, good_matches2, output);
    imshow("Matches result",output);
    waitKey(0);

}