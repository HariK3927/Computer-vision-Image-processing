#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
int nonmax(vector<vector<int>> &arr,int i,int j){
  int max=0;
  for(int p=i-1;p<i+1;p++){
    for(int q=j-1;q<j+1;q++){
       if(arr[p][q]>max){
         max=arr[p][q];
       }

    }
  } 
  return max;
}
int main() {
  // Load an image using OpenCV
  cv::Mat image = cv::imread("lena.png", cv::IMREAD_GRAYSCALE);

  // Check if the image is loaded successfully
  if (image.empty()) {
    std::cerr << "Failed to load image" << std::endl;
    return 1;
  }
  
  // Get dimensions of the image
  int rows = image.rows;
  int cols = image.cols;
  int channels = image.channels();
  cout << rows;
  cout << cols;
  cout << channels;
  std::vector<std::vector<int>> imagearray(rows, std::vector<int>(cols));
  // std::vector<std::vector<std::vector<std::vector<int>>>>
  // imagearray(channels, std::vector<std::vector<std::vector<int>>>(rows,
  // std::vector<std::vector<int>>(cols)));

  // Check if the image is grayscale or color
  if (channels == 1) {
    // Grayscale image
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        int pixel_value = static_cast<int>(image.at<uchar>(i, j));
        // std::cout << "Pixel value at (" << i << ", " << j << "): " <<
        // pixel_value << std::endl;
        imagearray[i][j] = pixel_value;
      }
    }
  } else if (channels == 3) {
    // Color image
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
        int blue = static_cast<int>(pixel[0]);
        int green = static_cast<int>(pixel[1]);
        int red = static_cast<int>(pixel[2]);
        // imagearray[i][j][0]=red;
        // imagearray[i][j][1]=green;
        // imagearray[i][j][2]=blue;
        // std::cout << "Pixel value at (" << i << ", " << j << "): (" << blue
        // << ", " << green << ", " << red << ")" << std::endl;
      }
    }
  } else {
    std::cerr << "Unsupported number of channels: " << channels << std::endl;
    return 1;
  }
  // Gaussian kernel
  vector<vector<float>> gaussian = {{1.0f / 16, 2.0f / 16, 1.0f / 16},
                                    {2.0f / 16, 4.0f / 16, 2.0f / 16},
                                    {1.0f / 16, 2.0f / 16, 1.0f / 16}};

  // Apply Gaussian blur

  vector<vector<int>> smoothimage(rows, vector<int>(cols, 0));
  for (int i = 1; i < rows - 1; ++i) {
    for (int j = 1; j < cols - 1; ++j) {
      float sum = 0.0;
      for (int p = 0; p < 3; ++p) {
        for (int q = 0; q < 3; ++q) {
          sum += imagearray[i + p - 1][j + q - 1] * gaussian[p][q];
        }
      }
      smoothimage[i][j] = static_cast<int>(sum);
    }
  }
  int ixmax = -1000;
  int ixmin = 1000;
  vector<vector<int>> sobel = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
  vector<vector<int>> sobely = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

  vector<vector<int>> Ix(rows, vector<int>(cols, 0));
  for (int i = 1; i < rows - 1; i++) {
    for (int j = 1; j < cols - 1; j++) {
      for (int p = -1; p < 2; p++) {
        for (int q = -1; q < 2; q++) {
          Ix[i][j] += smoothimage[i + p][j + q] * sobel[p + 1][q + 1];
        }
      }
      /*if(Ix[i][j]>ixmax){
        ixmax=Ix[i][j];
      }
      if(Ix[i][j]<ixmin){
        ixmin=Ix[i][j];

      }*/
    }
  }
  vector<vector<int>> Iy(rows, vector<int>(cols, 0));
  for (int i = 1; i < rows - 1; i++) {
    for (int j = 1; j < cols - 1; j++) {
      for (int p = -1; p < 2; p++) {
        for (int q = -1; q < 2; q++) {
          Iy[i][j] += smoothimage[i + p][j + q] * sobely[p + 1][q + 1];
        }
      }
    }
  }
  vector<vector<int>> Mag(rows, vector<int>(cols, 0));
  vector<vector<int>> Ori(rows,vector<int>(cols,0));
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      Mag[i][j] = sqrt(Ix[i][j] * Ix[i][j] + Iy[i][j] * Iy[i][j]);
      if (Ix[i][j] == 0) {
        Ori[i][j] = (Iy[i][j] > 0) ? CV_PI / 2 : -CV_PI / 2; // Handle vertical gradient
      } else {
        Ori[i][j] = atan2(Iy[i][j], Ix[i][j]); // Use atan2 to handle all quadrants and zero checks
      }
      if(Mag[i][j]>ixmax){
        ixmax=Mag[i][j];
      }
      if(Mag[i][j]<ixmin){
        ixmin=Mag[i][j];

      }
      
    }
  }
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
        Mag[i][j]=(Mag[i][j]-ixmin)*255/(ixmax-ixmin);
    }
  }
  //Non max compression
  vector<vector<int>> canny(rows,vector<int>(cols,0));
  for(int i=1;i<rows-1;i++){
    for(int j=1;j<cols-1;j++){
         canny[i][j]=nonmax(Mag,i,j);
         if(canny[i][j]<0.1*255){
           canny[i][j]=0;
         }
           
      
    }
  }
  
  
  cout << ixmax;
  cout << ixmin;
  // Convert the vector array to a cv::Mat object
  cv::Mat imageop(imagearray.size(), imagearray[0].size(), CV_8UC1);
  for (int i = 0; i < image.rows; ++i) {
    for (int j = 0; j < image.cols; ++j) {
      imageop.at<uchar>(i, j) = static_cast<uchar>(canny[i][j]);
    }
  }
  

  // Save the image
  cv::imwrite("output_image.png", imageop);
  return 0;
}
