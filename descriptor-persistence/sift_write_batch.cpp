/* 
 * Write SIFT descriptors to file.
 * @vonzhou
 */     
      
      
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <fstream>
 #include <dirent.h>
      
using namespace cv;
using namespace std;

void orb_write(Mat descriptors, string output_file){
  cv::FileStorage outfile(output_file, FileStorage::WRITE );
  // Yes , it works !!!
  outfile << "descriptor" << descriptors;
  outfile.release();
}

Mat orb_read(string input_file){
  Mat descriptors; 
  FileStorage fs(input_file, FileStorage::READ);
  fs["descriptor"] >> descriptors;
  fs.release();
  return descriptors;
}

bool has_suffix(const std::string &str, const std::string &suffix){
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

int main(int argc, const char* argv[]){

  string img_dir = string(argv[1]);
  string orb_dir = img_dir + "sifts/";
  string image_name;

  double ratio = 0.8;

  vector<KeyPoint> keypoints1, keypoints2;
  Mat descriptors1,descriptors2;
  Mat image;
  Ptr<FeatureDetector> detector;
  Ptr<DescriptorExtractor> extractor;

  initModule_nonfree();// NB. otherwise coredump!

  detector = FeatureDetector::create("SIFT");  
  extractor = DescriptorExtractor::create("SIFT");

  DIR *dir;
    struct dirent *ent;
    if((dir = opendir(img_dir.c_str())) != NULL){
        while((ent = readdir(dir)) != NULL){
            string name = (string)(ent->d_name);
            // jpg , jpeg png etc images(filter other files that is not image)  FIXME
            if(has_suffix(name, "g")){
                image_name = img_dir + name;
                image = imread(image_name, 1);
                // 2. detect features and extract the descriptors
                detector->detect(image, keypoints2);
                extractor->compute(image,keypoints2,descriptors2);
                //cout << "Descriptors size :" << descriptors1.cols << ":"<< descriptors1.rows << endl;
                string outfile = orb_dir + name + ".xml";
                //cout << outfile << endl;
                orb_write(descriptors2, outfile);
            }
        }
    }else{
        cout << "cannot open the directory!\n";
        exit(-1);
    }

   return 0;

}