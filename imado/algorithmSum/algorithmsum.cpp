#include "algorithmsum.h"


Mat AlgorithmSum::alterIma(Mat image){
    return image;
}

Mat ImaBlance::alterIma(Mat image){
    //直方图均衡化是通过调整图像的灰阶分布，使得在0~255灰阶上的分布更加均衡，提高了图像的对比度，达到改善图像主观视觉效果的目的。对比度较低的图像适合使用直方图均衡化方法来增强图像细节。
    Mat imageBalance = image;
    Mat imageRGB[3];
    split(imageBalance, imageRGB);
    for (int i = 0; i < 3; i++)
    {
        equalizeHist(imageRGB[i], imageRGB[i]);
    }
    merge(imageRGB, 3, imageBalance);
    return imageBalance;
}

Mat ImaEnhance::alterIma(Mat image)
{
    //拉普拉斯算子可以增强局部的图像对比度：
    Mat imageEnhance=image;
    Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    filter2D(image, imageEnhance, CV_8UC3, kernel);
    return imageEnhance;
}

Mat ImaLog::alterIma(Mat image)
{
    //对数变换对于整体对比度偏低并且灰度值偏低的图像增强效果较好。
    Mat imageLog(image.size(), CV_32FC3);
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            imageLog.at<Vec3f>(i, j)[0] = log(1 + image.at<Vec3b>(i, j)[0]);
            imageLog.at<Vec3f>(i, j)[1] = log(1 + image.at<Vec3b>(i, j)[1]);
            imageLog.at<Vec3f>(i, j)[2] = log(1 + image.at<Vec3b>(i, j)[2]);
        }
    }
    //归一化到0~255
    normalize(imageLog, imageLog, 0, 255, NORM_MINMAX);
    //转换成8bit图像显示
    convertScaleAbs(imageLog, imageLog);
    return imageLog;
}

Mat ImaGamma::alterIma(Mat image)
{
    Mat imageGamma(image.size(), CV_32FC3);
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            imageGamma.at<Vec3f>(i, j)[0] = (image.at<Vec3b>(i, j)[0])*(image.at<Vec3b>(i, j)[0])*(image.at<Vec3b>(i, j)[0]);
            imageGamma.at<Vec3f>(i, j)[1] = (image.at<Vec3b>(i, j)[1])*(image.at<Vec3b>(i, j)[1])*(image.at<Vec3b>(i, j)[1]);
            imageGamma.at<Vec3f>(i, j)[2] = (image.at<Vec3b>(i, j)[2])*(image.at<Vec3b>(i, j)[2])*(image.at<Vec3b>(i, j)[2]);
        }
    }
    //归一化到0~255
    normalize(imageGamma, imageGamma, 0, 255, NORM_MINMAX);
    //转换成8bit图像显示
    convertScaleAbs(imageGamma, imageGamma);
    return imageGamma;
}

Mat ImaGray::alterIma(Mat image)
{
    Mat gray;
    cvtColor(image,gray,COLOR_BGR2GRAY);
    return gray;
}
