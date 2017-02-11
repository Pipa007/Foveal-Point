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
Mat createFilter(int m, int n, int sigma)
{

    Mat gkernel(m,n,CV_64F);

    // sum is for normalization
    double sum = 0.0;
    double r, s = 2.0 * sigma * sigma;

    double xc=floor(m*0.5);
    double yc=floor(n*0.5);

    for (int x = 0; x < m; ++x) {

        for(int y = 0; y < n; ++y) {

            r = sqrt((x-xc)*(x-xc) + (y-yc)*(y-yc));

            gkernel.at<double>(x,y) = (exp(-(r*r)/s))/(M_PI * s);

            sum += gkernel.at<double>(x,y);
        }
    }


    // normalize the Kernel
    for(int x = 0; x < m; ++x)
        for(int y = 0; y < n; ++y)
            gkernel.at<double>(x,y) /= sum;



    for (int x = 0; x < m; ++x) {
        for(int y = 0; y < n; ++y) {

            cout << gkernel.at<double>(x,y) << "\t" <<endl;           
        }
    }

    imshow("Show kernel", gkernel);
    waitKey(0);

    // PROBLEMA: VER FORMULA MATEMATICA
    // GOAL:     DESENHAR A GAUSSIANA!!!

    return gkernel;
}


/*****************************************/
//		MAIN
/*****************************************/

int main(int argc, char** argv){

    // Initialization
    int sigma = 10000; // Fovea size: standard deviation
    int levels = 5; // number of pyramid levels

    // read one image
    string file = string(argv[1]) + "ILSVRC2012_val_00003.JPEG";   // load image

    Mat image = imread(file, -1);		 // Read image

    int height = image.size().height;
    int width = image.size().width;

    cout << "Height: " << height << endl;
    cout << "Width: " << width << endl;


    std::vector<Mat> kernels;

    for (int l=0; l<levels; l++){ // for each level

        int m = height/(powf(2, l));
        int n = width/(powf(2, l));
        cout << "m " << m << " - n " << n << endl;

        // Build Kernel
        Mat kernel = createFilter(m,n,sigma);
        kernels.push_back(kernel);
    }

    // Construct pyramid
    LaplacianBlending pyramid(image,levels, kernels); //  instantiate an object

    // center = (0,0)
    cv::Mat center(2,1,CV_64F);
    center.at<double>(0,0)=width*0.5;
    center.at<double>(1,0)=height*0.5;

    // Foveate
    pyramid.foveate(center);

}










