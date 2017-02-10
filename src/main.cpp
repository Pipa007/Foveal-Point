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
#include <limits>

#include "laplacian_foveation.hpp"
using namespace cv;
using namespace std;
using std::string;



/*****************************************/
//                  FUNCTIONS
/*****************************************/
void createFilter(int m, int n, int sigma)
{

    double kernels[m][n];

    double r, s = 2.0 * sigma * sigma;

    // sum is for normalization
    double sum = 0.0;

    //
    for (int x = -(floor(m/2)-1); x <= (floor(m/2)-1); x++) {

        for(int y = -(floor(n/2)-1); y <= (floor(n/2)-1); y++) {

            r = sqrt(x*x + y*y);
            kernels[x + m/2-1][y + n/2-1] = (exp(-(r*r)/s))/(M_PI * s);
            sum += kernels[x + m/2-1][y + n/2-1];
        }
    }

    // normalize the Kernel
    for(int i = 0; i < floor(m); ++i)
        for(int j = 0; j < floor(n); ++j)
            kernels[i][j] /= sum;

    for(int i = 0; i < floor(m); ++i)
    {
        for (int j = 0; j < floor(n); ++j)

            cout << kernels[i][j] << "\t" <<endl;
        }


}


/*****************************************/
//		MAIN
/*****************************************/

int main(int argc, char** argv){

    // Initialization
    int sigma = 10; // Fovea size: 3, 10 or 25 (standard deviation)
    int levels = 5; // number of pyramid levels

    // read one image
    string file = string(argv[1]) + "ILSVRC2012_val_00003.JPEG";   // load image

    Mat image = imread(file, -1);		 // Read image

    int height = image.size().height;
    int width = image.size().width;

    cout << "Height: " << height << endl;
    cout << "Width: " << width << endl;


    /*************************************/
    //          Build kernels
    /*************************************/
    //std::vector<Mat> kernels;


    for (int l=0; l<levels; l++){ // for each level

        int m = height*4/(powf(2, l));
        int n = width*4/(powf(2, l));
        cout << "m " << m << " - n " << n << endl;

        // Build Kernel
        createFilter(m,n,sigma);

    }


    /* for (int i = 1; i <= levels; ++i) { // for each level

         /*[h1, h2] = meshgrid(-(n-1)/2:(n-1)/2,-(m-1)/2:(m-1)/2);
         hg = exp(- 0.5*(h1.^2+h2.^2) / (i*sigma).^2);

         kernel_aux=0.1*ones(m,n,3);

         kernel_aux(:,:,1)=hg ./ sum(hg(:));
         kernel_aux(:,:,2)=kernel_aux(:,:,1);
         kernel_aux(:,:,3)=kernel_aux(:,:,1);

         % normalize
         kernel_aux(:,:,1) =  kernel_aux(:,:,1)/max(max(abs(kernel_aux(:,:,1))));
         kernel_aux(:,:,2) =  kernel_aux(:,:,2)/max(max(abs(kernel_aux(:,:,2))));
         kernel_aux(:,:,3) =  kernel_aux(:,:,3)/max(max(abs(kernel_aux(:,:,3))));

         kernel{i}=kernel_aux; */



    // Construct pyramid
    LaplacianBlending pyramid(image,levels, kernels); //  instantiate an object

    // Foveate - Send vector with 5 centers per image (center of 5 bbox)
    //pyramid.foveate(center);

}










