
/*  Use ORB to match images, the critical point is to use ratio test
 *  to get good matches!!
 * 1. ratio test
 * 2.symmetric test
 * 3. Use image dataset 4 images a group to score them and statistic!
 *    
 * @vonzhou
*/

#include "orb.hpp"

 struct  MatchScore{
    int query_image_id;
    int train_image_id;
    int query_image_keypoints_size;
    int train_image_keypoints_size;
    int good_matches_size;
    float jaccard;
};

// The Comparator to sort the similarities...
struct  ScoreComp{
    bool operator()(MatchScore &a, MatchScore &b){
        return a.jaccard < b.jaccard;
    }
}comp;


bool has_suffix(const std::string &str, const std::string &suffix);
vector<string> match_scores_sort(std::vector<MatchScore> v, int k);
vector<DMatch> ratio_test(vector< vector<DMatch> > matches12, double ratio);
vector<DMatch> symmetric_test(std::vector<DMatch> good_matches1, std::vector<DMatch> good_matches2);
int get_score(int num, priority_queue<MatchScore, vector<MatchScore>, ScoreComp> q, int group_size);
Mat resize_image(Mat origin, float factor);

Mat orb_read(string input_file){
  Mat descriptors; 
  FileStorage fs(input_file, FileStorage::READ);
  fs["descriptor"] >> descriptors;
  fs.release();
  return descriptors;
}

/*
 * This time we know how the images are named, 4 images a group, use one for query image and 
 * see if the other 3 images are searched, score each query 4,3,2....
 * num : the input query image id (so the group is num ~ num+k)
 * return : score for this query
*/
int  get_search_score(string input_image_prefix, int num, int group_size){

    // we will format the image name from its ids
    std::ostringstream strbuf;
    strbuf << setfill('0') << setw(5) << num;
    string query_name = IMAGE_DIR + input_image_prefix + strbuf.str() + ".jpg";
    //cout << query_name << endl;

    // NB. This time the train images are whate their ORBs indicate...
    string orb_dir = string(IMAGE_DIR) + "orbs/";
    // cout << query_name << "--" << orb_dir << endl;
    string orb_file ;

    //1. Get the image pair matrix...
    Mat query_image = imread(query_name, 1);
    query_image = resize_image(query_image, SCALE_FACTOR);
    Mat image;

    Ptr<FeatureDetector> detector;
    Ptr<DescriptorExtractor> extractor;
    
    detector = new OrbFeatureDetector(FEATURES_SIZE);
    extractor = DescriptorExtractor::create("ORB");

    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1,descriptors2;
     detector->detect(query_image, keypoints1);
     extractor->compute(query_image,keypoints1,descriptors1);

    vector< vector<DMatch> > matches12, matches21;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

    std::vector<string> result;
    std::vector<MatchScore> match_scores;

    // clock_t begin = clock();

    priority_queue<MatchScore, std::vector<MatchScore>, ScoreComp> q; // To get the top k matches...

    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(orb_dir.c_str())) != NULL){
        while((ent = readdir(dir)) != NULL){
            string name = (string)(ent->d_name);
            // jpg , jpeg png etc images(filter other files that is not image)  FIXME
            if(has_suffix(name, "xml")){

                orb_file = orb_dir + name;
                //We load the orb descriptors from the file not compute....
                Mat descriptors2 = orb_read(orb_file);
                
                //3.Match the descriptors in two directions...
                matcher->knnMatch( descriptors1, descriptors2, matches12, 2 );
                matcher->knnMatch( descriptors2, descriptors1, matches21, 2 );

                //4. ratio test proposed by David Lowe paper = 0.8
                std::vector<DMatch> good_matches1, good_matches2;
                good_matches1 = ratio_test(matches12, RATIO);
                good_matches2 = ratio_test(matches21, RATIO);

                // cout << "Good matches1:" << good_matches1.size() << endl;
                // cout << "Good matches2:" << good_matches2.size() << endl;

                // Symmetric Test
                std::vector<DMatch> better_matches;
                better_matches = symmetric_test(good_matches1, good_matches2);
    
                //cout << "Better matches:" << better_matches.size() << endl;

                //5. Compute the similarity of this image pair...
                float jaccard = 1.0 * better_matches.size() / (keypoints1.size() + keypoints2.size() - better_matches.size());
                MatchScore ms;
                ms.query_image_id = num;
                // Get the image id from image name...(such , ukbench00132.jpg - > 132)
                int start = name.find(input_image_prefix) + input_image_prefix.length() - 1;
                string str_id = name.substr(start+1, name.find_last_of(".") - 1);
                //cout << str_id << endl;
                ms.train_image_id = stoi(str_id);  // USE -std=c++11  
                ms.query_image_keypoints_size = keypoints1.size();
                ms.train_image_keypoints_size = keypoints2.size();
                ms.good_matches_size =  better_matches.size();
                ms.jaccard = jaccard;
               
                q.push(ms);
                //cout << query_name << "-" << orb_file<< ",jaccard:" << jaccard << endl;
            }
        }
    }else{
        cout << "Oops, cannot open the directory!\n";
        exit(-1);
    }

    //SO, Get the top k and score this query...
    int score = get_score(num, q, group_size);
    cout << "Score For " << num << " = " << score << endl;

    // clock_t end = clock();
    // double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    // cout << "Time Costs : " << elapsed_secs << endl;
    return score;
}

bool has_suffix(const std::string &str, const std::string &suffix){
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

 //ratio test nearest/second nearest < ratio
vector<DMatch> ratio_test(vector< vector<DMatch> > matches12, double ratio){
    vector<DMatch> good_matches;         
    for(int i = 0; i < matches12.size(); i++){  
        if(matches12[i][0].distance < ratio * matches12[i][1].distance)
            good_matches.push_back(matches12[i][0]);
    }
    return good_matches;                  
}

// Symmetric Test...
vector<DMatch> symmetric_test(std::vector<DMatch> good_matches1, std::vector<DMatch> good_matches2){
    std::vector<DMatch> better_matches;
    for(int i=0; i<good_matches1.size(); i++){
        for(int j=0; j<good_matches2.size(); j++){
            if(good_matches1[i].queryIdx == good_matches2[j].trainIdx && good_matches2[j].queryIdx == good_matches1[i].trainIdx){
                better_matches.push_back(DMatch(good_matches1[i].queryIdx, good_matches1[i].trainIdx, good_matches1[i].distance));
                break;
            }
        }
    }

    return better_matches;
}

int get_score(int num, priority_queue<MatchScore, vector<MatchScore>, ScoreComp> q, int group_size){
    // get top k and put it in a set
    set<int> id_set;
    for(int i=0; i < group_size; i++){
        MatchScore temp = q.top();
        int trainIdx = temp.train_image_id;
        id_set.insert(trainIdx);
        q.pop();
    }

    // test if num~num+k is in the searched set
    int score = 0;
    for(int i=0; i<group_size; i++){
        if(id_set.find(num+i) != id_set.end())
            score++;
    }

    return score;
}

// Scale the image by scale
Mat resize_image(Mat origin, float factor){
    Mat big = origin;
    Mat dst;
    // resize big to dst 
    cv::resize(big, dst, cvSize(0,0), factor, factor);

    return dst;
}


