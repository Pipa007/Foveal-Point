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
// Gaussian pyramid
void downsample(Mat &img, int scale){

    int s = scale;

    vector<int> param;
    param.push_back(CV_IMWRITE_PNG_COMPRESSION);
    param.push_back(9);

    while(s){
        pyrDown(img,img,Size(),4);
        s--;
    }
}

/****************************************/
/****************************************/
// Laplacian pyramid
void upsample(Mat &img){

    pyrUp(img,img, Size(img.cols*2, img.rows*2));

}
/*******************************************************/
/*******************************************************/
/*******************************************************/




/*****************************************/
//                  CLASSES
/*****************************************/

//class Foveate
//{
//public:
//    cv::Mat image;

//    std::vector<Mat> kernels;

//    std::vector<Mat> imageLapPyr;
//    std::vector<Mat> foveatedPyr;
//    std::vector<Mat> image_sizes;
//    std::vector<Mat> kernel_sizes;

//    Mat imageSmallestLevel;
//    Mat down,up;
//    Mat foveated_image;

//    void buildPyramids(Mat& image, int levels);
//};



///*****************************************/
////        FUNCTIONS OF CLASSES
///*****************************************/

//void Foveate::buildPyramids(Mat& image, int levels){

//    imageLapPyr.resize(levels);


//    imageLapPyr.clear();
//    Mat currentImg = image;
//    Mat lap = image;

//    imshow( "Input Image", currentImg );
//    cv::waitKey(2000);

//    for (int l=0; l<levels; l++){

//        //Mat image;
//        pyrDown(currentImg, down);          // pyrDown(src, dst)
//        //imshow( "Down ", down );
//        //cv::waitKey(2000);

//        pyrUp(down, up, currentImg.size()); // pyrUp(src, dst)
//        //imshow( "Up ", up );
//        //cv::waitKey(2000);

//        Mat lap = currentImg - up;  // cv::subtract(currentImg, up, lap);
//        //imshow("difer", lap);
//        //cv::waitKey(2000);

//        imageLapPyr[l] = lap;
//        //imwrite("files/level_1_lapPyr.jpg", imageLapPyr[0]);
//        currentImg = down;
//        cout << "iteration " << l << endl;
//    }

//    imageSmallestLevel=up;

//}




/*****************************************/
//		MAIN
/*****************************************/

int main(int argc, char** argv){

    // Initialization
    int sigma = 10; // Fovea size: 3, 10 or 25
    int levels = 5; // number of pyramid levels

    // read one image
    string file = string(argv[1]) + "ILSVRC2012_val_00003.JPEG";            // load image

    Mat image = imread(file, -1);		 // Read image

    int height = image.size().height;
    int width = image.size().width;

    cout << "Height: " << height << endl;
    cout << "Width: " << width << endl;


    /*************************************/
    //          Build kernels
    std::vector<Mat> kernels;


    // Construct pyramid
    LaplacianBlending pyramid(image,levels, kernels); //  instantiate an object

    // Foveate
    //pyramid.foveate(center);

}









   /* for (int i = 1; i <= levels; ++i) { // for each level
        // four times bigger than original image to guarantee that...
        float m = height*4/(powf(2, i-1));
        float n = width*4/(powf(2, i-1));
        cout << "m " << m << " - n " << n << endl;

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
   /*  }

    int s = 2; // scale

    cv::imshow("imagem",img);
    cv::waitKey(5000);

    // Primeiro passo: Construcao da piramide reduzida
    downsample(img,s);   // call function

    cv::imshow("imagem downsample",img);
    cv::waitKey(5000);

    cout << img.size().height << "\n" << img.size().width << endl;


    // Segundo passo: Construcao da piramide laplaciana
    upsample(img);

    cv::imshow("imagem Upsample",img);
    cv::waitKey(5000);

    cout << img.size().height << "\n" << img.size().width << endl;*/



// Initialize foveal stereo object

// laplacian_mex_ = laplacian_interface(img, levels, kernel);










/*





pyramid={1,levels};


[B]=get_pyramid(laplacian_mex_);

f=scatter (100,50,10, 'r');
pauseTimeInSeconds = 0.01;
durationInSeconds = 10*1;
        cordX=Data.GazeSearch(r).gazeCordX;
        cordX=cordX(cordX~=0);
        cordX=cordX(cordX~=512);
        cordY=Data.GazeSearch(r).gazeCordY;
        cordY=cordY(cordY~=0);
        cordY=cordY(cordY~=384);

%open(myVideo);
%for i=1:size(cordX,2)/2
%     for j=1:100:height
        
        
     
         center=[0 0]'
%        center=[cordX(i) cordY(i)]'

        tic()
        foveated_image=foveate(laplacian_mex_,center);
        toc()

    imshow(foveated_image);
    hold on 
    delete(f);
*/



