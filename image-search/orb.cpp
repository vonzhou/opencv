
/*  Use ORB to match images, the critical point is to use ratio test
 *  to get good matches!!
 * 1. ratio test
 * 2.symmetric test
 *  Q:How to decide the ratio ?? To Test it.
 *    
 * @vonzhou
*/

#include "orb.hpp"

 struct  MatchScore{
    string query_image;
    string train_image;
    int query_image_keypoints_size;
    int train_image_keypoints_size;
    int good_matches_size;
    float jaccard;
};

// The Comparator to sort the similarities...
struct  ScoreComp{
    bool operator()(MatchScore a, MatchScore b){
        return a.jaccard < b.jaccard;
    }
}comp;


bool has_suffix(const std::string &str, const std::string &suffix);
vector<string> match_scores_sort(std::vector<MatchScore> v, int k);
vector<DMatch> ratio_test(vector< vector<DMatch> > matches12, double ratio);
vector<DMatch> symmetric_test(std::vector<DMatch> good_matches1, std::vector<DMatch> good_matches2);

/*
 * param:search image ,the dir of images dataset, the top k
 * return : the top k iamges that are most similar sorted by similarity 
*/
vector<string> searchImages_ORB(string input_image, string img_dir, int k, int nfeatures, double ratio){

    string query_name = input_image;
    string image_name ;

    //1. Get the image pair matrix...
    Mat query_image = imread(query_name, 1);
    Mat image;

    Ptr<FeatureDetector> detector;
    Ptr<DescriptorExtractor> extractor;
    
    if(nfeatures > 500)
        detector = new OrbFeatureDetector(nfeatures);
    else detector = FeatureDetector::create("ORB");  

    extractor = DescriptorExtractor::create("ORB");

    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1,descriptors2;

    vector< vector<DMatch> > matches12, matches21;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

    std::vector<string> result;
    std::vector<MatchScore> match_scores;

    clock_t begin = clock();

    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(img_dir.c_str())) != NULL){
        while((ent = readdir(dir)) != NULL){
            string name = (string)(ent->d_name);
            // jpg , jpeg png etc images(filter other files that is not image)  FIXME
            if(has_suffix(name, "g")){
                image_name = img_dir + name;
                image = imread(image_name, 1);
                // 2. detect features and extract the descriptors
                detector->detect(query_image, keypoints1);
                detector->detect(image, keypoints2);

                // cout << "# keypoints of query_image :" << keypoints1.size() << endl;
                // cout << "# keypoints of image :" << keypoints2.size() << endl;

                extractor->compute(query_image,keypoints1,descriptors1);
                extractor->compute(image,keypoints2,descriptors2);
                //cout << "Descriptors size :" << descriptors1.cols << ":"<< descriptors1.rows << endl;

                //3.Match the descriptors in two directions...
                matcher->knnMatch( descriptors1, descriptors2, matches12, 2 );
                matcher->knnMatch( descriptors2, descriptors1, matches21, 2 );

                // BFMatcher bfmatcher(NORM_L2, true);
                // vector<DMatch> matches;
                // bfmatcher.match(descriptors1, descriptors2, matches);
                // cout << "Matches1-2:" << matches12.size() << endl;
                // cout << "Matches2-1:" << matches21.size() << endl;

                //4. ratio test proposed by David Lowe paper = 0.8
                std::vector<DMatch> good_matches1, good_matches2;
                good_matches1 = ratio_test(matches12, ratio);
                good_matches2 = ratio_test(matches21, ratio);

                // cout << "Good matches1:" << good_matches1.size() << endl;
                // cout << "Good matches2:" << good_matches2.size() << endl;

                // Symmetric Test
                std::vector<DMatch> better_matches;
                better_matches = symmetric_test(good_matches1, good_matches2);
    
                //cout << "Better matches:" << better_matches.size() << endl;

                //5. Compute the similarity of this image pair...
                float jaccard = 1.0 * better_matches.size() / (keypoints1.size() + keypoints2.size() - better_matches.size());
                MatchScore ms;
                ms.query_image = query_name.substr(query_name.find_last_of("/") + 1);
                ms.train_image = name;
                ms.query_image_keypoints_size = keypoints1.size();
                ms.train_image_keypoints_size = keypoints2.size();
                ms.good_matches_size =  better_matches.size();
                ms.jaccard = jaccard;
               
                match_scores.push_back(ms);
                //cout << query_name << "-" << image_name<< ",jaccard:" << jaccard << endl;
            }
        }
    }else{
        cout << "cannot open the directory!\n";
        exit(-1);
    }

    // We got all the matches and its scores, so sort them :) FIXME to better top-k
    result = match_scores_sort(match_scores, k);

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    // cout << "Time Costs : " << elapsed_secs << endl;
    return result;
}

