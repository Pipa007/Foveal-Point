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

    Mat kernel(m,n,CV_64F);
    Mat gkernel(m,n,CV_64FC3,Scalar(0.1));

    double sum = 0.0;  // for normalization
    double r,rx, ry, s = 2.0 * sigma * sigma;

    double xc= ceil(m*0.5);
    double yc= ceil(n*0.5);

    for (int x = 0; x < m; ++x) {
        rx = ((x-xc)*(x-xc));

        for(int y = 0; y < n; ++y) {

            //r = sqrt((x-xc)*(x-xc) + (y-yc)*(y-yc));
            //gkernel.at<double>(x,y) = (exp(-(r*r)/s))/(M_PI * s);

            ry = ((y-yc)*(y-yc));


            // FOR ONE CHANNEL
//            kernel.at<double>(x,y) = exp(-(rx + ry)/s)*(1/(M_PI*s));
//            sum += kernel.at<double>(x,y);


            // FOR 3 CHANNELS
            gkernel.at<Vec3f>(x,y)[0] = exp(-(rx + ry)/s);//*(1/(M_PI*s));
            gkernel.at<Vec3f>(x,y)[1] = exp(-(rx + ry)/s);//*(1/(M_PI*s));
            gkernel.at<Vec3f>(x,y)[2] = exp(-(rx + ry)/s);//*(1/(M_PI*s));


            // Acumulate kernel values
            sum += gkernel.at<Vec3f>(x,y)[0];
            sum += gkernel.at<Vec3f>(x,y)[1];
            sum += gkernel.at<Vec3f>(x,y)[2];


//            // Miguel:
//            kernel.at<double>(x,y) = exp(-(rx + ry)/s) ;//*(1/(M_PI*s));   // hg
//            sum += kernel.at<double>(x,y);

//            gkernel.at<Vec3f>(x,y)[0] = kernel.at<double>(x,y) / sum;
//            gkernel.at<Vec3f>(x,y)[1] = gkernel.at<Vec3f>(x,y)[0];
//            gkernel.at<Vec3f>(x,y)[2] = gkernel.at<Vec3f>(x,y)[0];



        }
    }

    //imshow("Show kernel", gkernel);
    //waitKey(2000);


    // normalize the Kernel
    for(int x = 0; x < m; ++x){
        for(int y = 0; y < n; ++y){
            // FOR ONE CHANNEL
            //gkernel.at<double>(x,y) /= sum;

            // FOR 3 CHANNELS
            gkernel.at<Vec3f>(x,y)[0] /= sum;
            gkernel.at<Vec3f>(x,y)[1] /= sum;
            gkernel.at<Vec3f>(x,y)[2] /= sum;


//            gkernel.at<Vec3f>(x,y)[0] = gkernel.at<Vec3f>(x,y)[0] / abs(gkernel.at<Vec3f>(x,y)[0] );
//            gkernel.at<Vec3f>(x,y)[1] = gkernel.at<Vec3f>(x,y)[1] / abs(gkernel.at<Vec3f>(x,y)[1] );
//            gkernel.at<Vec3f>(x,y)[2] = gkernel.at<Vec3f>(x,y)[2] / abs(gkernel.at<Vec3f>(x,y)[2] );

         }
    }


//    for (int x = 0; x < m; ++x) {
//        for(int y = 0; y < n; ++y) {

//            cout << kernel.at<double>(x,y);
//            cout << "\t" <<endl;
//        }
//    }

    //imshow("Show kernel", gkernel);
    //waitKey(2000);


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

    cv::Mat image = imread(file, -1);		 // Read image

    int height = image.size().height;
    int width = image.size().width;

    cout << "Height: " << height << endl;
    cout << "Width: " << width << endl;
    //imshow("Input Image ", image);
    //waitKey(1500);

    std::vector<Mat> kernels;

    for (int l=0; l<levels; ++l){ // for each level

        int m = ceil(height/(powf(2, l)));
        int n = ceil(width/(powf(2, l)));
        cout << "m " << m << "\t" << "n " << n << endl;

        // Build Kernel
        Mat kernel = createFilter(m,n,l*sigma);
        kernels.push_back(kernel);
    }

    // Construct pyramid
    LaplacianBlending pyramid(image,levels, kernels); //  instantiate an object

    // center
    cv::Mat center(2,1,CV_64F);
    center.at<double>(0,0)= ceil(height*0.5);
    center.at<double>(1,0)= ceil(width*0.5);
    cout << "\n Centro: " << center << "\n" << endl;

    // Foveate
    pyramid.foveate(center);

}










