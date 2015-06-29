# Learning OpenCV !!


---

1. feature_methods_compare.cxx : Compare the speed of feature detecting method(just detecting, it is easy to include the descriptors computing into it.)

* FAST ,[Machine Learning for High-speed Corner Detection, 2006](http://www.edwardrosten.com/work/fast.html)
* SIFT,[Distinctive Image Features from Scale-Invariant Keypoints,2004](http://en.wikipedia.org/wiki/Scale-invariant_feature_transform), invariant to image translation, scaling, and rotation, partially invariant to illumination changes and robust to local geometric distortion
* SURF,[Speeded Up Robust Features,2006](http://en.wikipedia.org/wiki/Speeded_up_robust_features),受SIFT启发，比SIFT快，健壮
* ORB,[ORB: an efficient alternative to SIFT or SURF,2011](http://en.wikipedia.org/wiki/ORB_(feature_descriptor))，基于FAST，比SIFT快两个数量级，可作为SIFT的替代
* BRISK，[BRISK: Binary Robust Invariant Scalable Keypoints ](http://www.asl.ethz.ch/people/lestefan/personal/iccv2011.pdf)
* STAR，[Censure: Center surround extremas for realtime feature detection and matching](),引用次数不高
* MSER，[Robust Wide Baseline Stereo from Maximally Stable Extremal Regions,2002](http://en.wikipedia.org/wiki/Maximally_stable_extremal_regions),斑点检测
* GFTT，[Good Features to Track,1994](http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#shi94),Determines strong corners on an image
* HARRIS,[Harris and M. Stephens (1988). "A combined corner and edge detector"](http://en.wikipedia.org/wiki/Corner_detection),也是一种角点检测方法

[Test Results]()

2. match_brouteforce.cxx : Descriptors Matching using broute force method. it is worked with SIFT, SURF, ORB(provided by params)
3. match_brouteforce_fast.cxx : Fast detecting + Surf descriptors!
4. SIFT_match.cpp : Extract SIFT descriptors and match two images by broute force, **Using ratio test and symmetric test , it is very effective!** 
5. SIFT_match2.cpp : As above , but use Flann-based matcher (kd-tree) !
6. orb_write.cpp : Extract ORB descriptors of a image and dump to a file! 
7. orb_write_batch.cpp : Extract ORB descriptors of image dataset and dump them  to a coresponding files! **very useful**....
8. sift_write.cpp : Extract SIFT descriptors of a image and dump to a file!
9. ORB_match0.cpp : 

very easy segments:
sift_demo.cxx : Hello SIFT...
read_image.cxx : Read a image and show ....
resize.cxx : Resize image...
sift_descriptor.cxx : To see SIFT descriptors...


