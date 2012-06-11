#include <math.h>
#include <QtGui>
#include "imageviewer.h"

#if 0
void ImageViewer::rotation(cv::Mat &image, double angle)
{
    cv::Point2f src_center(image.cols/2.0f, image.rows/2.0f);
    cv::Mat rot_matrix = getRotationMatrix2D(src_center, angle, 1.0);
    cv::Mat rotated_img(cv::Size(image.size().height, image.size().width), image.type());

    warpAffine(image, rotated_img, rot_matrix, image.size());
    //imwrite("rotated.jpg", rotated_img);
    img= QImage((const unsigned char *)(rotated_img.data),  // Qt image structure
                rotated_img.cols,rotated_img.rows,QImage::Format_RGB888);
}
#endif

#if !defined randomAngle         // ����ת�㷨ÿ��˳ʱ����ת90��͵��û���������Ƕ���ת���㷨
                                 // ��ҿ����ڴ��㷨���иĽ���ʵ������Ƕ���ת
void ImageViewer::imageRotationClick()
{
    // cv::Mat dst;
    //cv::transpose(dst, dst);
    cv::transpose(image, image);
    cv::flip(image, image, 1);

    //imwrite("rotated90.jpg", dst);
/*
    if (imageType)
        img= QImage((const unsigned char *)(dst.data),  // Qt image structure
                dst.cols,dst.rows,QImage::Format_Indexed8);
    else
        img= QImage((const unsigned char *)(dst.data),  // Qt image structure
                dst.cols,dst.rows,QImage::Format_RGB888);
*/
    imageDisplay(image);
}
#endif

#if 0
    // int i;
    cv::Mat dst;
    int angle = 0;
    int delta = 1;
    double factor = 1.0;

    for (; ;) {
        float m[6];

        // cv::Mat M = Mat(2, 3, CV_32F, m);
        cv::Mat transMat(2, 3, CV_32F, m);
        int w = image.cols;
        int h = image.rows;

        m[0] = (float)(factor * cos(-angle*2*CV_PI/180.));
        m[1] = (float)(factor * sin(-angle*2*CV_PI/180.));
        m[3] = -m[1];
        m[4] = m[0];
        m[2] = w * 0.5f;
        m[5] = h * 0.5f;

        // cvZero(dst);
        // cvGetQuadrangleSubPix(image, dst, &M);
        warpAffine(image, dst, transMat, cv::Size(image.cols, image.rows), cv::WARP_INVERSE_MAP | cv::INTER_LINEAR, IPL_BORDER_CONSTANT);
        // cv::warpAffine(image, dst, transMat, cv::Size(image.cols, image.rows));


        // if (cvWaitKey(1) == 27)
            // break;
        imageDisplay(dst);
        angle = (int)(angle + delta)%360;
    }
#endif


void ImageViewer::horizontalMirrorClick()
{
    cv::flip(image, image, 1);

/*
    if (imageType)
        img= QImage((const unsigned char *)(image.data),  // Qt image structure
                image.cols,image.rows,QImage::Format_Indexed8);
    else
        img= QImage((const unsigned char *)(image.data),  // Qt image structure
                image.cols,image.rows,QImage::Format_RGB888);
*/
    imageDisplay(image);

    //cv::namedWindow("Output Image");
    //cv::imshow("Output Image", image);
}

void ImageViewer::verticalMirrorClick()
{
    cv::flip(image, image, 0);
/*
    if (imageType)
        img= QImage((const unsigned char *)(image.data),  // Qt image structure
                image.cols,image.rows,QImage::Format_Indexed8);
    else
        img= QImage((const unsigned char *)(image.data),  // Qt image structure
                image.cols,image.rows,QImage::Format_RGB888);
*/
    imageDisplay(image);
}

void ImageViewer::addImageClick()
{
    cv::Mat addmat;         //��ˮӡ���ͼ��
    cv::Mat imageROI;       //����ͼ��ROI
    cv::Mat logo;           //ˮӡlogo
    cv::Mat mask;
    //int logoType = 0;       //=0:͸���ı�ڣ�=1��mask��ʽ����ʾ��Ӱ��=2���Ҷ���ʽ��ʾlogo

    if (logoType > 2)
        logoType = 0;

    addmat = image.clone();
    QString logoName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());
    if (!logoName.isEmpty()) {
        QImage qimage(logoName);     // C++�ĸ߼��÷��������Կ��Ը�Ϊ��QImage image = QImage(fileName)
        if (qimage.isNull()) {
            QMessageBox::information(this, tr("logo viewer"),
                                     tr("Cannot load %1.").arg(logoName));
            return;
        }

        logo= cv::imread(logoName.toAscii().data());
        if (!imageType)
            img= QImage((const unsigned char*)(addmat.data),  // Qt image structure
                    addmat.cols,addmat.rows,QImage::Format_RGB888);
        else
            img= QImage((const unsigned char*)(addmat.data),  // Qt image structure
                    addmat.cols,addmat.rows,QImage::Format_Indexed8);

        switch (logoType) {
        case 0:     // logoͼƬԭ����͸����Ϊ��ɫ
            imageROI = addmat(cv::Rect(10, 10, logo.cols, logo.rows));
            // cv::addWeighted(imageROI,1.0,logo,0.3,0.,imageROI);
            logo.copyTo(imageROI);
            logoType++;
            imageDisplay(image);
            break;

        case 1:     // mask��ʽ����ʾԭ������Ӱ
            imageROI = addmat(cv::Rect(80, 80, logo.cols, logo.rows));
            mask= cv::imread(logoName.toAscii().data(), 0);
            logo.copyTo(imageROI, mask);
            logoType++;
            imageDisplay(image);
            break;

        case 2:     // ��ȫ�Ҷ���ʽ��ʾlogo
            imageROI = addmat(cv::Rect(160, 160, logo.cols, logo.rows));
            cv::addWeighted(imageROI, 1.0, logo, 0.3, 0, imageROI);
            logoType++;
            imageDisplay(image);
            break;

        default:
            break;
        }
    }
}
