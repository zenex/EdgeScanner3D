// ╔═╗╦╔═╔═╗╦  ╔═╗╔╦╗╔═╗╔╗╔╔═╗╦
// ╚═╗╠╩╗║╣ ║  ║╣  ║ ║ ║║║║║ ╦║
// ╚═╝╩ ╩╚═╝╩═╝╚═╝ ╩ ╚═╝╝╚╝╚═╝╩═╝
// ─┐ ┬┌─┐┌┐┌┌─┐┌┐ ┬ ┬┌┬┐┌─┐ ─┐ ┬┬ ┬┌─┐
// ┌┴┬┘├┤ ││││ │├┴┐└┬┘ │ ├┤  ┌┴┬┘└┬┘┌─┘
// ┴ └─└─┘┘└┘└─┘└─┘ ┴  ┴ └─┘o┴ └─ ┴ └─┘
// Author:  SENEX @ XENOBYTE.XYZ
// License: MIT License
// Website: https://xenobyte.xyz/projects/?nav=edgeScanner3D

#include "mainWindow.h"
#include "ui_mainWindow.h"
#include <QtCore>
#include <QtWidgets>
#include <QCameraInfo>
#include <QMediaMetaData>
#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{

  QWidget *widget = new QWidget();
  setCentralWidget(widget);

  //NOTE: initialize the UI AFTER setting the central widget
  ui->setupUi(this);
    
  //start the signal mapper
  signalMapper = new QSignalMapper(this);

  //initialize the scanner
  scanner = new Scanner();
  if(scanner->initialize(0, 1280, 800) != 0)
    {
      ui->txtProgramLog->appendPlainText("ERROR: Camera not responding.");
    }

  //the while loop in NOT accesible in QT, to go around this this timer will let us add a new branch
  //from the loop for our stuff
  mainTimer = new QTimer(this);
  connect(mainTimer, SIGNAL(timeout()), this, SLOT(updateScanner()));
  mainTimer->start(20);

  //label setupUi
  label = new QLabel(tr("<i>Testing label</i>"));
  label->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  label->setAlignment(Qt::AlignCenter);

  createActions();
  createMenus();
  createToolbars();
  createStatusBar();

  imageW = 1920;
  imageH = 1080;
  grayScale = false;
  invertBits = false;
  customContrast = false;
  detectEdges = false;
  paused = false;
  scanColors = false;
  treshold = 1;
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(aboutAct);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAct);

  cameraMenu = menuBar()->addMenu(tr("&Camera"));

  //connect mapped signal to menu
  connect(signalMapper, SIGNAL(mapped(const int)), this, SLOT(setCamera(const int)));

  QList<QCameraInfo> cameras = QCameraInfo::availableCameras(); //list all found cameras
  int i = 0;
  ui->txtProgramLog->appendPlainText(QString("Found Cameras: "));

  foreach(const QCameraInfo &cameraInfo, cameras)
    {
      ui->txtProgramLog->appendPlainText(QString(QString(cameraInfo.description()) + QString::number(i)));

      QAction *newCam = new QAction(QString(cameraInfo.description()), this);
      newCam->setStatusTip(tr("CV camera ") + i);

      connect(newCam, SIGNAL(triggered()), signalMapper, SLOT(map()));
      signalMapper->setMapping(newCam, i);
      cameraMenu->addAction(newCam);
      ++i;
    }
}

void MainWindow::createToolbars()
{
  //mainToolbar = addToolBar(tr("File"));
  //mainToolbar->addAction(aboutAct);


  centralTab = new QTabWidget(0);


  //create contrast sliders

  ui->slider_ContrastSliderAlpha->setRange(1,300);
  ui->slider_ContrastSliderAlpha->setSingleStep(1);

    
  ui->slider_ContrastSliderBeta->setRange(1,100);
  ui->slider_ContrastSliderBeta->setSingleStep(1);

  ui->slider_TresholdSlider->setRange(1,100);
  ui->slider_TresholdSlider->setSingleStep(1);
}

void MainWindow::setCamera(const int camera)
{
  ui->txtProgramLog->appendPlainText(QString("Camra chosen: ") + QString::number(camera));
  if(this->scanner->changeCamera(camera) != 0)
    {
      ui->txtProgramLog->appendPlainText(QString("Error opening camera."));
    }
}


void MainWindow::createStatusBar()
{

}

//GUI ELEMENTS

