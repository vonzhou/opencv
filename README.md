# SIFT,SURF,ORB,FAST feature detection comparison


---

主要的特征检测方法有以下几种，在一般的图像处理库中（如opencv, VLFeat, Boofcv等）都会实现。

* FAST ,[Machine Learning for High-speed Corner Detection, 2006](http://www.edwardrosten.com/work/fast.html)
* SIFT,[Distinctive Image Features from Scale-Invariant Keypoints,2004](http://en.wikipedia.org/wiki/Scale-invariant_feature_transform), invariant to image translation, scaling, and rotation, partially invariant to illumination changes and robust to local geometric distortion
* SURF,[Speeded Up Robust Features,2006](http://en.wikipedia.org/wiki/Speeded_up_robust_features),受SIFT启发，比SIFT快，健壮
* ORB,[ORB: an efficient alternative to SIFT or SURF,2011](http://en.wikipedia.org/wiki/ORB_(feature_descriptor))，基于FAST，比SIFT快两个数量级，可作为SIFT的替代
* BRISK，[BRISK: Binary Robust Invariant Scalable Keypoints ](http://www.asl.ethz.ch/people/lestefan/personal/iccv2011.pdf)
* STAR，[Censure: Center surround extremas for realtime feature detection and matching](),引用次数不高
* MSER，[Robust Wide Baseline Stereo from Maximally Stable Extremal Regions,2002](http://en.wikipedia.org/wiki/Maximally_stable_extremal_regions),斑点检测
* GFTT，[Good Features to Track,1994](http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#shi94),Determines strong corners on an image
* HARRIS,[Harris and M. Stephens (1988). "A combined corner and edge detector"](http://en.wikipedia.org/wiki/Corner_detection),也是一种角点检测方法


这里主要关注SIFT,SURF,FAST,ORB的对比。
| Image NO  | SIFT      |  SURF     |  ORB  |   FAST |
| --------  | :-----:    | :----:    |  :----:  | :----:  | 
| 0         |   2414    |   4126       |  500     |  11978       |
| 1         |   4295    |   8129       |  500     |  16763      |
| 2         |   3404    |   4784      |    500   |   16191     |
| 3         |    1639   |  2802      |    500   |    7166    |
| 4         |    1510   |  1484      |    497   |    29562    |
| 5         |    10572   |  8309      |   500    |   720     |
| 6         |    191   |  187      |    295   |    16125    |
| 7         |    3352   |  4706      |   500    |   567     |
| 8         |    165   |  403      |   374    |    26701    |
| 9         |    4899   |  7523      |   500    |   12780     |
| 10        |    1979   |  4212      |   500    |   10676     |
| 11        |    3599   |  3294      |    500   |   663     |
| 12        |    163   |  168      |   287    |     7923   |
| 13        |    1884   |  2413      |   500    |   11681     |
| 14        |    2509   |  5055      |   500    |   18097     |
| 15        |    9177   |  4773      |   500    |   7224     |
| 16        |    3332   |  3217      |   500    |   20502     |
| 17        |    5446   |  6611      |    500   |    16553    |
| 18        |    4592   |  6033      |   500    |    706    |
| 19        |    266   |  509      |   459    |    9613    |
| 20        |    2087   |  2786      |  500     |  7459      |
| 21        |    2582   |  3651      |  500     |   12147     |
| 22        |    2509   |  4237      |  500     |  14890      |
| 23        |    1236   |  4545      |   500    |    6473    |
| 24        |    1311   |  2606      |  500     |   4293     |
| 25        |    237   |  387      |   500    |   657     |
| 26        |    968   |  1418      |   488    |   6609     |
| *Time Cost* |    21.52   |  17.4      |  0.97     |   0.25     |

可以看到FAST提取了大量的特征点，在计算时间上，比SIFT SURF快两个数量级，ORB在FAST基础上得来的，特征点的质量比较高！

下面通过通过两张图片来看这几个算法匹配的效果，1639-1311-697表示图片1，2分别提取了1639，1311个keypoints，其中匹配的有697个。
| Image pair  | SIFT      |  SURF     |  ORB  |   FAST(SURF) |
| --------  | :-----:    | :----:    |  :----:  | :----:  | 
| eiffel-1.jpg，eiffel-13.jpg   |  1639-1311-697    |   2802-2606-1243       |  500-500-251     |  1196-1105-586       |


接下来是eiffel-1.jpg，eiffel-13.jpg 俩图片通过不同算法进行匹配的结果示意图。
![sift]({{match_SIFT.jpg}})




需要注意的地方：
* 链接的时候加上`pkg-config opencv --cflags --libs`可以加入所有opencv的库
* SIFT，SURF是nonfree的，使用的时候需要方法`initModule_nonfree()`，需要头文件opencv2/nonfree/nonfree.hpp
