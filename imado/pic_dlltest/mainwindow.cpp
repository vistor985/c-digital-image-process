#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}


MainWindow::~MainWindow()
{
    delete ui;
    delete algoBlance;
    delete algoEnhance;
    delete algoLog;
    delete algoGamma;
    delete algoGray;

}



QImage MainWindow::mat2qim(Mat  mat)
{
    cvtColor(mat, mat, COLOR_BGR2RGB);
    QImage qim((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step,
               QImage::Format_RGB888);
    return qim;
}

Mat MainWindow::qim2mat(QImage  qim)
{

    Mat mat = Mat(qim.height(), qim.width(),
                  CV_8UC3,(void*)qim.constBits(),qim.bytesPerLine());
    return mat;


}
//
QImage MainWindow:: cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        //        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        //        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
cv::Mat MainWindow:: QImage2cvMat(QImage image)
{
    cv::Mat mat;
    //    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, COLOR_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    default:
        break;
    }
    return mat;
}

void MainWindow::showPicLabel(QImage pic, QLabel *label)
{
    QImage* img=new QImage,* scaledimg=new QImage;//分别保存原图和缩放之后的图片
    *img = pic;
    double Owidth=img->width(),Oheight=img->height();
    double Fwidth,Fheight;       //缩放后的图片大小
    //    ui->label->setGeometry(0,0,400,300);
    double wid = label->width();//得到label的宽高
    double hei = label->height();
    double Mul;            //记录图片与label大小的比例，用于缩放图片
    if(Owidth/wid>=Oheight/hei)
        Mul=Owidth/wid;
    else
        Mul=Oheight/hei;
    Fwidth=Owidth/Mul;
    Fheight=Oheight/Mul;
    *scaledimg=img->scaled(Fwidth,Fheight,Qt::KeepAspectRatio);
    //label->setScaledContents(true);
    //    label->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    label->setPixmap(QPixmap::fromImage(*scaledimg));
}


