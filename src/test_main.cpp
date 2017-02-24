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
//                  FUNCTIONS
/*****************************************/


Mat createFilter(int m, int n, int sigma){


    //Mat kernel(m,n,CV_64F);
    Mat gkernel(m,n,CV_64FC3);

    double r,rx, ry, s = 2.0 * sigma * sigma;

    double xc= n*0.5;
    double yc= m*0.5;
    double max_value=-std::numeric_limits<double>::max();

    for (int x = 0; x < n; ++x) {
        rx = ((x-xc)*(x-xc));

        for(int y = 0; y < m; ++y) {

            ry = ((y-yc)*(y-yc));

            // FOR 3 CHANNELS
            gkernel.at<Vec3d>(y,x)[0] = exp(-(rx + ry)/s);
            gkernel.at<Vec3d>(y,x)[1] = exp(-(rx + ry)/s);
            gkernel.at<Vec3d>(y,x)[2] = exp(-(rx + ry)/s);

            if(gkernel.at<Vec3d>(y,x)[0]>max_value)
            {
                max_value=gkernel.at<Vec3d>(y,x)[0];
            }

        }
    }


    // normalize the Kernel
    for(int x = 0; x < n; ++x){
        for(int y = 0; y < m; ++y){

            // FOR 3 CHANNELS
            gkernel.at<Vec3d>(y,x)[0] /= max_value;
            gkernel.at<Vec3d>(y,x)[1] /= max_value;
            gkernel.at<Vec3d>(y,x)[2] /= max_value;

         }
    }

    return gkernel;
}


std::vector<Mat> createFilterPyr(int m, int n, int levels, int sigma){

    std::vector<Mat> kernels;
    Mat gkernel=createFilter(m,n,sigma);
    kernels.push_back(gkernel);

    std::cout << "OI"<< std::endl;

    for (int l=0; l<levels; ++l)
    {
        Mat kernel_down;
        std::cout << "OI"<<l<< std::endl;

        pyrDown(kernels[l], kernel_down);
        std::cout << "sizes: "<< kernels[l].size() << std::endl;
        kernels.push_back(kernel_down);

    }
    return kernels;

}
/*****************************************/
//		MAIN
/*****************************************/

int main(int argc, char** argv){

    // Initialization
    int sigma = 50; // Fovea size: standard deviation
    int levels = 5; // number of pyramid levels

    // read one image
    string file = string(argv[1]) + "ILSVRC2012_val_00000001.JPEG";   // load image
    //string file = string(argv[1]) + "wc.jpg";

    cv::Mat image = imread(file, 1);		 // Read image

    int height = image.size().height;
    int width = image.size().width;

    cout << "Height: " << height << endl;
    cout << "Width: " << width << endl;
    //imshow("Input Image ", image);
    //waitKey(1500);

    int m = floor(4.0*height);
    int n = floor(4.0*width);

    image.convertTo(image, CV_64F);
    std::vector<Mat> kernels=createFilterPyr(m,n,levels,sigma);

    /*for (int l=0; l<levels; ++l){ // for each level
        float aux=powf(2, l);

        int m = floor(4.0*height/aux);
        int n = floor(4.0*width/aux);

        cout << "m " << m << "\t" << "n " << n << endl;

        // Build Kernel
        Mat kernel = createFilter(m,n,sigma/aux);
        kernels.push_back(kernel);
    }*/

    // Construct pyramid
    LaplacianBlending pyramid(image,levels, kernels); //  instantiate an object

    // center
    cv::Mat center(2,1,CV_32S);
    center.at<int>(0,0)= height*0.25;
    center.at<int>(1,0)= width*0.1;
    cout << "center: " << center << endl;

    // Foveate
    cv::Mat foveated_image = pyramid.foveate(center);

    foveated_image.convertTo(foveated_image, CV_8UC3);
    imshow("Foveated image", foveated_image);
    waitKey(0);


}










