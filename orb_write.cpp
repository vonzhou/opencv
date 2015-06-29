#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp> //
#include <fstream>

using namespace std;
using namespace cv;

// For every keypoints, we output its 
void write_sift(std::vector<cv::KeyPoint> &keys, cv::Mat descriptors, std::ostream &out1){

  cout << descriptors.cols << endl;
  for(int i=0; i < (int) keys.size(); i++)   {
    //out1 << keys[i].pt.x << " " << keys[i].pt.y << " " << keys[i].size << " " << keys[i].angle << " "; 
    //If you don`t need information about keypoints (position, size) you can comment out the string above

    uchar* descriptor_pointer = descriptors.ptr<uchar>(i);
    
    for (int j = 0; j < descriptors.cols; j++)
      out1  << (int)*descriptor_pointer ++ << " ";
      out1 << std::endl;
    }
}


int main(int argc, const char* argv[])    {

  const cv::Mat img1 = cv::imread("imgs/eiffel-1.jpg", 0); //Load as grayscale

  Ptr<FeatureDetector> detector;
  Ptr<DescriptorExtractor> extractor;
    
    // TODO default is 500 keypoints..but we can change
  detector = FeatureDetector::create("ORB");  
  extractor = DescriptorExtractor::create("ORB");

  std::vector<cv::KeyPoint> keypoints;
  detector->detect(img1, keypoints);

  cv::Mat descriptors;
  extractor->compute(img1, keypoints, descriptors);

  cout << descriptors << endl;

  std::ofstream file1("descriptors.orb");
  if (file1.is_open())
    write_sift(keypoints,descriptors,file1);

  file1.close();
  return 0;
}