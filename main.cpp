/*
 * Image search demo entry
 * test in local machine , its better to transfer the iamges by socket ..
 * @vonzhou
*/

#include "orb.hpp"

// Search top 20 pictures ....
int main(int argc, const char *argv[]){

    if(argc != 3){
        cout << "usage:search <image> <dataset dir>\n" ;
        exit(-1);
    }
  
    string image_name = string(argv[1]), dir_name = string(argv[2]);
    std::vector<string> result = searchImages_ORB(image_name, dir_name, 30, 4000);
    //cout << "result:" << result.size() << endl;
    // After get the top  k, we show it...
    // for(int i=0; i < result.size(); i++){
    // 	Mat mat = imread(result[i], 1);
    // 	imshow("result"+i, mat);
    // }

    // waitKey(0);
    return 0;
}