#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
/*include "mex.h"
#include "foveal_handle.hpp"
#include "mc_convert/mc_convert.hpp"
#include "MxArray.hpp"
#include <conio.h>*/
#include <string>

using namespace cv;
using namespace std;

class LaplacianBlending
{
public:
    cv::Mat image;
    int levels;
    
    std::vector<Mat> kernels;

    std::vector<Mat> imageLapPyr;
    std::vector<Mat> foveatedPyr;
    std::vector<cv::Mat> image_sizes;
    std::vector<cv::Mat> kernel_sizes;
    
    Mat imageSmallestLevel;
    Mat down,up;           
    Mat foveated_image;

    void buildPyramids()
    {
        imageLapPyr.clear();
        Mat currentImg = image;
        imshow("IMAGE ", image);
        waitKey(500);
        
        for (int l=0; l<levels; l++)
        {
            Mat image;
            pyrDown(currentImg, down);
            //imshow("IMAGE ", currentImg);
            //waitKey(1500);

            //imshow("PYRDOWN", down);
            //waitKey(1500);
            pyrUp(down, up, currentImg.size());

            //imshow("PYRUP", up);
            //waitKey(1500);

            Mat lap = currentImg - up;
            
            imageLapPyr[l]=lap;
            
            currentImg = down;

            //imshow("Laplaciano", lap);
            //waitKey(1500);
            //cout << "Level " << l << endl;
        }
        
        imageSmallestLevel=up;
        
    }


public:
    LaplacianBlending(const cv::Mat& _image,const int _levels, std::vector<Mat> _kernels):
        image(_image),levels(_levels), kernels(_kernels)
        {

            //image.convertTo(image,CV_64F);
            imageLapPyr.resize(levels);
            foveatedPyr.resize(levels);

            buildPyramids();
            image_sizes.resize(levels);
            kernel_sizes.resize(levels);

            for(int i=levels-1; i>=0;--i){

                cv::Mat image_size(2,1,CV_64F); // CV_32S
                
                image_size.at<int>(0,0)=imageLapPyr[i].cols;
                image_size.at<int>(1,0)=imageLapPyr[i].rows;
                image_sizes[i]=image_size;
                
                cv::Mat kernel_size(2,1,CV_64F); // CV_32S
                
                kernel_size.at<int>(0,0)=kernels[i].cols;
                kernel_size.at<int>(1,0)=kernels[i].rows;
                kernel_sizes[i]=kernel_size;
            }
        };


        
        // COMPUTE ROIS
        void computeRois(const cv::Mat & center, cv::Rect & kernel_roi_rect,
                         cv::Mat & kernel_size, const cv::Mat & image_size)
        {

            // Kernel center - image coordinate
            cv::Mat upper_left_kernel_corner=kernel_size/2.0-center;

            //cv::Mat bottom_right_kernel_corner=image_size-center+kernel_size/2.0;
                       

            // encontrar roi no kernel
            // cv::Rect take (upper left corner, width, heigth)
            kernel_roi_rect=cv::Rect(upper_left_kernel_corner.at<int>(0,0),
                                     upper_left_kernel_corner.at<int>(1,0),
                                     image_size.at<int>(0,0),
                                     image_size.at<int>(1,0));

            cout << "Kernel roi rect " << kernel_roi_rect << endl;
            cout << "Image size : " << image_size.at<int>(0,0) << "\t" << image_size.at<int>(1,0) << endl;
            cout << "Kernel size : " << kernel_size.at<int>(0,0) << "\t" << kernel_size.at<int>(1,0) << endl;
        }



        
        // FOVEATE
        cv::Mat foveate(const cv::Mat & center)
        {
            imageSmallestLevel.copyTo(foveated_image);
            cv::Rect kernel_roi_rect;
            
            for(int i=levels-1; i>=0; --i){

                cv::Rect image_roi_rect;
                cv::Rect kernel_roi_rect;
                // rows

                
                // encontrar rois
                
                cv::Mat aux;
                if(i!=0){
                    aux=center/(powf(2,i));
                }
                else{
                    aux=center;
                }

                computeRois(aux,kernel_roi_rect,kernel_sizes[i],image_sizes[i]);
                

                // Multiplicar
                cv::Mat aux_pyr;
                imageLapPyr[i].copyTo(aux_pyr);
                cv::Mat result_roi;

                cout << "size aux_pyr " << aux_pyr.size() << "\n" << endl;
                cout << "size kernel roi " << kernels[i](kernel_roi_rect).size() << "\n" << endl;
                cout << "size result " << result_roi.size() << "\n" << endl;

                cout << "multiplicacao" << endl;

                cout << "Type kernel " << kernels[i](kernel_roi_rect).type() << endl;


                aux_pyr.convertTo(aux_pyr,CV_64F);
                cout << "Type aux pyr " << aux_pyr.type() << endl;

                cv::multiply(aux_pyr,kernels[i](kernel_roi_rect),result_roi,1.0);
                cout << "boa multiplicacao" << endl;



                result_roi.copyTo(aux_pyr);


                cout << "Type foveated image " << foveated_image.type() << endl;
                foveated_image.convertTo(foveated_image,CV_64F);
                cout << "Type foveated image " << foveated_image.type() << endl;


                if(i==(levels-1)) {

                    add(foveated_image,aux_pyr,foveated_image);
                    cout << "tou no if " << endl;
                    cout << "Foveated image size" << foveated_image.size() << "\n" << endl;
                }

                else {
                    cout << "tou no else" << endl;
                    cout << "Foveated image size" << foveated_image.size() << "\n" << endl;

                    // pyrUp( tmp, dst, Size( tmp.cols*2, tmp.rows*2 ) )
                    pyrUp(foveated_image, foveated_image, Size(2*foveated_image.cols,2*foveated_image.rows));

                    cv::add(foveated_image,aux_pyr,foveated_image);
                }

            }
            cout << "FINAL: Foveated image size" << foveated_image.size() << "\n" << endl;

            imshow("Foveated image", foveated_image);
            waitKey(0);

            return foveated_image;
        }
        
        
        
};



