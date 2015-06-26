
#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;

int main( int argc, char** argv ){

Ptr<FeatureDetector> l_Detector = FeatureDetector::create("ORB"); 
Ptr<DescriptorExtractor> l_Descriptor = DescriptorExtractor::create("ORB"); 
Ptr<DescriptorMatcher> l_Matcher = DescriptorMatcher::create("BruteForce-Hamming");

// Extracting keypoints... 
std::vector<cv::KeyPoint> keypoints1; 
std::vector<cv::KeyPoint> keypoints2;

l_Detector->detect(img1, keypoints1); 
l_Detector->detect(img2, keypoints2);

// Computing descriptors... 
Mat descriptors1; 
Mat descriptors1;

l_Descriptor->compute( img1, keypoints1, descriptors1 ); 
l_Descriptor->compute( img2, keypoints2, descriptors2 );

double ransacReprojThreshold = 5; 
int mactherFilterType = 1;

//crossCheckMatching 
vector<DMatch> filteredMatches; 
filteredMatches12.clear(); 
vector<vector<DMatch> > matches12, matches21; 
descriptorMatcher->knnMatch( descriptors1, descriptors2, matches12, knn ); 
descriptorMatcher->knnMatch( descriptors2, descriptors1, matches21, knn ); 

for( size_t m = 0; m < matches12.size(); m++ ) 
{ 
bool findCrossCheck = false; 
for( size_t fk = 0; fk < matches12[m].size(); fk++ ) 
{ 
DMatch forward = matches12[m][fk];

for( size_t bk = 0; bk < matches21[forward.trainIdx].size(); bk++ ) 
{ 
DMatch backward = matches21[forward.trainIdx][bk]; 
if( backward.trainIdx == forward.queryIdx ) 
{ 
filteredMatches12.push_back(forward); 
findCrossCheck = true; 
break; 
} 
} 
if( findCrossCheck ) break; 
} 
}

// filter outliers	
H12 = findHomography( Mat(points1), Mat(points2), CV_RANSAC, ransacReprojThreshold );

double maxInlierDist = ransacReprojThreshold < 0 ? 3 : ransacReprojThreshold; 
for( size_t i1 = 0; i1 < points1.size(); i1++ ) 
{ 
if( norm(points2[i1] - points1t.at<Point2f>((int)i1,0)) <= maxInlierDist ) // inlier 
matchesMask[i1] = 1; 
}

// Convert keypoints into Point2f and filter all the outliers points 
std::vector<cv::Point2f> finalpoints1, finalpoints2; 
int idx = 0; 
for (std::vector<cv::DMatch>:: 
const_iterator it= filteredMatches.begin(); 
it!= filteredMatches.end(); ++it) { 
if ( matchesMask[idx] == 1) 
{ 
// Get the position of left keypoints 
float x= keypoints1[it->queryIdx].pt.x; 
float y= keypoints1[it->queryIdx].pt.y; 
finalpoints1.push_back(cv::Point2f(x,y));

// Get the position of right keypoints 
x= keypoints2[it->trainIdx].pt.x; 
y= keypoints2[it->trainIdx].pt.y; 
finalpoints2.push_back(cv::Point2f(x,y)); 
} 
idx++; 
}

Mat T = estimateRigidTransform(Mat(finalpoints2),Mat(finalpoints1),false);

Mat output; 
warpAffine( img2, output, T, img1.size(),INTER_CUBIC,BORDER_TRANSPARENT ); 
imwrite("output/output.bmp",output);


}