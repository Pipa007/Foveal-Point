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



/*****************************************/
//                  FUNCTIONS
/*****************************************/
Mat createFilter(int m, int n, int sigma){

    Mat gkernel(m,n,CV_64F);

    double sum = 0.0;  // for normalization
    double r,rx, ry, s = 2.0 * sigma * sigma;

    double xc= floor(m*0.5);
    double yc= floor(n*0.5);

    for (int x = 0; x < m; ++x) {

        for(int y = 0; y < n; ++y) {

            //r = sqrt((x-xc)*(x-xc) + (y-yc)*(y-yc));
            //gkernel.at<double>(x,y) = (exp(-(r*r)/s))/(M_PI * s);

            rx = ((x-xc)*(x-xc));
            ry = ((y-yc)*(y-yc));

            //gkernel.at<double>(x,y) = (1/sqrt(M_PI*s))*exp((rx + ry)/s);

            gkernel.at<double>(x,y) = exp(-(rx + ry)/s); // *(1/(M_PI*s))
            sum += gkernel.at<double>(x,y);
        }
    }

    imshow("Show kernel", gkernel);
    waitKey(1000);

    // normalize the Kernel
    for(int x = 0; x < m; ++x)
        for(int y = 0; y < n; ++y)
            gkernel.at<double>(x,y) /= sum;


//    for (int x = 0; x < m; ++x) {
//        for(int y = 0; y < n; ++y) {

//            cout << gkernel.at<double>(x,y);
//            cout << "\t" <<endl;
//        }
//    }

//    imshow("Show kernel", gkernel);
//    waitKey(2000);


    return gkernel;
}



/*****************************************/
//		MAIN
/*****************************************/

int main(int argc, char** argv){

    // Initialization
    int sigma = 10; // Fovea size: standard deviation
    int levels = 5; // number of pyramid levels

    // read one image
    //string file = string(argv[1]) + "ILSVRC2012_val_00003.JPEG";   // load image
    string file = string(argv[1]) + "quarto.jpg";

    Mat image = imread(file, -1);		 // Read image
    cout << sizeof(image) << endl;
    //cv::normalize(image, image,1,0,NORM_MINMAX);

    int height = image.size().height;
    int width = image.size().width;

    cout << "Height: " << height << endl;
    cout << "Width: " << width << endl;


    std::vector<Mat> kernels;

    for (int l=0; l<levels; ++l){ // for each level

        int m = height/(powf(2, l));
        int n = width/(powf(2, l));
        cout << "m " << m << "\t" << "n " << n << endl;

        // Build Kernel
        Mat kernel = createFilter(m,n,l*sigma);
        kernels.push_back(kernel);
    }

    // Construct pyramid
    LaplacianBlending pyramid(image,levels, kernels); //  instantiate an object

    // center
    cv::Mat center(2,1,CV_64F);
    center.at<double>(0,0)= height*0.5;
    center.at<double>(1,0)= width*0.5;
    cout << "\n Centro: " << center << "\n" << endl;

    // Foveate
    pyramid.foveate(center);

}










