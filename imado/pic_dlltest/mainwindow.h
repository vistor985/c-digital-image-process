#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include "algorithmsum.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QImage>
#include<QMessageBox>
#include<QBuffer>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
#include<iostream>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    AlgorithmSum *algo;
    AlgorithmSum *algoBlance = new ImaBlance();
    AlgorithmSum *algoEnhance = new ImaEnhance();
    AlgorithmSum *algoLog = new ImaLog();
    AlgorithmSum *algoGamma = new ImaGamma();
    AlgorithmSum *algoGray = new ImaGray();
    list<Mat> matList;//记录图片的播放
    list<Mat> matforward;
    QString picPath;

    QImage mat2qim(Mat  mat);
    Mat qim2mat(QImage  qim);
    QImage cvMat2QImage(const cv::Mat& mat);
    Mat QImage2cvMat(QImage image);
    void showPicLabel(QImage pic,QLabel* label);
private slots:
    void on_testButton_clicked();

    void on_custom_clicked();

    void on_OpenImg_clicked();

    void on_Enhance_clicked();

    void on_imageLog_clicked();

    void on_gamma_clicked();

    void on_gray_clicked();

    void on_listBack_clicked();

    void on_listForward_clicked();

    void on_convertHist_clicked();

    void on_save_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
