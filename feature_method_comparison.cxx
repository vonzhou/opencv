#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <map>

#define sq(x) ((x) * (x))

using namespace std;
using namespace cv;

string mainWindow = "featureSelector";
string magniWindow = "magnifier";
Mat origImg, grayImg, colorImg, largeImg, descriptors;
vector<KeyPoint> keypoints;
bool* selected;
int cropSize = 120;
int scaleF = 3;

void update_large(int n){
	Scalar color;
	if (selected[n]){
		color = Scalar(255, 255, 0);
	}
	else{
		color = Scalar(255, 0, 0);
	}
	circle(largeImg, Point(keypoints[n].pt.x*scaleF+cropSize, keypoints[n].pt.y*scaleF+cropSize), 5, color);
}

void update_orig(int n){
	Scalar color;
	if (selected[n]){
		color = Scalar(255, 255, 0);
	}
	else{
		color = Scalar(255, 0, 0);
	}
	circle(colorImg, Point(keypoints[n].pt.x, keypoints[n].pt.y), 15, color);
}

void mouseCallBack(int event, int x, int y, int flags, void*){
	Mat cutImg,centorCursor;
	int mindist = 10000000,nearest = 0,dist;
	int selectedCount = 0;
	switch (event)
	{
	case EVENT_MOUSEMOVE:
		cutImg = Mat(largeImg, Rect(x * scaleF + cropSize / 2, y * scaleF + cropSize / 2, cropSize, cropSize)).clone();
		centorCursor = Mat(cutImg, Rect(cutImg.cols / 2 - 1, cutImg.rows / 2 - 10, 2, 20));
		centorCursor = ~centorCursor;
		centorCursor = Mat(cutImg, Rect(cutImg.cols / 2 - 10, cutImg.rows / 2 - 1, 20, 2));
		centorCursor = ~centorCursor;
		imshow(magniWindow, cutImg);
		break;
	case EVENT_LBUTTONDOWN:
		for (int i = 0; i < keypoints.size(); i++){
			dist = sq(keypoints[i].pt.x - x) + sq(keypoints[i].pt.y - y);
			if (dist < mindist){
				mindist = dist;
				nearest = i;
			}
		}
		selected[nearest] = !selected[nearest];
		update_large(nearest);
		update_orig(nearest);
		imshow(mainWindow, colorImg);
		for (int i = 0; i < keypoints.size(); i++){
			if (selected[i]) {
				selectedCount++;
			}
		}
		cout << "Total selected point: " << selectedCount << endl;
		break;
	default:
		break;
	}
}

int main(int argc, char* argv[])
{
	if (argv[1] == NULL || argv[2] == NULL){
		cout << "opening file failed" << endl;
		return EXIT_FAILURE;
	}
	colorImg = imread(string(argv[2]), IMREAD_COLOR);
	if (colorImg.empty()) {
		cout << "opening file failed" << endl;
		return EXIT_FAILURE;
	}
	string method = string(argv[1]);
	if (!(method == "SIFT" || method == "SURF" || method == "ORB")){
		cout << "specify at least 1 method" << endl;
		return EXIT_FAILURE;
	}

	//init large image
	resize(colorImg, largeImg, Size(), scaleF, scaleF, INTER_LANCZOS4);
	Mat base(Size(largeImg.cols + 2 * cropSize, largeImg.rows + 2 * cropSize), CV_8UC3, CV_RGB(0, 0, 0));
	largeImg.copyTo(Mat(base, Rect(cropSize, cropSize, largeImg.cols, largeImg.rows)));
	largeImg = base;
	//init large end
	
	cvtColor(colorImg, grayImg, CV_BGR2GRAY);
	if (method == "SIFT"){
		SiftFeatureDetector detector;
		detector.detect(grayImg, keypoints);
	}
	else if (method == "SURF"){
		SurfFeatureDetector detector(350);
		detector.detect(grayImg, keypoints);
	}
	else if (method == "ORB"){
		OrbFeatureDetector detector;
		detector.detect(grayImg, keypoints);
	}
	cout << "Image keypoints: " << keypoints.size() << endl;
	selected = new bool[keypoints.size()];
	for (int i = 0; i < keypoints.size(); i++){
		selected[i] = false;
		update_orig(i);
		update_large(i);
	}
	namedWindow(mainWindow, CV_WINDOW_AUTOSIZE);
	namedWindow(magniWindow,CV_WINDOW_AUTOSIZE);
	setMouseCallback(mainWindow, &mouseCallBack);
	imshow(mainWindow, colorImg);
	waitKey(0);
	Mat descriptors;
	vector<KeyPoint> selectedPoints;
	for (int i = 0; i < keypoints.size();i++){
		if (selected[i]){
			selectedPoints.push_back(keypoints[i]);
		}
	}
	if (selectedPoints.size() == 0){
		cout << "no point selected" << endl;
		return EXIT_FAILURE;
	}
	if (method == "SIFT"){
		SiftDescriptorExtractor extractor;
		extractor.compute(grayImg, selectedPoints, descriptors);
	}
	else if (method == "SURF"){
		SurfDescriptorExtractor extractor;
		extractor.compute(grayImg, selectedPoints, descriptors);
	}
	else if (method == "ORB"){
		OrbDescriptorExtractor extractor;
		extractor.compute(grayImg, selectedPoints, descriptors);
	}
	FileStorage fs("keys.xml", CV_STORAGE_WRITE);
	write(fs, "descriptors", descriptors);
	imwrite("keys.jpg", colorImg);
	delete selected;
	return 0;
}