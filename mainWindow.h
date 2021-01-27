// ╔═╗╦╔═╔═╗╦  ╔═╗╔╦╗╔═╗╔╗╔╔═╗╦
// ╚═╗╠╩╗║╣ ║  ║╣  ║ ║ ║║║║║ ╦║
// ╚═╝╩ ╩╚═╝╩═╝╚═╝ ╩ ╚═╝╝╚╝╚═╝╩═╝
// ─┐ ┬┌─┐┌┐┌┌─┐┌┐ ┬ ┬┌┬┐┌─┐ ─┐ ┬┬ ┬┌─┐
// ┌┴┬┘├┤ ││││ │├┴┐└┬┘ │ ├┤  ┌┴┬┘└┬┘┌─┘
// ┴ └─└─┘┘└┘└─┘└─┘ ┴  ┴ └─┘o┴ └─ ┴ └─┘
// Author:  SENEX @ XENOBYTE.XYZ
// License: MIT License
// Website: https://xenobyte.xyz/projects/?nav=edgeScanner3D

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//qt libs
#include <QMainWindow>
#include <QElapsedTimer>
#include <QCameraInfo>
#include <QMediaMetaData>
#include <QSignalMapper>
//opencv libs
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//standard C++ libs
#include <iostream>
#include <vector>
#include <sstream>
#include <ctime>
#include <iomanip>

#include "scanner.hpp"

//forward declarations
class QAction;
class QMenu;
class QToolBar;
class QMessageBox;
class QLabel;
class QTabWidget;
//ui loading
namespace Ui {
  class MainWindow;
}


class MainWindow : public QMainWindow
{
  Q_OBJECT    

  private slots:
    void setCamera(const int camera);
    void about();


    //push buttons
    void on_btn_PauseOrResume_clicked();
    void on_btn_GrayScale_clicked();
    void on_btn_ChangeContrast_clicked();
    void on_btn_PrintImgData_clicked();
    void on_btn_ClearLog_clicked();
    void on_btn_SaveImage_clicked();
    void on_btn_InvertBits_clicked();
    void on_btn_CannyEdgeDetection_clicked();
    void on_btn_ScanImg_clicked();
    //sliders
    void on_slider_ContrastSliderBeta_valueChanged(int value);
    void on_slider_ContrastSliderAlpha_valueChanged(int value);
    void on_slider_TresholdSlider_valueChanged(int value);
 
    public slots:
      void updateScanner();
      
 private:
      //ui
      Ui::MainWindow *ui;
      //This are all overloaded funcs
      void createActions();
      void createMenus();
      void createToolbars();
      void createStatusBar();

      //GUI
      QTimer *mainTimer; //sets timer to refresh the window
      QElapsedTimer *countTime; //counts time
      //static, this are not declared / defined in the UI file
      QMenu *fileMenu;
      QMenu *cameraMenu;
      QAction *aboutAct, *quitAct;
      QLabel *label;
      QSignalMapper *signalMapper; //sends custom signals	
      QTabWidget *centralTab; //tab menu testing
      //image to display
      QImage originalImg;
      //cv settings
      double contrastAlpha;
      int contrastBeta, treshold;
      int camera;

      //Scanner
      Scanner *scanner;
      bool grayScale, customContrast, invertBits, detectEdges, paused, scanColors;
      int imageW, imageH;


 public:
      explicit MainWindow(QWidget *parent = 0);
      ~MainWindow();

      //scanner wrappers
    

};

#endif // MAINWINDOW_H
