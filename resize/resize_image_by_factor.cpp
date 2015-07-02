#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //
#include <opencv2/imgproc/imgproc.hpp> // resize()

#include <string>
#include <iostream> 
#include <dirent.h>

#include <ctime>

using namespace cv;
using namespace std;

void retrieve_with_bf(string input_image, int size, std::vector<string> imgs);
Mat resize(Mat origin, float factor);
bool has_suffix(const std::string &str, const std::string &suffix);

int main(int argc, const char *argv[]){

    string input_image = string(argv[1]);
    Mat big = imread(input_image,1);//CV_LOAD_IMAGE_ANYDEPTH);// the big pic
    Mat dst;

    // show it on an image
    imshow("big", big);
    dst = resize(big, 0.3);
    imshow("test", dst);
    waitKey(0);

	

    // string input_image = string(argv[1]);
    // // size scale 
    // string arg2 = string(argv[2]);
    // int size = atoi(arg2.c_str());


    //1.We get the dataset images
    // vector<string> img_names;
    // DIR *dir;
    // struct dirent *ent;
    // if((dir = opendir(IMG_DIR)) != NULL){
    //     while((ent = readdir(dir)) != NULL){
    //         string name = (string)(ent->d_name);
    //         // jpg , jpeg imags
    //         if(has_suffix(name, "g")){
    //             //Mat input = imread(IMG_DIR + name, 0);// load as grayscale
    //             img_names.push_back(name);
    //         }
    //     }
    // }else{
    //     cout << "cannot open the directory!\n";
    //     exit(-1);
    // }

    // cout << "Img dataset size :" << img_names.size() << endl;

    // // We Need To measure the elapsed time 
    // clock_t begin = clock();
    // retrieve_with_bf(input_image, size, img_names);
    // clock_t end = clock();
    // double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    // cout << "Time Costs : " << elapsed_secs << endl;
    
    return 0;
}

// Scale the image by size
Mat resize(Mat origin, float factor){
    Mat big = origin;
    Mat dst;
    // resize big to dst 
    cv::resize(big, dst, cvSize(0,0), factor, factor);

    return dst;
}

bool has_suffix(const std::string &str, const std::string &suffix){
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}