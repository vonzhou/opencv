#include "util.hpp"


double average_score(string input_image_prefix, int begin_id, int groups, int group_size){
    int score = 0;
    for(int i=0; i<groups * group_size; i+=group_size){
        int temp = get_search_score(input_image_prefix, begin_id + i, group_size);
        score += temp;
    }

    return 1.0 * score / groups;
}


int main(int argc, const char *argv[]){

    string input_image_prefix = "ukbench";
    // clock_t begin = clock();

    double  score = average_score(input_image_prefix, 0, GROUPS, GROUP_SIZE);
    //int  score = get_search_score(input_image_prefix, 188, 4);
    cout << "Average Score For This Dataset: " << score << endl;

    // clock_t end = clock();
    // double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    // cout << "Time Costs : " << elapsed_secs << endl;
  
    return 0;
}