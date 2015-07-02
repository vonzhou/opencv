/* ORB descriptors serialization component, compute the descriptors,
 * and store them to disk will make the matching task faster.
 *
 * @vonzhou
 */     
      
      
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <fstream>
      
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

// read 2 images, dump their orb descriptors to file,
// get descriptors from file , do the match.....is it right??
// You can compare with the normal result.
int main(int argc, const char* argv[]){

  Mat image1 = imread(argv[1], 1);
  Mat image2 = imread(argv[2], 1);
  double ratio = 0.8;


  Ptr<FeatureDetector> detector;
  Ptr<DescriptorExtractor> extractor;
    
  // TODO default is 500 keypoints..but we can change
  detector = FeatureDetector::create("ORB");  
  extractor = DescriptorExtractor::create("ORB");

  std::vector<cv::KeyPoint> keypoints1,keypoints2;
  detector->detect(image1, keypoints1);
  detector->detect(image2, keypoints2);

  cv::Mat descriptors1, descriptors2;
  extractor->compute(image1, keypoints1, descriptors1);
  extractor->compute(image2, keypoints2, descriptors2);

  // dump them to files
  orb_write(descriptors1, "1.xml");
  orb_write(descriptors2, "2.xml");

  //balabala....

  // load from the files
  Mat des1, des2;
  des1 = orb_read("1.xml");
  des2 = orb_read("2.xml");

  // This time we match ...
  vector< vector<DMatch> > matches12, matches21;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->knnMatch( des1, des2, matches12, 2 );
    matcher->knnMatch( des2, des1, matches21, 2 );
    
    // BFMatcher bfmatcher(NORM_L2, true);
    // vector<DMatch> matches;
    // bfmatcher.match(descriptors1, descriptors2, matches);
    cout << "Matches1-2:" << matches12.size() << endl;
    cout << "Matches2-1:" << matches21.size() << endl;

    // ratio test proposed by David Lowe paper = 0.8
    std::vector<DMatch> good_matches1, good_matches2;

    // Yes , the code here is redundant, it is easy to reconstruct it ....
    for(int i=0; i < matches12.size(); i++){
        if(matches12[i][0].distance < ratio * matches12[i][1].distance)
            good_matches1.push_back(matches12[i][0]);
    }

    for(int i=0; i < matches21.size(); i++){
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
    // show it on an image
    Mat output;
    drawMatches(image1, keypoints1, image2, keypoints2, better_matches, output);
    imshow("Matches result",output);
    waitKey(0);



  return 0;

}