//actions
void MainWindow::createActions()
{
  aboutAct = new QAction(tr("About"), this);
  aboutAct->setStatusTip(tr("Sobre el programa"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  quitAct = new QAction(tr("Quit"), this);
  quitAct->setStatusTip(tr("Exit the application."));
  connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::about()
{
  QMessageBox::about(this, tr("Sobre el Software"), tr("Ver 0.1 3DScanner"));
}


//sliders
void MainWindow::on_slider_ContrastSliderBeta_valueChanged(int value)
{
  contrastBeta = value;
  ui->txtProgramLog->appendPlainText(tr("Beta contrast = ") + QString::number(contrastBeta));
  								  
}

void MainWindow::on_slider_ContrastSliderAlpha_valueChanged(int value)
{
  contrastAlpha = value / 100.0;
  ui->txtProgramLog->appendPlainText(tr("Alpha contrast = ") + QString::number(contrastAlpha));
  								  
}

void MainWindow::on_slider_TresholdSlider_valueChanged(int value)
{
  treshold = value;
  ui->txtProgramLog->appendPlainText(tr("Current treshold = ") + QString::number(treshold));
  								  
}
//push buttons
void MainWindow::on_btn_PauseOrResume_clicked()
{
  if(paused)
    {
      //mainTimer->stop();
      paused = false;
      ui->btn_PauseOrResume->setText("Pause");
      ui->txtProgramLog->appendPlainText(QString("Camera resumed. \n"));
    }
  else
    {
      //mainTimer->start(20);
      paused = true;
      ui->btn_PauseOrResume->setText("Resume");
      ui->txtProgramLog->appendPlainText(QString("Camera paused. \n"));
    }
}

void MainWindow::on_btn_GrayScale_clicked()
{
  if(grayScale)
    {
      grayScale = false;
      ui->btn_GrayScale->setText("Disable Gray Scale");
      ui->txtProgramLog->appendPlainText(QString("Gray scale disabled."));
    }
  else
    {
      grayScale = true;
      ui->btn_GrayScale->setText("Gray Scale");
      ui->txtProgramLog->appendPlainText(QString("Gray scale enabled."));
    }
}


void MainWindow::on_btn_PrintImgData_clicked()
{
  cv::Mat infoImage(scanner->getFrame());
  
  ui->txtProgramLog->appendPlainText(QString("Channels: ") + QString::number(infoImage.channels()));
  cv::Size s = infoImage.size();
  ui->txtProgramLog->appendPlainText(QString("Resolution: ") + QString::number(s.width) + QString(" x ")
				     + QString::number(s.height));
  ui->txtProgramLog->appendPlainText(QString("Type: ") + QString::number(infoImage.type()));
  ui->txtProgramLog->appendPlainText(QString("Image Matrix dimensions: ") + QString::number(infoImage.dims));
  ui->txtProgramLog->appendPlainText(QString("Image Size: ") + QString::number(infoImage.total()));
}

void MainWindow::on_btn_ClearLog_clicked()
{
  ui->txtProgramLog->clear();
}

void MainWindow::on_btn_ChangeContrast_clicked()
{
  if(customContrast)
    {
      customContrast = false;
      ui->btn_ChangeContrast->setText("Disable Custom Contrast");
      ui->txtProgramLog->appendPlainText(QString("Custom Contrast disabled."));      
    }
  else
    {
      customContrast = true;
      ui->btn_ChangeContrast->setText("Change Contrast");
      ui->txtProgramLog->appendPlainText(QString("Custom Contrast enabled."));            
    }
}
  
void MainWindow::on_btn_SaveImage_clicked()
{
  //time as string
  QString dateTime = "images/ss_" + QString::number(countTime->elapsed()/ 1000) + ".jpg";

  cv::Mat infoImage(scanner->getFrame());
  cv::resize(infoImage, infoImage, cv::Size(imageW, imageH), 0, 0, cv::INTER_CUBIC);
  try
    {
      //cv::imwrite("ss.jpg", infoImage);
      cv::imwrite(dateTime.toStdString(), infoImage);      
    }
  catch(...)
    {
      QMessageBox::about(this, tr("Error saving the image"), tr("Verify folder permissions"));
      ui->txtProgramLog->appendPlainText(QString("The image could not be saved."));            	    
    }
}


void MainWindow::on_btn_InvertBits_clicked()
{
  if(invertBits)
    {
      invertBits = false;
      ui->btn_InvertBits->setText("Revert Bits");
      ui->txtProgramLog->appendPlainText(QString("Bits inverted."));      
    }
  else
    {
      invertBits = true;
      ui->btn_InvertBits->setText("Invert bits.");
      ui->txtProgramLog->appendPlainText(QString("Bits reverted."));            
    }
}


void MainWindow::on_btn_CannyEdgeDetection_clicked()
{
  if(detectEdges)
    {
      detectEdges = false;
      ui->btn_CannyEdgeDetection->setText("Stop CED");
      ui->txtProgramLog->appendPlainText(QString("Scanning for edges."));
    }
  else
    {
      detectEdges = true;
      ui->btn_CannyEdgeDetection->setText("Canny Edge Detection");
      ui->txtProgramLog->appendPlainText(QString("Stopped scanning for edges."));
    }
}

void MainWindow::on_btn_ScanImg_clicked()
{
  scanColors = true;
}

//PUBLIC SLOTS

void MainWindow::updateScanner()
{
  //update the webcam
  if(!paused)
    this->scanner->processFrame();
  
  if(grayScale)
    this->scanner->applyGrayScale();
  if(invertBits)
    this->scanner->applyBitInvertion();
  if(customContrast)
    this->scanner->changeContrast(this->contrastBeta, this->contrastAlpha);
  if(detectEdges)
    this->scanner->cannyEdgeDetection(3, treshold, 3);
  if(scanColors)
    {
      this->scanner->scanImage();
      scanColors = false;
    }
  
    
  cv::Mat currentFrame = this->scanner->getFrame();

  QImage frameToImg((uchar *)currentFrame.data, currentFrame.cols, currentFrame.rows, currentFrame.step,
		    (grayScale) ? QImage::Format_Indexed8 : QImage::Format_RGB888);      
      
  //apply the current image to the label
  ui->lbl_MainLabel->setPixmap(QPixmap::fromImage(frameToImg));
    
}
