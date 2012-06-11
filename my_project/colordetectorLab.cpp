#include "colordetector.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>

cv::Mat ColorDetector::process(const cv::Mat &image) {

          cv::Mat converted;

          /*
           * ���ݴ�Сͬimage�����ǲ�����ԭͼ��Ļ�����
           * ��ͼ��ĸ�ʽ����Ϊ��ͨ���ĻҶ�ͼ����QT����ʾ�������
           */
          result.create(image.rows,image.cols,CV_8U);

          /*
           * ���ݴ�Сͬimage�����ǲ�����ԭͼ��Ļ����壬
           * ��ʽͬԭͼ��ĸ�ʽ,=image.type()
           */
          converted.create(image.rows,image.cols,image.type());
          qDebug()<<image.type();

          // �ı���ɫ��Lab color�ռ�
          cv::cvtColor(image, converted, CV_BGR2Lab);

          qDebug()<<"the convert type is "<<converted.type();

          // ��ȡ����
          cv::Mat_<cv::Vec3b>::iterator it= converted.begin<cv::Vec3b>();
          cv::Mat_<cv::Vec3b>::iterator itend= converted.end<cv::Vec3b>();
          cv::Mat_<uchar>::iterator itout= result.begin<uchar>();

          for ( ; it!= itend; ++it, ++itout) {

                // process each pixel ---------------------
                  // ����Ŀ��ͼ�������ɫֵ����
                  if (getDistance(*it)<minDist) {

                          *itout= 255;

                  } else {

                          *itout= 0;
                  }

        //end
          }

          return result;
}
