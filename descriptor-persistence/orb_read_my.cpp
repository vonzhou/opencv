/*
 * Read ORB descriptors from a file writen by orb_write ,
 * and Construct a Mat for Matching use....
 * @vonzhou
 */

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp> //
#include <fstream>

using namespace std;
using namespace cv;

// Read and construct orb descriptors from file
// TODO.....
void read_ORB(std::istream &in1){
  // 1. first get the lines 
  // 2. read line by line to get the descriptors
  // 3. return this Mat

  
}


int main(int argc, const char* argv[])    {

  const cv::Mat img1 = cv::imread("imgs/eiffel-1.jpg", 0); //Load as grayscale

  Ptr<FeatureDetector> detector;
  Ptr<DescriptorExtractor> extractor;
    
    // TODO default is 500 keypoints..but we can change
  detector = FeatureDetector::create("ORB");  
  extractor = DescriptorExtractor::create("ORB");

  std::vector<cv::KeyPoint> keypoints;
  //detector->detect(img1, keypoints);

  cv::Mat descriptors;
  //extractor->compute(img1, keypoints, descriptors);

  //cout << descriptors << endl;

  std::ifstream file1("descriptors.orb");
  if (file1.is_open())
    read_orb(file1);



  file1.close();
  return 0;
}