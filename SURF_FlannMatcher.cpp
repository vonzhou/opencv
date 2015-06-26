/**
 * @file SURF_FlannMatcher
 * @brief SURF detector + descriptor + FLANN Matcher
 * @author A. Huaman
 */

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace cv;

void readme();
std::vector<DMatch> filter_distance(Mat descriptors,std::vector< DMatch > matches);

/**
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )
{
  if( argc != 3 )
  { readme(); return -1; }

  Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
  Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );

  if( !img_1.data || !img_2.data )
  { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 400;

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_1, keypoints_2;

  detector.detect( img_1, keypoints_1 );
  detector.detect( img_2, keypoints_2 );

  //-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;

  Mat descriptors_1, descriptors_2;

  extractor.compute( img_1, keypoints_1, descriptors_1 );
  extractor.compute( img_2, keypoints_2, descriptors_2 );

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;
  // BFMatcher matcher(NORM_L2, true);

  std::vector< DMatch > matches, matches2;
  matcher.match( descriptors_1, descriptors_2, matches );
  matcher.match( descriptors_2, descriptors_1, matches2 );

  std::vector< DMatch > good_matches1, good_matches2, better_matches;
  good_matches1 = filter_distance(descriptors_1, matches);
  good_matches2 = filter_distance(descriptors_2, matches2);

  for(int i=0; i<good_matches1.size(); i++){
    DMatch temp1 = good_matches1[i];
    for(int j=0; j<good_matches2.size(); j++){
      DMatch temp2 = good_matches2[j];
      if(temp1.queryIdx == temp2.trainIdx && temp2.queryIdx == temp1.trainIdx) {
        better_matches.push_back(temp1);
        break;
      }

    }
  }

 
  //-- Draw only "good" matches
  Mat img_matches;
  drawMatches( img_1, keypoints_1, img_2, keypoints_2,
               better_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

  //-- Show detected matches
  imshow( "Good Matches", img_matches );

  for( int i = 0; i < (int)better_matches.size(); i++ )  { 
    printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, better_matches[i].queryIdx, better_matches[i].trainIdx ); 
  }

  waitKey(0);

  return 0;
}

std::vector<DMatch> filter_distance(Mat descriptors,std::vector< DMatch > matches){
  double max_dist = 0; double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors.rows; i++ )  { 
    double dist = matches[i].distance;
    if( dist < min_dist ) 
      min_dist = dist;
    if( dist > max_dist ) 
      max_dist = dist;
  }

  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );

  //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
  //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
  //-- small)
  //-- PS.- radiusMatch can also be used here.
  std::vector< DMatch > good_matches;

  for( int i = 0; i < descriptors.rows; i++ )
  { if( matches[i].distance <= 5 * min_dist )
    { good_matches.push_back( matches[i]); }
  }
  return good_matches;
}

void readme()
{ std::cout << " Usage: ./SURF_FlannMatcher <img1> <img2>" << std::endl; }