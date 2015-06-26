/**
 * ORB pure: detect -> extract -> brouteforce match -> filter distance !
 * result: 
 * @vonzhou
 */

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace cv;
using namespace std;

void readme();
int main( int argc, char** argv )
{
  if( argc != 3 )
  { readme(); return -1; }

  Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
  Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );

  if( !img_1.data || !img_2.data )
  { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

  //-- Step 1: Detect the keypoints using ORB Detector

  ORB detector(1000,1.1f,8,31,0,2,ORB::HARRIS_SCORE,31);

  std::vector<KeyPoint> keypoints_1, keypoints_2;

  detector.detect( img_1, keypoints_1 );
  detector.detect( img_2, keypoints_2 );

  //-- Step 2: Calculate descriptors 
  OrbDescriptorExtractor extractor;

  Mat descriptors_1, descriptors_2;

  extractor.compute( img_1, keypoints_1, descriptors_1 );
  extractor.compute( img_2, keypoints_2, descriptors_2 );

  //-- Step 3: Matching descriptor vectors using FLANN matcher or BruteForce
  //FlannBasedMatcher matcher;
  // BFMatcher matcher(NORM_L2, true);
  Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");


  

  vector< vector<DMatch> > matches, matches2;
  matcher->knnMatch( descriptors_1, descriptors_2, matches, 500 );
  matcher->knnMatch( descriptors_2, descriptors_1, matches2, 500);
  cout << "==" << matches.size() << ", "<< matches[0].size() << endl;
 
  //-- Draw matches
  // Mat img_matches, img_matches2;
  // drawMatches( img_1, keypoints_1, img_2, keypoints_2,matches, img_matches, Scalar::all(-1), Scalar::all(-1),
  //              vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
  // drawMatches( img_1, keypoints_1, img_2, keypoints_2,matches2, img_matches2, Scalar::all(-1), Scalar::all(-1),
  //              vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
  // cout << "Matches1-2:" << matches.size() << endl;
  // cout << "Matches2-1:" << matches2.size() <<endl;

  // imshow("Matches result 1-2",img_matches);
  // imshow("Matches result 2-1", img_matches2);
  waitKey(0);

  return 0;
}

void readme()
{ std::cout << " Usage: ./SURF_FlannMatcher <img1> <img2>" << std::endl; }