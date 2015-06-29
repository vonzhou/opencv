/*
 * Extract ORB feature descriptors of a image dataset and write it to file
 * and then deal with the binary descriptors, such that need not extract every time!
 * @vonzhou
 */ 

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp> 
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <string>

using namespace std;
using namespace cv;

bool has_suffix(const std::string &str, const std::string &suffix){
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

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

  if(argc != 3){
        cout << "usage:orb-write <dataset dir> <output dir>\n" ;
        exit(-1);
    }
  
  const char* dir_name = argv[1];
  const char* output_dir = argv[2];
  string image_name;
  Mat image;

  Ptr<FeatureDetector> detector;
  Ptr<DescriptorExtractor> extractor;
    
    // TODO default is 500 keypoints..but we can change
  detector = FeatureDetector::create("ORB");  
  extractor = DescriptorExtractor::create("ORB");

  std::vector<cv::KeyPoint> keypoints;
  cv::Mat descriptors;

  

    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(dir_name)) != NULL){
        while((ent = readdir(dir)) != NULL){
            string name = (string)(ent->d_name);
            // jpg , jpeg png etc images(filter other files that is not image)  FIXME
            if(has_suffix(name, "g")){
              
                  string orb_file_name = string(output_dir) + name + ".orb";
                  //cout << orb_file_name << endl;
                  image_name = dir_name + name;
                  image = imread(image_name, 1);
                  
                  detector->detect(image, keypoints);
                  extractor->compute(image, keypoints, descriptors);
                  
                  
                  std::ofstream file1(orb_file_name.c_str());
                  if (file1.is_open())
                    write_sift(keypoints,descriptors,file1);
                  file1.close();  
            }
        }
    }else{
        cout << "cannot open the directory!\n";
        exit(-1);
    } 
  return 0;
}