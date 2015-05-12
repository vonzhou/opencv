#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //
#include <opencv2/imgproc/imgproc.hpp> // resize()

#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, const char *argv[]){
    Mat big = imread("imgs/linux.jpg",-1);//CV_LOAD_IMAGE_ANYDEPTH);// the big pic
    Mat dst = imread("imgs/200X200.jpg", -1);

    cout << "Mat info:" << endl;
    cout << "Original pic," << big.rows << " : "<< big.cols << endl;
    cout << "dst pic, " << dst.rows << " : "<< dst.cols << endl;
    
    Size s = big.size();
    cout << "Pic Size info:" << endl;
    cout << "Original pic," << s.width << " : "<< s.height << endl;
    s = dst.size();
    cout << "dst  pic," << s.width << " : "<< s.height << endl;
    // resize big to dst 
    cv::resize(big, dst, dst.size());



    // show it on an image
    imshow("big", big);
    imshow("test", dst);
    waitKey(0);

    return 0;
}
