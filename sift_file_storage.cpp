    #include <opencv2/opencv.hpp>
    #include <opencv2/nonfree/nonfree.hpp>
    #include <fstream>

    using namespace cv;
    using namespace std;

    int main(int argc, const char* argv[])
    {
      const cv::Mat img1 = cv::imread("imgs/car1.jpeg", 0); //Load as grayscale

      cv::SiftFeatureDetector detector;
      std::vector<cv::KeyPoint> keypoints;
      detector.detect(img1, keypoints);

      cv::SiftDescriptorExtractor extractor;
      cv::Mat descriptors;
      extractor.compute(img1, keypoints, descriptors);
      cout << descriptors.rows << endl;
      
      cv::FileStorage fsWrite("descriptor.xml", FileStorage::WRITE );
      // Yes , it works !!!
      fsWrite << "descriptor" << descriptors;
      fsWrite.release();

      Mat sift_descriptor; 
      FileStorage fs("descriptor.xml", FileStorage::READ);
      fs["descriptor"] >> sift_descriptor;
      fs.release();  

      cout << sift_descriptor.rows << endl;

      return 0;
    }