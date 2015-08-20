# Learning OpenCV !!


---

### 1. Feature Detecting Methods Compare

=> feature_methods_compare.cxx : Compare the speed of feature detecting method(just detecting, it is easy to include the descriptors computing into it.)

* FAST ,[Machine Learning for High-speed Corner Detection, 2006](http://www.edwardrosten.com/work/fast.html)
* SIFT,[Distinctive Image Features from Scale-Invariant Keypoints,2004](http://en.wikipedia.org/wiki/Scale-invariant_feature_transform), invariant to image translation, scaling, and rotation, partially invariant to illumination changes and robust to local geometric distortion
* SURF,[Speeded Up Robust Features,2006](http://en.wikipedia.org/wiki/Speeded_up_robust_features),受SIFT启发，比SIFT快，健壮
* ORB,[ORB: an efficient alternative to SIFT or SURF,2011](http://en.wikipedia.org/wiki/ORB_(feature_descriptor))，基于FAST，比SIFT快两个数量级，可作为SIFT的替代
* BRISK，[BRISK: Binary Robust Invariant Scalable Keypoints ](http://www.asl.ethz.ch/people/lestefan/personal/iccv2011.pdf)
* STAR，[Censure: Center surround extremas for realtime feature detection and matching](),引用次数不高
* MSER，[Robust Wide Baseline Stereo from Maximally Stable Extremal Regions,2002](http://en.wikipedia.org/wiki/Maximally_stable_extremal_regions),斑点检测
* GFTT，[Good Features to Track,1994](http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#shi94),Determines strong corners on an image
* HARRIS,[Harris and M. Stephens (1988). "A combined corner and edge detector"](http://en.wikipedia.org/wiki/Corner_detection),也是一种角点检测方法

[Test Results](http://blog.csdn.net/vonzhoufz/article/details/46594369)


### 2.To See How Ratio impact the ORB Descriptors Matching.

=> ORB_match0.cpp : detect features, compute descriptors, then broute force match them ,but the result is bad, even not similar images also mathces too many!

=> ORB_match.cpp : **After the ratio test and symmetric test, the result is good, but with ORB the Jaccard similarity is low.(Q)**

| ratio   |  image1 keypoints size    |  image2 keypoints size     |  Good matches1  |   Good matches2 | Better matches | 
| --------  | :-----:    | :----:    |  :----:  | :----:  | :----:  | 
|0.9  |  500 |   487 | 287    | 263    |176     |
|0.85  |  500 |   487 | 237    | 218    |157     |
|0.8  |  500 |   487 | 209    | 192    |144     |
|0.75  |  500 |   487 | 170    | 160    |120     |
|0.65  |  500 |   487 | 113    | 106    |76     |

**So, You can see the trend!**


### 3.Combine Them Together to Implement Image Retrieval by similarity.

=> [image-search](image-search/README.md)

=> **Test Results** 

1. [500 features with ratio 0.9](test_log_500_0.9.md)

1. [1000 features with ratio 0.8](test_log_1000_0.8.md)



### 4. How the Query Image size impact the retrieval score.

[test result](image-search2/README.md)



### 5. Dump the Descriptors to file for using next time, make it faster to process large dataset. There are two mehtods:1)You can define your own serialization format, such as [my demo](descriptor-persistence/orb_write_my.cpp), 2) Using OpenCV built in component [FileStorage](http://docs.opencv.org/modules/core/doc/xml_yaml_persistence.html). Here I Choose the second method for its easy use.

==> [write ORB descriptors to file batchly](descriptor-persistence/orb_write_batch.cpp)
==> [write SIFT descriptors to file batchly](descriptor-persistence/sift_write_batch.cpp)


### 6.1 This time I get train images' ORB descriptors from file to do the image retrieval!

[test result](image-search3/README.md)

### 6.2 As above, get train images' SIFT descriptors from files, and then do image retrieval.

[Source Code](image-search-by-sift/README.md)


### 7. Match descriptors of SIFT,ORB,FAST,etc ,and show how two images matched...

==>[match](match/)


### 8.Image resize by scale factor and dump them to specified folders.

==>[resize](resize/)


### 8.Other easy demo on the road.

==>[demo](tutorial/)


### 9. My Blog is Here [vonzhou](http://blog.csdn.net/vonzhoufz).










