// ╔═╗╦╔═╔═╗╦  ╔═╗╔╦╗╔═╗╔╗╔╔═╗╦
// ╚═╗╠╩╗║╣ ║  ║╣  ║ ║ ║║║║║ ╦║
// ╚═╝╩ ╩╚═╝╩═╝╚═╝ ╩ ╚═╝╝╚╝╚═╝╩═╝
// ─┐ ┬┌─┐┌┐┌┌─┐┌┐ ┬ ┬┌┬┐┌─┐ ─┐ ┬┬ ┬┌─┐
// ┌┴┬┘├┤ ││││ │├┴┐└┬┘ │ ├┤  ┌┴┬┘└┬┘┌─┘
// ┴ └─└─┘┘└┘└─┘└─┘ ┴  ┴ └─┘o┴ └─ ┴ └─┘
// Author:  SENEX @ XENOBYTE.XYZ
// License: MIT License
// Website: https://xenobyte.xyz/projects/?nav=edgeScanner3D

#ifndef SCANNER_H
#define SCANNER_H

//opencv libs
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//C++ STDs
#include <iostream>

class Scanner
{
  private:
  //webcam loader
  cv::VideoCapture webcam;
  //images
  cv::Mat originalFrame;
  cv::Mat backupFrame;

  //options
  bool grayScale, customContrast, invertBits, edgeDetection;

  //contrast settings
  int contrastBeta;
  double contrastAlpha;

public:
  Scanner();
  ~Scanner();

  //initialize webcam
  int initialize(int camera, int w, int h);
  int changeCamera(int newCamera);
  //update webcam 
  int processFrame();
  //modifications
  void changeContrast(int beta, double alpha);
  void applyGrayScale();
  void applyBitInvertion();
  void cannyEdgeDetection(int ratio, int treshold, int kernel);
  void scanImage();
  //return images
  cv::Mat getFrame();
  cv::Mat getBackupFrame();
  
  
};

#endif //SCANNER_H
