#include "opencv2/core/core.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int,char**)
{
    // create by using the constructor
    //! [constructor]
    Mat M(2,2, CV_8UC3, Scalar(0,0,255));
    cout << "M = " << endl << " " << M << endl << endl;
    //! [constructor]

    // create a 3x3 double-precision identity matrix
    //! [comma]
    Mat C = (Mat_<double>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    cout << "C = " << endl << " " << C << endl << endl;
    //! [comma]

    // Fill a matrix with random values
    Mat R = Mat(3, 2, CV_8UC3);
    randu(R, Scalar::all(0), Scalar::all(255));

    // Demonstrate the output formating options
    //! [out-default]
    cout << "R (default) = " << endl <<        R           << endl << endl;

    return 0;
}