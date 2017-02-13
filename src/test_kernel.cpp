#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>
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

using namespace cv;
using namespace std;
using std::string;


double gaussian (double x, double mu, double sigma) {
  return std::exp( -(((x-mu)/(sigma))*((x-mu)/(sigma)))/2.0 );
}

typedef std::vector<double> kernel_row;
typedef std::vector<kernel_row> kernel_type;


// FUNCTION
kernel_type produce2dGaussianKernel (int kernelRadius, int m, int n) {
  double sigma = kernelRadius/2.;
  kernel_type kernel2d(2*kernelRadius+1, kernel_row(2*kernelRadius+1));
  double sum = 0;
  // compute values
  for (int row = 0; row < kernel2d.size(); row++)
    for (int col = 0; col < kernel2d[row].size(); col++) {
      double x = gaussian(row, kernelRadius, sigma)
               * gaussian(col, kernelRadius, sigma);
      kernel2d[row][col] = x;
      sum += x;
    }
  // normalize
  for (int row = 0; row < kernel2d.size(); row++)
    for (int col = 0; col < kernel2d[row].size(); col++)
      kernel2d[row][col] /= sum;
  return kernel2d;
}



// MAIN
int main(int argc, char** argv) {
  
  int levels = 5;

  string file = string(argv[1]) + "quarto.jpg";

  Mat image = imread(file, -1);

  for (int l=0; l<levels; ++l){ // for each level

      int m = image.size().height/(powf(2, l));
      int n = image.size().width/(powf(2, l));


      kernel_type kernel2d = produce2dGaussianKernel(10,m ,n);

      std::cout << std::setprecision(5) << std::fixed;

      for (int row = 0; row < kernel2d.size(); row++) {
        for (int col = 0; col < kernel2d[row].size(); col++){

          std::cout << kernel2d[row][col] << ' ';}
        std::cout << '\n';

      }



  }

}
