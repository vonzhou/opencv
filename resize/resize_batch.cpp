#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //
#include <opencv2/imgproc/imgproc.hpp> // resize()

#include <string>
#include <iostream> 
#include <ctime>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


using namespace cv;
using namespace std;

void retrieve_with_bf(string input_image, int size, std::vector<string> imgs);
Mat resize(Mat origin, float factor);
bool has_suffix(const std::string &str, const std::string &suffix);

void makeDirectory(const char* str){
    struct stat st={0};

    if(stat(str, &st) == -1){
        mkdir(str, 0700);
    }
}

int main(int argc, const char *argv[]){

    const char* image_dir = argv[1]; // 
    double factor = atof(argv[2]);
    // to_string in c++ 11
    string output_dir = string(argv[1]) + "resize" + string(argv[2]) + "/";
    const char* temp = output_dir.c_str();
    //create the output directory...
    makeDirectory(temp);

    Mat big ;//= imread(input_image,1);//CV_LOAD_IMAGE_ANYDEPTH);// the big pic
    Mat dst;
    string image_name;

    //cout << image_dir << ", " << factor << "," << output_dir << endl;

    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(image_dir)) != NULL){
        while((ent = readdir(dir)) != NULL){
            string name = (string)(ent->d_name);
            // jpg , jpeg png etc images(filter other files that is not image)  FIXME
            if(has_suffix(name, "g")){
                image_name = image_dir + name;
                big = imread(image_name, 1);
                dst = resize(big, factor);
                //cout << output_dir + name << endl;
                imwrite((output_dir + name).c_str(), dst);
                // imshow("test", dst);
                //  waitKey(0);
            }
        }
    }else{
        cout << "cannot open the directory!\n";
        exit(-1);
    }

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