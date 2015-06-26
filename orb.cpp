
/*  Use ORB to match two images, the critical point is to use ration test
 *  to get good matches!!
 * 1. ratio test
 * 2.symmetric test
 *  Q:How to decide the ratio ??
 *    
 * @vonzhou
*/

#include "orb.hpp"

 struct  MatchScore{
    string image;
    float jaccard;
};

struct  ScoreComp{
    bool operator()(MatchScore a, MatchScore b){
        return a.jaccard < b.jaccard;
    }
}comp;


bool has_suffix(const std::string &str, const std::string &suffix);
std::vector<string> match_scores_sort(std::vector<MatchScore> v, int k);

/*
 * param:search image ,the dir of images dataset, the top k
 * return : the top k iamges that are most similar sorted by similarity \
*/
std::vector<string> searchImages_ORB(string input_image, string img_dir, int k){

    string image1_name = input_image;
    string image2_name ;

    //1. Get the image pair matrix...
    Mat image1 = imread(image1_name, 1);
    Mat image2;

    Ptr<FeatureDetector> detector;
    Ptr<DescriptorExtractor> extractor;
    
    // TODO default is 500 keypoints..but we can change
    detector = FeatureDetector::create("ORB");  
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
                image2_name = img_dir+name;
                image2 = imread(image2_name, 1);
                // 2. detect features and extract the descriptors
                detector->detect(image1, keypoints1);
                detector->detect(image2, keypoints2);

                // cout << "# keypoints of image1 :" << keypoints1.size() << endl;
                // cout << "# keypoints of image2 :" << keypoints2.size() << endl;

                extractor->compute(image1,keypoints1,descriptors1);
                extractor->compute(image2,keypoints2,descriptors2);
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

                for(int i=0; i < matches12.size(); i++){
                    const float ratio = 0.8;
                    if(matches12[i][0].distance < ratio * matches12[i][1].distance)
                    good_matches1.push_back(matches12[i][0]);
                }

                for(int i=0; i < matches21.size(); i++){
                    const float ratio = 0.8;
                    if(matches21[i][0].distance < ratio * matches21[i][1].distance)
                    good_matches2.push_back(matches21[i][0]);
                }

                // cout << "Good matches1:" << good_matches1.size() << endl;
                // cout << "Good matches2:" << good_matches2.size() << endl;

                // Symmetric Test
                std::vector<DMatch> better_matches;
                for(int i=0; i<good_matches1.size(); i++){
                    for(int j=0; j<good_matches2.size(); j++){
                        if(good_matches1[i].queryIdx == good_matches2[j].trainIdx && good_matches2[j].queryIdx == good_matches1[i].trainIdx){
                            better_matches.push_back(DMatch(good_matches1[i].queryIdx, good_matches1[i].trainIdx, good_matches1[i].distance));
                            break;
                        }
                    }
                }

                //cout << "Better matches:" << better_matches.size() << endl;

                //5. Compute the similarity of this image pair...
                float jaccard = 1.0 * better_matches.size() / (keypoints1.size() + keypoints2.size());
                MatchScore ms;
                ms.image = image2_name;
                ms.jaccard = jaccard;
                match_scores.push_back(ms);
                //cout << image1_name << "-" << image2_name<< ",jaccard:" << jaccard << endl;
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

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// oops, maybe it is better to use heap or priority queue....
std::vector<string> match_scores_sort(std::vector<MatchScore> v, int k){
    std::sort(v.begin(), v.end(), comp);
    std::vector<string> res;
    int last = v.size()-1;
    for(int i=last; i > last-k; i--){
        res.push_back(v[i].image);
        cout << v[i].image << endl;
    }
    return res;
}




