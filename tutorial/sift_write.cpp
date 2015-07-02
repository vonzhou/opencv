#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <fstream>

using namespace std;

// For every keypoints, we output its 
void write_sift(std::vector<cv::KeyPoint> &keys, cv::Mat descriptors, std::ostream &out1){

  cout << descriptors.cols << endl;
  for(int i=0; i < (int) keys.size(); i++)   {
    //out1 << keys[i].pt.x << " " << keys[i].pt.y << " " << keys[i].size << " " << keys[i].angle << " "; 
    //If you don`t need information about keypoints (position, size) you can comment out the string above

    float* descriptor_pointer = descriptors.ptr<float>(i);
    
    for (int j = 0; j < descriptors.cols; j++)
      out1  << *descriptor_pointer ++ << " ";
    out1 << std::endl;
    }
}


int main(int argc, const char* argv[])    {

  const cv::Mat img1 = cv::imread("imgs/car1.jpeg", 0); //Load as grayscale

  cv::SiftFeatureDetector detector;
  std::vector<cv::KeyPoint> keypoints;
  detector.detect(img1, keypoints);

  cv::SiftDescriptorExtractor extractor;
  cv::Mat descriptors;
  extractor.compute(img1, keypoints, descriptors);

  std::ofstream file1("descriptors.sift");
  if (file1.is_open())
    write_sift(keypoints,descriptors,file1);

  file1.close();
  return 0;
}