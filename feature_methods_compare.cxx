#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //
#include <opencv2/nonfree/nonfree.hpp>

#include <string>
#include <iostream> 
#include <dirent.h>

#include <ctime>

using namespace cv;
using namespace std;

#define IMG_NUM 50;
#define IMG_DIR "./imgs/"

void extract_faeatures(std::vector<Mat> imgs, string method);
bool has_suffix(const std::string &str, const std::string &suffix);

int main(int argc, const char *argv[]){

	if(argc != 2){
		cout << "usage:compare <method>\n" ;
		exit(-1);
	}
    string method = string(argv[1]);

    //We just want to compare the speed of image feature extraction,
    // so read all iamges to memory first.(FIXME)
    vector<Mat> imgs;
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(IMG_DIR)) != NULL){
        while((ent = readdir(dir)) != NULL){
            string name = (string)(ent->d_name);
            // jpg , jpeg imags
            if(has_suffix(name, "g")){
                Mat input = imread(IMG_DIR + name, 0);// load as grayscale
                imgs.push_back(input);
            }
        }
    }else{
        cout << "cannot open the directory!\n";
        exit(-1);
    }

    cout << "Img dataset size :" << imgs.size() << endl;

    // We Need To measure the elapsed time 
    clock_t begin = clock();
    extract_faeatures(imgs, method);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    cout << "Time Costs : " << elapsed_secs << endl;
    
    
    // show the keypoints on an image
    // Mat output;
    // drawKeypoints(input, keypoints, output);
    // imshow("show keypoints",output);
    // waitKey(0);

    return 0;
}

void extract_faeatures(std::vector<Mat> imgs, string method){

    // Get detector 
    initModule_nonfree();
    Ptr<FeatureDetector> detector;

    detector = FeatureDetector::create(method);

    for(std::vector<Mat>::size_type i=0; i != imgs.size(); i++){
        Mat input = imgs[i];
        vector<KeyPoint> keypoints;
        detector->detect(input, keypoints);
        
    
        cout << "Image #KeyPoints : " << keypoints.size() << endl;
    }
}

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}