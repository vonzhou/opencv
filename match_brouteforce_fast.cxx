#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //
#include <opencv2/nonfree/nonfree.hpp>

#include <iostream>
#include <dirent.h>
using namespace cv;
using namespace std;

#define IMG_DIR "./imgs/"

bool has_suffix(const std::string &str, const std::string &suffix);

void showFiles(string &method){

    DIR *dir;
    struct dirent *ent;
    cout << "We Have Imags as follows:\n";
    if((dir = opendir(IMG_DIR)) != NULL){
        int count = 0;
        while((ent = readdir(dir)) != NULL){
            string name = (string)(ent->d_name);
            // jpg , jpeg imags
            if(has_suffix(name, "g")){
                count ++;
               cout << name << "\t";
               if(count == 5){
                cout << endl;
                count = 0;
               }
            }
        }
    }else{
        cout << "cannot open the directory!\n";
        exit(-1);
    }
    cout << "\n Choose Two ,I will match them by " << method <<endl;

}

int main(int argc, const char *argv[]){

    if(argc != 2){
        cout << "usage:match <method>\n" ;
        exit(-1);
    }
    string method = string(argv[1]);

    showFiles(method);

    string image1_name,image2_name;
    getline(cin, image1_name);
    getline(cin, image2_name);

    Mat image1 = imread(IMG_DIR + image1_name, 0);
    Mat image2 = imread(IMG_DIR + image2_name, 0);

    FastFeatureDetector fast(40);   // 检测的阈值为40  
    SurfDescriptorExtractor extractor;
   
    //Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SIFT"); 
    // WHY CANNOT WORK ???

    vector<KeyPoint> keypoints1, keypoints2;
    fast.detect(image1, keypoints1);
    fast.detect(image2, keypoints2);

    cout << "# keypoints of image1 :" << keypoints1.size() << endl;
    cout << "# keypoints of image2 :" << keypoints2.size() << endl;
   
    Mat descriptors1,descriptors2;
    extractor.compute(image1,keypoints1,descriptors1);
    extractor.compute(image2,keypoints2,descriptors2);

    BFMatcher bfmatcher(NORM_L2, true);
    vector<DMatch> matches;
    bfmatcher.match(descriptors1, descriptors2, matches);
    cout << "# matches : " << matches.size() << endl;

    // show it on an image
    Mat output;
    drawMatches(image1, keypoints1, image2, keypoints2, matches, output);
    imshow("Matches result",output);
    waitKey(0);

    return 0;
}


bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}