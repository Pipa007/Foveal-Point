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

            //r = sqrt((x-xc)*(x-xc) + (y-yc)*(y-yc));
            //gkernel.at<double>(x,y) = (exp(-(r*r)/s))/(M_PI * s);

            ry = ((y-yc)*(y-yc));


            // FOR ONE CHANNEL
//            kernel.at<double>(x,y) = exp(-(rx + ry)/s)*(1/(M_PI*s));
//            sum += kernel.at<double>(x,y);


            // FOR 3 CHANNELS
            gkernel.at<Vec3d>(y,x)[0] = exp(-(rx + ry)/s);//*(1/(M_PI*s));
            gkernel.at<Vec3d>(y,x)[1] = exp(-(rx + ry)/s);//*(1/(M_PI*s));
            gkernel.at<Vec3d>(y,x)[2] = exp(-(rx + ry)/s);//*(1/(M_PI*s));

//            gkernel.at<Vec3f>(y,x)[0] = 0.0;//*(1/(M_PI*s));
//            gkernel.at<Vec3f>(y,x)[1] = 0.0;//*(1/(M_PI*s));
//            gkernel.at<Vec3f>(y,x)[2] = 0.0;//*(1/(M_PI*s));

            if(gkernel.at<Vec3d>(y,x)[0]>max_value)
            {
                max_value=gkernel.at<Vec3d>(y,x)[0];
            }

//            if(sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc))<100)
//            {
//                gkernel.at<Vec3f>(y,x)[0] = 1.0;//*(1/(M_PI*s));
//                gkernel.at<Vec3f>(y,x)[1] = 1.0;//*(1/(M_PI*s));
//                gkernel.at<Vec3f>(y,x)[2] = 1.0;//*(1/(M_PI*s));
//            }
//            else
//            {
//                gkernel.at<Vec3f>(y,x)[0] = 0.0;//*(1/(M_PI*s));
//                gkernel.at<Vec3f>(y,x)[1] = 0.0;//*(1/(M_PI*s));
//                gkernel.at<Vec3f>(y,x)[2] = 0.0;//*(1/(M_PI*s));
//           }

            //std::cout << -(rx + ry)/s <<  " " ;




        }
    }

    //std::cout << max_value << std::endl;
    //imshow("Show kernel", gkernel);
    //waitKey(2000);


    // normalize the Kernel
    for(int x = 0; x < n; ++x){
        for(int y = 0; y < m; ++y){
            // FOR ONE CHANNEL
            //gkernel.at<double>(x,y) /= sum;

            // FOR 3 CHANNELS
            gkernel.at<Vec3d>(y,x)[0] /= max_value;
            gkernel.at<Vec3d>(y,x)[1] /= max_value;
            gkernel.at<Vec3d>(y,x)[2] /= max_value;

//            gkernel.at<Vec3d>(y,x)[0] *= 20.0;
//            gkernel.at<Vec3d>(y,x)[1] *= 20.0;
//            gkernel.at<Vec3d>(y,x)[2] *= 20.0;


//            if(abs(x-xc)<10&&abs(y-yc)<10)
//            std::cout << gkernel.at<Vec3d>(y,x)[0] << " "<< std::endl;
////            gkernel.at<Vec3f>(x,y)[0] = gkernel.at<Vec3f>(x,y)[0] / abs(gkernel.at<Vec3f>(x,y)[0] );
////            gkernel.at<Vec3f>(x,y)[1] = gkernel.at<Vec3f>(x,y)[1] / abs(gkernel.at<Vec3f>(x,y)[1] );
////            gkernel.at<Vec3f>(x,y)[2] = gkernel.at<Vec3f>(x,y)[2] / abs(gkernel.at<Vec3f>(x,y)[2] );

         }
    }



//    for (int x = 0; x < m; ++x) {
//        for(int y = 0; y < n; ++y) {

//            cout << kernel.at<double>(x,y);
//            cout << "\t" <<endl;
//        }
//    }

//    //gkernel.convertTo(gkernel, CV_8UC3);
//    imshow("Show kernel", gkernel);    // tirei o transposto
//    waitKey(1000);
//    //gkernel.convertTo(gkernel, CV_64FC3);

    return gkernel;
}


/*****************************************/
//		MAIN
/*****************************************/

int main(int argc, char** argv){

    // Initialization
    int sigma = 50; // Fovea size: standard deviation
    int levels = 5; // number of pyramid levels

    // read one image
    //string file = string(argv[1]) + "ILSVRC2012_val_00000003.JPEG";   // load image
    string file = string(argv[1]) + "wc.jpg";

    cv::Mat image = imread(file, 1);		 // Read image

    int height = image.size().height;
    int width = image.size().width;

    cout << "Height: " << height << endl;
    cout << "Width: " << width << endl;
    //imshow("Input Image ", image);
    //waitKey(1500);


    image.convertTo(image, CV_64F);
    std::vector<Mat> kernels;

    for (int l=0; l<levels; ++l){ // for each level
        float aux=powf(2, l);
        //double aux=pow(2, l);

        int m = round(4.0*height/aux); // height*4/aux;
        int n = round(4.0*width/aux);  // width*4/aux;
        cout << "m " << m << "\t" << "n " << n << endl;

        // Build Kernel
        Mat kernel = createFilter(m,n,sigma/aux);
        kernels.push_back(kernel);
    }

    // Construct pyramid
    LaplacianBlending pyramid(image,levels, kernels); //  instantiate an object

    // center
    cv::Mat center(2,1,CV_32S);
    center.at<int>(0,0)= height*0.75;
    center.at<int>(1,0)= width*0.25;

    // Foveate
    cv::Mat foveated_image=pyramid.foveate(center);

    foveated_image.convertTo(foveated_image, CV_8UC3);
    imshow("Foveated image", foveated_image);
    waitKey(2000);


}










