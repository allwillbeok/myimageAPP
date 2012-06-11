#include "imageviewer.h"
#include <QtGui>

void ImageViewer::colordetectorClick()
{
    //cv::Mat detectorM;

    cdetect.setTargetColor(130, 190, 230); // ����������ɫ��յ���ɫ

    if (image.channels() < 3) {

        QMessageBox::information(this, tr("warning"),
                                 tr("Cannot support grayscale image!!!").arg(fileName));
        return;
    }
    image = cdetect.process(image);

    // cv::namedWindow("detect");
    // cv::imshow("result", detectorM);
    // cv::waitKey();

    /*
     * ��colordetectorLab���Ѿ�����ͼ���ͨ����ʽΪ�Ҷ�(��ͨ����ʽ)
     * ������QT����ʾ��ʱ������Ϊ��Format_Indexed8������������ʾ
     */
    //img= QImage((const unsigned char *)(detectorM.data),       // Qt image structure
                //detectorM.cols, detectorM.rows, QImage::Format_Indexed8);
    // else
        //img= QImage((const unsigned char *)(detectorM.data),  // Qt image structure
                //detectorM.cols, detectorM.rows, QImage::Format_RGB888);
    imageDisplay(image);
}
