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
        
        for (int l=0; l<levels; l++)
        {
            Mat image;
            pyrDown(currentImg, down);
            pyrUp(down, up, currentImg.size());
            
            Mat lap = currentImg - up;
            
            imageLapPyr[l]=lap;
            
            currentImg = down;
            cout << "Level " << l << endl;
        }
        
        imageSmallestLevel=up;
        
    }




public:
    LaplacianBlending(const cv::Mat& _image,const int _levels, std::vector<Mat> _kernels):
        image(_image),levels(_levels), kernels(_kernels)
        {
            image.convertTo(image,CV_64F);
            imageLapPyr.resize(levels);
            foveatedPyr.resize(levels);
            
            buildPyramids();
            image_sizes.resize(levels);
            kernel_sizes.resize(levels);
            for(int i=levels-1; i>=0;--i){

                cv::Mat image_size(2,1,CV_32S);
                
                image_size.at<int>(0,0)=imageLapPyr[i].cols;
                image_size.at<int>(1,0)=imageLapPyr[i].rows;
                image_sizes[i]=image_size;
                
                cv::Mat kernel_size(2,1,CV_32S);
                
                kernel_size.at<int>(0,0)=kernels[i].cols;
                kernel_size.at<int>(1,0)=kernels[i].rows;
                kernel_sizes[i]=kernel_size;
            }
        };
        
        // COMPUTE ROIS
        void computeRois(const cv::Mat & center,
                cv::Rect & kernel_roi_rect,
                cv::Mat & kernel_size,
                const cv::Mat & image_size)
        {
            // Kernel center - image coordinate
            cv::Mat upper_left_kernel_corner=kernel_size/2.0-center;;
            cv::Mat bottom_right_kernel_corner=image_size-center+kernel_size/2.0;
            
            // encontrar roi no kernel
            kernel_roi_rect=cv::Rect(
                    upper_left_kernel_corner.at<int>(0,0),
                    upper_left_kernel_corner.at<int>(1,0),
                    image_size.at<int>(0,0),
                    image_size.at<int>(1,0));
        }
        
        // FOVEATE
        cv::Mat foveate(const cv::Mat & center)
        {
            imageSmallestLevel.copyTo(foveated_image);
            cv::Rect kernel_roi_rect;
            
            for(int i=levels-1; i>=0;--i)
            {
                cv::Rect image_roi_rect;
                cv::Rect kernel_roi_rect;
                // rows

                
                // encontrar rois
                
                cv::Mat aux;
                if(i!=0)
                {
                    aux=center/(powf(2,i));
                }
                else
                {
                    aux=center;
                }
                computeRois(aux,kernel_roi_rect,kernel_sizes[i],image_sizes[i]);
                
                // Multiplicar
                cv::Mat aux_pyr;
                imageLapPyr[i].copyTo(aux_pyr);
                cv::Mat result_roi;
                cv::multiply(aux_pyr,kernels[i](kernel_roi_rect),result_roi,1.0);
                result_roi.copyTo(aux_pyr);
                if(i==(levels-1))
                {
                    add(foveated_image,aux_pyr,foveated_image);
                }
                
                else
                {
                    
                    pyrUp(foveated_image,foveated_image,cv::Size(2*foveated_image.cols,2*foveated_image.rows));
                    //resize(foveated_image, foveated_image, cv::Size(2*foveated_image.cols,2*foveated_image.rows), 0, 0, 0);
                    cv::add(foveated_image,aux_pyr,foveated_image);
                }
            }

            
            return foveated_image;
        }
        
        
        
};