void MainWindow::on_OpenImg_clicked()
{

    QString OpenFile, OpenFilePath;
    QImage image;
    //打开文件夹中的图片文件
    OpenFile = QFileDialog::getOpenFileName(this,
                                            "Please choose an image file",
                                            "",
                                            "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
    if( OpenFile != "" )
    {
        if( image.load(OpenFile) )
        {
            showPicLabel(image,ui->label);
        }
    }

    //        显示所示图片的路径

    QFileInfo OpenFileInfo;
    OpenFileInfo = QFileInfo(OpenFile);
    OpenFilePath = OpenFileInfo.filePath();
    ui->lineEdit->setText(OpenFilePath);
    picPath = OpenFilePath;
    Mat imageCount = imread(picPath.toStdString(),1);
    matList.push_back(imageCount);

}


void MainWindow::on_custom_clicked()
{
    if(matList.size()<1){
        return;
    }
    Mat imageCount = imread(picPath.toStdString(),1);//用于记录
    QImage image;
    image.load(picPath);
    showPicLabel(image,ui->label_3);
    matList.push_back(imageCount);//记录
    //    on_label_linkActivated("C:/darkpic.jpg");

}

void MainWindow::on_testButton_clicked()
{
    //    Mat image = imread(picPath.toStdString(),1);
    //直方图均衡化是通过调整图像的灰阶分布，使得在0~255灰阶上的分布更加均衡，提高了图像的对比度，达到改善图像主观视觉效果的目的。对比度较低的图像适合使用直方图均衡化方法来增强图像细节。
    if(matList.size()<1){
        return;
    }
    Mat image = matList.back();
    if(image.type()==0){//不对灰度图进行处理
        return;
    }
    Mat imageBalance = algoBlance->alterIma(image);
    showPicLabel(mat2qim(imageBalance),ui->label_3);
    matList.push_back(imageBalance);//记录
}

void MainWindow::on_Enhance_clicked()
{
    //拉普拉斯算子可以增强局部的图像对比度：
    //    Mat image = imread(picPath.toStdString(),1);
    if(matList.size()<1){
        return;
    }
    Mat image = matList.back();
    if(image.type()==0){//不对灰度图进行处理
        return;
    }
    Mat imageEnhance=algoEnhance->alterIma(image);
    //    Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    //    filter2D(image, imageEnhance, CV_8UC3, kernel);
    showPicLabel(mat2qim(imageEnhance),ui->label_3);
    matList.push_back(imageEnhance);//记录

}


void MainWindow::on_imageLog_clicked()
{
    //对数变换对于整体对比度偏低并且灰度值偏低的图像增强效果较好。
    //    Mat image = imread(picPath.toStdString(),1);
    if(matList.size()<1){
        return;
    }
    Mat image = matList.back();
    if(image.type()==0){//不对灰度图进行处理
        return;
    }
    Mat imageLog = algoLog->alterIma(image);
    showPicLabel(mat2qim(imageLog),ui->label_3);
    matList.push_back(imageLog);//记录
}


void MainWindow::on_gamma_clicked()
{
    //伽马变换对于图像对比度偏低，并且整体亮度值偏高（对于于相机过曝）情况下的图像增强效果明显。
    if(matList.size()<1){
        return;
    }
    Mat image = matList.back();
    if(image.type()==0){//不对灰度图进行处理
        return;
    }
    Mat imageGamma = algoGamma->alterIma(image);
    showPicLabel(mat2qim(imageGamma),ui->label_3);
    matList.push_back(imageGamma);//记录
}


void MainWindow::on_gray_clicked()
{
    if(matList.size()<1){
        return;
    }
    Mat image = matList.back();
    if(image.type()==0){//不对灰度图进行处理
        return;
    }
    Mat gray = algoGray->alterIma(image);
    showPicLabel(mat2qim(gray),ui->label_3);
    //记录
    matList.push_back(gray);
}

void MainWindow::on_convertHist_clicked()
{
    Mat src;
    if(matList.size()<1){
        return;
    }
    // 1. 加载源图像
    src=matList.back();
    if(src.type()==0){//不对灰度图进行处理
        return;
    }
    imshow("input Image",src);

    // 2. 在R、G、B平面中分离源图像，把多通道图像分为多个单通道图像。使用OpenCV函数cv::split。
    vector<Mat> bgr_planes;
    split(src, bgr_planes);// 把多通道图像分为多个单通道图像

    printf("channels=%d\n", bgr_planes.size());//3通道，所以size也是3
    imshow("channels_b", bgr_planes[0]);
    imshow("channels_g", bgr_planes[1]);
    imshow("channels_r", bgr_planes[2]);

    // 3. 现在我们准备开始为每个平面配置直方图。 由于我们正在使用B，G和R平面，我们知道我们的值将在区间[0,255]范围内
    int histBins = 256;//建立箱数（5,10 ......）
    float range[] = { 0, 256 };//设置值的范围（在0到255之间）
    const float * histRanges = range;//注意：函数形参 float ** 与 const float ** 是两种不同数据类型。
    bool uniform = true, accumulate = false;//我们希望我们的箱子具有相同的尺寸（均匀）并在开头清除直方图
    Mat b_hist, g_hist, r_hist;//calcHist计算出来的Mat中元素的最大值可能上几千，所以最好归一化后再绘制直方图
    //使用OpenCV函数cv::calcHist计算直方图：
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histBins, &histRanges, uniform, accumulate);//计算直方图
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histBins, &histRanges, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histBins, &histRanges, uniform, accumulate);

    // 4. 归一化
    int hist_cols = 400;
    int hist_rows = 512;
    int bin_w = hist_rows / histBins;
    /*
            normalize(  // normalize函数作用为 归一化数据
                InputArray src, // 输入数组
                InputOutputArray dst, // 输出数组，数组的大小和原数组一致
                double alpha = 1, // 1,用来规范值，2.规范范围，并且是下限
                double beta = 0, // 只用来规范范围并且是上限
                int norm_type = NORM_L2, // 归一化选择的数学公式类型
                int dtype = -1, // 当为负，输出在大小深度通道数都等于输入，当为正，输出只在深度与输如不同，不同的地方由dtype决定
                InputArray mask = noArray() // 掩码。选择感兴趣区域，选定后只能对该区域进行操作
            );

            值归一化举例说明:   参考博客 https://blog.csdn.net/cosmispower/article/details/64457406
                src={10,23,71}，参数 beta 设为0，值的范围为 0-参数alpha 设置的值，例子中alpha设置为1
                NORM_L1运算后得到   dst={0.096,0.221,0.683}       NORM_* 的归一化公式参考上述博客
                NORM_INF运算后得到  dst={0.141,0.324,1}
                NORM_L2运算后得到   dst={0.133,0.307,0.947}
                NORM_MINMAX运算得到 dst={0,0.377,1}             P = Ak / (max(Ai)-min(Ai))  Ak等于最大最小Ai时，不按此公式计算，P直接等于1, 0

            范围归一化时，beta必有值不等于0，范围为 alpha-beta ，alpha为下限(可为0也可非0)，beta为上限
        */
    //请注意，在绘制之前，我们首先对直方图进行cv :: normalize，使其值落在输入参数指示的范围内：
    normalize(b_hist, b_hist, 0, hist_cols, NORM_MINMAX, -1, Mat());//b_hist中元素的值转换到 0-hist_cols 之间
    normalize(g_hist, g_hist, 0, hist_cols, NORM_MINMAX, -1, Mat());
    normalize(r_hist, r_hist, 0, hist_cols, NORM_MINMAX, -1, Mat());//传参 0, hist_cols 或 hist_cols, 0 结果一致

    // 5. 绘制直方图
    Mat histImage(hist_rows, hist_cols, CV_8UC3, Scalar(0, 0, 0));
    for (int i = 1; i < histBins; i++)
    {
        // cvRound 四舍五入，返回整型值
        line(histImage, Point((i - 1)*bin_w, hist_cols - cvRound(b_hist.at<float>(i - 1))),
             Point(i*bin_w, hist_cols - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, LINE_AA);
        line(histImage, Point((i - 1)*bin_w, hist_cols - cvRound(g_hist.at<float>(i - 1))),
             Point(i*bin_w, hist_cols - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, LINE_AA);
        line(histImage, Point((i - 1)*bin_w, hist_cols - cvRound(r_hist.at<float>(i - 1))),
             Point(i*bin_w, hist_cols - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, LINE_AA);
    }
    // 6. 最后，我们显示直方图并等待用户退出：
    imshow("histImage", histImage);

}

void MainWindow::on_listBack_clicked()
{
    if(matList.size()<=1){
        return;
    }
    matforward.push_back(matList.back());
    matList.pop_back();
    showPicLabel(mat2qim(matList.back()),ui->label_3);
}


void MainWindow::on_listForward_clicked()
{
    if(matforward.size()<1){
        return;
    }
    matList.push_back(matforward.back());
    matforward.pop_back();
    showPicLabel(mat2qim(matList.back()),ui->label_3);
}

void MainWindow::on_save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
                this, "保存图片",
                ".",
                "图片 (*.jpg *.bmp *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;所有文件(*.*)");
    if(!fileName.isEmpty()&&matList.size()>0)
    {
        cv::imwrite(fileName.toStdString(),matList.back());
    }
    else
    {
        return;
    }
}

