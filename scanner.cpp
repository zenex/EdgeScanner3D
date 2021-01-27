// ╔═╗╦╔═╔═╗╦  ╔═╗╔╦╗╔═╗╔╗╔╔═╗╦
// ╚═╗╠╩╗║╣ ║  ║╣  ║ ║ ║║║║║ ╦║
// ╚═╝╩ ╩╚═╝╩═╝╚═╝ ╩ ╚═╝╝╚╝╚═╝╩═╝
// ─┐ ┬┌─┐┌┐┌┌─┐┌┐ ┬ ┬┌┬┐┌─┐ ─┐ ┬┬ ┬┌─┐
// ┌┴┬┘├┤ ││││ │├┴┐└┬┘ │ ├┤  ┌┴┬┘└┬┘┌─┘
// ┴ └─└─┘┘└┘└─┘└─┘ ┴  ┴ └─┘o┴ └─ ┴ └─┘
// Author:  SENEX @ XENOBYTE.XYZ
// License: MIT License
// Website: https://xenobyte.xyz/projects/?nav=edgeScanner3D

#include "scanner.hpp"
#include <fstream>
#include <sstream>

Scanner::Scanner() : grayScale(false), customContrast(false), invertBits(false),
		     contrastBeta(0), contrastAlpha(0.0f)
{

}

Scanner::~Scanner()
{

}

int Scanner::initialize(int camera, int w, int h)
{
  //video setup
  webcam.set(cv::CAP_PROP_FRAME_WIDTH, w);
  webcam.set(cv::CAP_PROP_FRAME_HEIGHT, h);

  webcam.open(camera, cv::CAP_V4L2);

  if(webcam.isOpened() == false)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

//update webcam
int Scanner::processFrame()
{
  webcam.read(originalFrame);
  //webcam >> originalFrame;
  if(originalFrame.empty() == true)
    {
      //std::cout << "FRAME NOT UPDATED" << std::endl;
      return 1;
    }
  return 0;
}

void Scanner::changeContrast(int beta, double alpha)
{
  this->contrastBeta = beta;
  this->contrastAlpha = alpha;
  for(int y = 0; y != originalFrame.rows; ++y)
    {
      for(int x = 0; x != originalFrame.cols; ++x)
	{
	  for(int c = 0; c < 3; ++c)
	    {
	      originalFrame.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(contrastAlpha * (originalFrame.at<cv::Vec3b>(y,x)[c]) + contrastBeta);
	    }
	}
    }
}

void Scanner::applyGrayScale()
{
  cv::Mat gray;
  cvtColor(originalFrame, gray, cv::COLOR_BGR2GRAY);
  originalFrame = gray;
}

void Scanner::applyBitInvertion()
{
  cv::bitwise_not(originalFrame, originalFrame);

}

void Scanner::cannyEdgeDetection(int ratio, int treshold, int kernel)
{
  cv::Mat detectedEdges, grayFrame;
  detectedEdges.create(originalFrame.size(), originalFrame.type());
  grayFrame = originalFrame;
  cv::cvtColor(originalFrame, grayFrame, cv::COLOR_BGR2GRAY);

  //reduce noise
  cv::blur(grayFrame, detectedEdges, cv::Size(3,3));

  //openCV canny detection
  cv::Canny(detectedEdges, detectedEdges, treshold, treshold * ratio, kernel);
  //use cannys output as a mask
  cv::Mat dst;
  dst.create(originalFrame.size(), originalFrame.type());

  dst = cv::Scalar::all(0);
  originalFrame.copyTo(dst, detectedEdges);
  originalFrame = dst;
}

//Scans the current frame for vertices
//will run it in its own thread, but thats a problem for future me
void Scanner::scanImage()
{
  std::ofstream file;
  file.open("vertexData.xyz");
  //file << "VERTEX DATA\n";

  for(int y = 0; y != originalFrame.rows; ++y)
    {
      for(int x = 0; x != originalFrame.cols; ++x)
	{
	  if(originalFrame.at<cv::Vec3b>(y,x)[0] != 0 && originalFrame.at<cv::Vec3b>(y,x)[1] != 0 && originalFrame.at<cv::Vec3b>(y,x)[2] != 0)
	    {
          file << x;
          file << " ";
          file << -y;
          file << " ";
          file << 0;
          file << "\n";
          //color data
          //file << std::to_string(originalFrame.at<cv::Vec3b>(y,x)[0]);
          //file << " ";
          //file << std::to_string(originalFrame.at<cv::Vec3b>(y,x)[1]);
          //file << " ";
          //file << std::to_string(originalFrame.at<cv::Vec3b>(y,x)[2]);
          //file << "\n";
	    }
	}
    }
  file.close();
}

//change the camera
int Scanner::changeCamera(int newCamera)
{
  webcam.open(newCamera, cv::CAP_V4L2);
  if(webcam.isOpened() == false)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

//return images
cv::Mat Scanner::getFrame()
{
  return originalFrame;
}
cv::Mat Scanner::getBackupFrame()
{
  return backupFrame;
}

