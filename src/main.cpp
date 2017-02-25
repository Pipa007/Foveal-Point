#include <caffe/caffe.hpp>
#include <caffe/util/io.hpp>
#include <caffe/blob.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <math.h>
#include <cmath>
#include <limits>

#include "laplacian_foveation.hpp"
using namespace cv;
using namespace std;
using std::string;

#define CV_64FC3 CV_MAKETYPE(CV_64F,3)


/*****************************************/
//		MAIN
/*****************************************/

int main(int argc, char** argv){

    // Initialization
    int sigma = 50; // Fovea size: standard deviation
    int levels = 5; // number of pyramid levels

    string file = string(argv[1]) + "ILSVRC2012_val_00000001.JPEG";   // load image
    //string file = string(argv[1]) + "wc.jpg";

    cv::Mat image = imread(file, 1);		 // Read image

    int height = image.size().height;
    int width = image.size().width;

    int m = floor(4.0*height);
    int n = floor(4.0*width);

    image.convertTo(image, CV_64F);

    // Compute kernels
    std::vector<Mat> kernels=createFilterPyr(m,n,levels,sigma);

    // Construct pyramid
    LaplacianBlending pyramid(image,levels, kernels); //  instantiate an object

    // center
    cv::Mat center(2,1,CV_32S);
    center.at<int>(0,0)= height*0.25;
    center.at<int>(1,0)= width*0.25;

    // Foveate
    cv::Mat foveated_image = pyramid.foveate(center);

//    foveated_image.convertTo(foveated_image, CV_8UC3);
//    imshow("Foveated image", foveated_image);
//    waitKey(1000);


}










