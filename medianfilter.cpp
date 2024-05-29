#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
void merge(vector<int> &arr,int low,int mid,int high){
  vector<int> temp;
  int i=low;
  int j=mid+1;

  while(i<=mid && j<=high){
    if(arr[i]<=arr[j]){
        temp.push_back(arr[i]);
        i+=1;
    }
    else{
      temp.push_back(arr[j]);
      j=j+1;
    }

  }
  while(i<=mid){
    temp.push_back(arr[i]);
    i+=1;
  }
  while(j<=high){
    temp.push_back(arr[j]);
    j+=1;
  }
  for(int p=low;p<=high;p++){
    arr[p]=temp[p-low];
  }


}

void mergesort(vector<int> &arr,int low,int high ){
  if(low>=high) return;
  int mid=(low+high)/2;
  mergesort(arr,low,mid);
  mergesort(arr,mid+1,high);
  merge(arr,low,mid,high);
}


int main() {
  cv::Mat image=cv::imread("saltpepr.png",cv::IMREAD_GRAYSCALE);
  if(image.empty()){
    cout<<"image is not there";
  }
  int rows=image.rows;
  int cols=image.cols;
  int channels = image.channels();
  cout<<rows<<endl;
  cout<<cols<<endl;
  cout<<channels<<endl;
  vector<vector<int>> imagearray(rows,vector<int>(cols,0));
  if (channels==1){
    for(int i=0;i<rows;i++){
      for(int j=0;j<cols;j++){
        int pixelvalue=static_cast<int>(image.at<uchar>(i,j));
        imagearray[i][j]=pixelvalue;
        
      }
    }
  }
  else if(channels==3){
    for(int i=0;i<rows;i++){
      for(int j=0;j<cols;j++){
        cv::Vec3b pixel= image.at<cv::Vec3b>(i,j);
        
      }
    }
    
  }
  vector<vector<int>> medianarray(rows,vector<int>(cols,0));
  for(int i=1;i<rows-1;i++){
    for(int j=1;j<cols-1;j++){
      vector<int> arr;
      //cout<<"array";
      for(int p=-1;p<1;p++){
        for(int q=-1;q<1;q++){
            arr.push_back(imagearray[i+p][j+q]);
            
            //cout<<imagearray[i+p][j+q]<<" ";
          
        }
      }
      mergesort(arr,0,8);
      for(int lx=0;lx<=24;lx++){
        //cout<<arr[lx]<<" ";
        
      }
      int median=arr[arr.size()/2];
      medianarray[i][j]=median;
      //cout<<"median"<<median<<endl;
    }
  }
  cv::Mat dst(rows, cols, CV_8UC1);

  for(int p=1;p<rows-1;p++){
    for(int q=1;q<cols-1;q++){
      dst.at<uchar>(p,q)=static_cast<uchar>(medianarray[p][q]);

    }
  }
  cv::imwrite("median2.jpg",dst);
  
  
}
