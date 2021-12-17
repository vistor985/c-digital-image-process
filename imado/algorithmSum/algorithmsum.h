#ifndef ALGORITHMSUM_H
#define ALGORITHMSUM_H
#include "algorithmSum_global.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
#include<iostream>
using namespace std;
class AlgorithmSum{
public:
    virtual Mat alterIma(Mat image);
    virtual ~AlgorithmSum();
};

class ImaBlance:public AlgorithmSum{
    Mat alterIma(Mat image);
};

class ImaEnhance:public AlgorithmSum{
    Mat alterIma(Mat image);
};

class ImaLog:public AlgorithmSum{
    Mat alterIma(Mat image);
};

class ImaGamma:public AlgorithmSum{
    Mat alterIma(Mat image);
};

class ImaGray:public AlgorithmSum{
    Mat alterIma(Mat image);
};

#endif // ALGORITHMSUM_H
