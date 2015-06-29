
/*  Use SIFT to match two images, and show them!
 * 1. ratio test
 * 2.symmetric test
 * 3. Broute force matching 
 * @vonzhou
*/

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
        cout << "usage:match <level> <image1> <image2>\n" ;
        exit(-1);
    }
  
    string arg2 = string(argv[2]);
    int level = atoi(arg2.c_str());
    string image1_name=string(argv[1]), image2_name = string(argv[2]);
    // getline(cin, image1_name);
    // getline(cin, image2_name);

    Mat image1 = imread(image1_name, 1);
    Mat image2 = imread(image2_name, 1);

    Ptr<FeatureDetector> detector;
    Ptr<DescriptorExtractor> extractor;

    initModule_nonfree();
    /* 
     * SIFT,SURF, ORB
    */
    detector = FeatureDetector::create("SIFT");
    extractor = DescriptorExtractor::create("SIFT");

    clock_t begin = clock();

    vector<KeyPoint> keypoints1, keypoints2;
    detector->detect(image1, keypoints1);
    detector->detect(image2, keypoints2);

    cout << "# keypoints of image1 :" << keypoints1.size() << endl;
    cout << "# keypoints of image2 :" << keypoints2.size() << endl;
   
    Mat descriptors1,descriptors2;
    extractor->compute(image1,keypoints1,descriptors1);
    extractor->compute(image2,keypoints2,descriptors2);

    

    cout << "Descriptors size :" << descriptors1.cols << ":"<< descriptors1.rows << endl;

    vector< vector<DMatch> > matches12, matches21;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
    matcher->knnMatch( descriptors1, descriptors2, matches12, 2 );
    matcher->knnMatch( descriptors2, descriptors1, matches21, 2 );
    
    // BFMatcher bfmatcher(NORM_L2, true);
    // vector<DMatch> matches;
    // bfmatcher.match(descriptors1, descriptors2, matches);
    cout << "Matches1-2:" << matches12.size() << endl;
    cout << "Matches2-1:" << matches21.size() << endl;

    // ratio test proposed by David Lowe paper = 0.8
    std::vector<DMatch> good_matches1, good_matches2;

    for(int i=0; i < matches12.size(); i++){
        const float ratio = 0.8;
        if(matches12[i][0].distance < ratio * matches12[i][1].distance)
            good_matches1.push_back(matches12[i][0]);
    }

    for(int i=0; i < matches21.size(); i++){
        const float ratio = 0.8;
        if(matches21[i][0].distance < ratio * matches21[i][1].distance)
            good_matches2.push_back(matches21[i][0]);
    }

    cout << "Good matches1:" << good_matches1.size() << endl;
    cout << "Good matches2:" << good_matches2.size() << endl;

    // Symmetric Test
    std::vector<DMatch> better_matches;
    for(int i=0; i<good_matches1.size(); i++){
        for(int j=0; j<good_matches2.size(); j++){
            if(good_matches1[i].queryIdx == good_matches2[j].trainIdx && good_matches2[j].queryIdx == good_matches1[i].trainIdx){
                better_matches.push_back(DMatch(good_matches1[i].queryIdx, good_matches1[i].trainIdx, good_matches1[i].distance));
                break;
            }
        }
    }

    cout << "Better matches:" << better_matches.size() << endl;

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Time Costs : " << elapsed_secs << endl;

    // show it on an image
    Mat output;
    drawMatches(image1, keypoints1, image2, keypoints2, better_matches, output);
    imshow("Matches result",output);
    waitKey(0);

    return 0;
}
