/*
 * use a image in a group as query image, search the dataset and get the score
 * The image dataset has 56 groups , every group has 4 images.SO:
 * @vonzhou
*/

#include "orb.hpp"


double average_score(string input_image_prefix, int begin_id, int groups, int group_size){
    int score = 0;
    for(int i=0; i<groups * group_size; i+=group_size){
        int temp = get_search_score(input_image_prefix, begin_id + i, group_size);
        score += temp;
    }

    return 1.0 * score / groups;
}


int main(int argc, const char *argv[]){

    string input_image_prefix = "ukbench00";
    double  score = average_score(input_image_prefix, 132, 56, 4);
    //int  score = get_search_score(input_image_prefix, 188, 4);
    cout << "Average Score For This Dataset: " << score << endl;
  
    return 0;
}