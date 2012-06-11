#if !defined HISTOGRAM
#define HISTOGRAM

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

class Histogram1D {
  private:
    int histSize[1];
    float hranges[2];
    const float *ranges[1];
    int channels[1];

  public:
    Histogram1D() {
        histSize[0] = 256;
        hranges[0] = 0.0;
        hranges[1] = 255.0;
        ranges[0] = hranges;
        channels[0] = 0;        //Ĭ��ͨ����Ϊ0
    }

    /*
     * ���ý��м����ֱ��ͼ��ͨ����
     * Ĭ��Ϊ0
     */
    void setChannel(int c) {
        channels[0] = c;
    }

    /*
     * �õ�ʹ�ù���ͨ����
     */
    int getChannel() {

        return channels[0];
    }

    /*
     * ���ûҶȼ���Ĭ��Ϊ[0, 255]
     */
    void setRange(float minValue, float maxValue) {

         hranges[0]= minValue;
         hranges[1]= maxValue;
    }

    /*
     * ������С�Ҷȼ�
     */
    float getMinValue() {
        return hranges[0];
    }

    /*
     * �������Ҷȼ�
     */
    float getMaxValue() {
        return hranges[1];
    }

    /*
     * ����histogram��bin��Ŀ��
     * Ĭ��Ϊ256
     */
    void setNBins(int nbins) {
        histSize[0] = nbins;
    }

    /*
     * ��ȡbin��Ŀ
     */
    int getNBins() {
        return histSize[0];
    }

    /*
     * ����Ҷ�ֱ��ͼ
     */
    cv::MatND getHistogram(const cv::Mat &image) {
        cv::MatND hist;

        cv::calcHist(&image,
            1,
            channels,
            cv::Mat(),
            hist,
            1,
            histSize,
            ranges
        );

        return hist;
    }

    /*
     * ����һάֱ��ͼ����������ͼ��
     */
    cv::Mat getHistogramImage(const cv::Mat &image) {

        cv::MatND hist = getHistogram(image);

        double minVal = 0;
        double maxVal = 0;

        cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

        cv::Mat histImg(histSize[0], histSize[0], CV_8U,cv::Scalar(255));

        int hpt = static_cast<int>(0.9*histSize[0]);

        for (int h = 0; h < histSize[0]; h++) {
            float binVal = hist.at<float>(h);
            int intensity = static_cast<int>(binVal*hpt/maxVal);
            cv::line(histImg, cv::Point(h, histSize[0]), cv::Point(h,histSize[0]-intensity),cv::Scalar::all(0));
        }

        return histImg;
    }

    /*
     * �Ҷ�ͼ����
     */
    cv::Mat equalize(const cv::Mat &image) {
        cv::Mat result;
        cv::equalizeHist(image, result);

        return result;
    }

    /*
     * ��ԭͼ����лҶ�����
     */
    cv::Mat stretch(const cv::Mat &image, int minValue=0) {

        int imin= 0;
        int imax= histSize[0]-1;
        int dims[1]={256};
        cv::MatND hist= getHistogram(image);        //����ֱ��ͼ

        // ����ֱ��ͼ����
        for( ; imin < histSize[0]; imin++ ) {
            std::cout<<hist.at<float>(imin)<<std::endl;
            if (hist.at<float>(imin) > minValue)
                break;
        }

        // �ҵ�ֱ��ͼ����
        for( ; imax >= 0; imax-- ) {

            if (hist.at<float>(imax) > minValue)
                break;
        }

        // �������ұ�
        cv::MatND lookup(1, dims, CV_8U);

        for (int i=0; i<256; i++) {

            if (i < imin) lookup.at<uchar>(i) = 0;
            else if (i > imax) lookup.at<uchar>(i) = 255;
            else lookup.at<uchar>(i) = static_cast<uchar>(255.0*(i-imin)/(imax-imin)+0.5);
        }

        // Ӧ�ò��ұ�
        cv::Mat result;
        result= applyLookUp(image, lookup);

        return result;
    }

    /*
     * Ӧ�ò��ұ�ԭʼͼ��ת���ɵ�ͨ��ͼ��
     */
    cv::Mat applyLookUp(const cv::Mat& image, const cv::MatND& lookup) {

        // �������ͼ�� (ͨ��Ϊ 1-channel)
        cv::Mat result(image.rows,image.cols,CV_8U);
        cv::Mat_<uchar>::iterator itr = result.begin<uchar>();

        // �������������ͼ��
        cv::Mat_<uchar>::const_iterator it = image.begin<uchar>();
        cv::Mat_<uchar>::const_iterator itend = image.end<uchar>();

        // �����ұ�Ӧ����ÿ������
        for ( ; it != itend; ++it, ++itr) {

            *itr = lookup.at<uchar>(*it);
        }

        return result;
    }
};


#endif
