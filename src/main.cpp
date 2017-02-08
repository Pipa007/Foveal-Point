#include <caffe/caffe.hpp>
#include <caffe/util/io.hpp>
#include <caffe/blob.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <math.h>
#include <limits>


using namespace std;
using std::string;


void downscale(cv::Mat &img, int scale){

    int s = scale;

    //imshow("input image", img);
    //cv::waitKey(5000);

    vector<int> param;
    param.push_back(CV_IMWRITE_PNG_COMPRESSION);
    param.push_back(9);

    while(s){
        pyrDown(img,img,cv::Size(),4);
        s--;
    }
    //imshow("Down sampled",img);
    //cv::waitKey(5000);
}



/*****************************************/
//		MAIN
/*****************************************/

int main(int argc, char** argv){

    // read one image
    string file = string(argv[1]) + "ILSVRC2012_val_00000001.JPEG";            // load image

    cv::Mat img = cv::imread(file, -1);		 // Read image

    // Create kernel
    cout << "Height: " << img.size().height << endl;
    cout << "Width: " << img.size().width << endl;

    int height = img.size().height;
    int width = img.size().width;

    // Fovea size: 3, 10 or 25
    int sigma = 10;
    int levels = 5; // number of pyramid levels


    for (int i = 1; i <= levels; ++i) { // for each level
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
     }

    int s = 2; // scale

    cv::imshow("imagem",img);
    cv::waitKey(5000);

    downscale(img,s);   // call function

    cv::imshow("imagem downsample",img);
    cv::waitKey(5000);

    cout << img.size().height << "\n" << img.size().width << endl;

// Initialize foveal stereo object


}



/*


%% Initialize foveal stereo object
laplacian_mex_ = laplacian_interface(img, levels, kernel);

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