vector<string> searchImages_SIFT(string input_image, string img_dir, int k, double ratio){

    string query_name = input_image;
    string image_name ;

    //1. Get the image pair matrix...
    Mat query_image = imread(query_name, 1);
    Mat image;

    Ptr<FeatureDetector> detector;
    Ptr<DescriptorExtractor> extractor;

    initModule_nonfree(); // IF YOU USE SIFT,MAKE SURE TO DO THIS, OR YOU WILL GET COREDUMP!!!
    
    detector = FeatureDetector::create("SIFT");  

    extractor = DescriptorExtractor::create("SIFT");

    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1,descriptors2;

    vector< vector<DMatch> > matches12, matches21;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");

    std::vector<string> result;
    std::vector<MatchScore> match_scores;

    clock_t begin = clock();

    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(img_dir.c_str())) != NULL){
        while((ent = readdir(dir)) != NULL){
            string name = (string)(ent->d_name);
            // jpg , jpeg png etc images(filter other files that is not image)  FIXME
            if(has_suffix(name, "g")){
                image_name = img_dir + name;
                image = imread(image_name, 1);
                // 2. detect features and extract the descriptors
                detector->detect(query_image, keypoints1);
                detector->detect(image, keypoints2);

                // cout << "# keypoints of query_image :" << keypoints1.size() << endl;
                // cout << "# keypoints of image :" << keypoints2.size() << endl;

                extractor->compute(query_image,keypoints1,descriptors1);
                extractor->compute(image,keypoints2,descriptors2);
                //cout << "Descriptors size :" << descriptors1.cols << ":"<< descriptors1.rows << endl;

                //3.Match the descriptors in two directions...
                matcher->knnMatch( descriptors1, descriptors2, matches12, 2 );
                matcher->knnMatch( descriptors2, descriptors1, matches21, 2 );

                // BFMatcher bfmatcher(NORM_L2, true);
                // vector<DMatch> matches;
                // bfmatcher.match(descriptors1, descriptors2, matches);
                // cout << "Matches1-2:" << matches12.size() << endl;
                // cout << "Matches2-1:" << matches21.size() << endl;

                //4. ratio test proposed by David Lowe paper = 0.8
                std::vector<DMatch> good_matches1, good_matches2;
                good_matches1 = ratio_test(matches12, ratio);
                good_matches2 = ratio_test(matches21, ratio);

                // cout << "Good matches1:" << good_matches1.size() << endl;
                // cout << "Good matches2:" << good_matches2.size() << endl;

                // Symmetric Test
                std::vector<DMatch> better_matches;
                better_matches = symmetric_test(good_matches1, good_matches2);
    
                //cout << "Better matches:" << better_matches.size() << endl;

                //5. Compute the similarity of this image pair...
                float jaccard = 1.0 * better_matches.size() / (keypoints1.size() + keypoints2.size() - better_matches.size());
                MatchScore ms;
                ms.query_image = query_name.substr(query_name.find_last_of("/") + 1);
                ms.train_image = name;
                ms.query_image_keypoints_size = keypoints1.size();
                ms.train_image_keypoints_size = keypoints2.size();
                ms.good_matches_size =  better_matches.size();
                ms.jaccard = jaccard;
               
                match_scores.push_back(ms);
                //cout << query_name << "-" << image_name<< ",jaccard:" << jaccard << endl;
            }
        }
    }else{
        cout << "cannot open the directory!\n";
        exit(-1);
    }

    // We got all the matches and its scores, so sort them :) FIXME to better top-k
    result = match_scores_sort(match_scores, k);

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    // cout << "Time Costs : " << elapsed_secs << endl;
    return result;
}

bool has_suffix(const std::string &str, const std::string &suffix){
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// oops, maybe it is better to use heap or priority queue....
vector<string> match_scores_sort(vector<MatchScore> v, int k){
    std::sort(v.begin(), v.end(), comp);
    std::vector<string> res;
    int last = v.size()-1;
    int counter = 1;
    for(int i=last; i > last-k; i--){
        res.push_back(v[i].train_image);
        cout << counter << ": {" << v[i].query_image << ", " << v[i].train_image << ", "
             << v[i].query_image_keypoints_size << ", " << v[i].train_image_keypoints_size << ", "
             << v[i].good_matches_size << ", " << v[i].jaccard << "}" << endl << endl;
        counter ++ ;
    }
    return res;
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



