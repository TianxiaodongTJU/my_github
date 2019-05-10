#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stack>
#include <algorithm>//*min_element(p,p+n)º¯Êý
#include <numeric>//iotaº¯Êý
#include <math.h>

using namespace std;
using namespace cv;

#define cut_rows 3
#define cut_cols 3


void AdaptiveFindThreshold(const Mat src, double& low, double& high, int aperture_size = 3)
{
    const int cn = src.channels();
    cv::Mat dx_img(src.rows, src.cols, CV_16SC(cn));
    cv::Mat dy_img(src.rows, src.cols, CV_16SC(cn));

    cv::Sobel(src, dx_img, CV_16S, 1, 0, aperture_size, 1, 0, cv::BORDER_DEFAULT);
    cv::Sobel(src, dy_img, CV_16S, 0, 1, aperture_size, 1, 0, cv::BORDER_DEFAULT);

    const int width = dx_img.cols;
    const int height = dx_img.rows;

    // ŒÆËã±ßÔµµÄÇ¿¶È, ²¢ŽæÓÚÍŒÏñÖÐ
    float maxv = 0;
    Mat img_dxy = Mat(height, width, CV_8UC1);
    for (int i = 0; i < height; i++)
    {
        const short* _dx = (short*)(dx_img.data + dx_img.step*i);//Ã¿Ò»ÐÐµÄÊ×µØÖ·
        const short* _dy = (short*)(dy_img.data + dy_img.step*i);
        for (int j = 0; j < width; j++)
        {
            img_dxy.ptr<uchar>(i)[j]= (int)(abs(_dx[j]) + abs(_dy[j]));
            maxv = maxv < img_dxy.ptr(i)[j] ? img_dxy.ptr(i)[j] : maxv;
        }
    }
    Mat BW;
    low=threshold(img_dxy, BW, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

high = low * 2.5;

}

void CutPics(Mat& srcImg, vector<Mat>& ceilImg, int height, int width) {

    int ceil_height = (int)(height / cut_rows);
    int ceil_width = (int)(width / cut_cols);
    int ceil_down_height = height - (cut_rows - 1)*ceil_height;
    int ceil_right_width = width - (cut_cols - 1)*ceil_width;

    for (int i = 0; i < cut_rows - 1; i++)
        for (int j = 0; j < cut_cols; j++)
        {
            if (j < cut_cols - 1)
            {
                Rect rect(j*ceil_width, i*ceil_height, ceil_width, ceil_height);
                ceilImg.push_back(srcImg(rect));

            }
            else
            {
                Rect rect((cut_cols - 1)*ceil_width, i*ceil_height, ceil_right_width, ceil_height);
                ceilImg.push_back(srcImg(rect));
            }
        }

    for (int i = 0; i < cut_cols; i++)
    {
        if (i < cut_cols - 1)
        {
            Rect rect(i*ceil_width, (cut_rows - 1)*ceil_height, ceil_width, ceil_down_height);
            ceilImg.push_back(srcImg(rect));
        }
        else   //ÓÒÏÂœÇÕâžöÍŒÏñ¿é
        {
            Rect rect((cut_cols - 1)*ceil_width, (cut_rows - 1)*ceil_height, ceil_right_width, ceil_down_height);
            ceilImg.push_back(srcImg(rect));
        }
    }

}

Mat StitchPics(vector<Mat>& ceilImg, int height,int width) {

    Mat dst(height, width, CV_8UC1, Scalar(0));
    int ceil_x = 0, ceil_y = 0;
    for (auto ptr = ceilImg.begin(); ptr != ceilImg.end(); ++ptr) {

        Rect rect(ceil_x, ceil_y, (*ptr).cols, (*ptr).rows);
        (*ptr).copyTo(dst(rect));

        if (ceil_x + (*ptr).cols >= width) {
            ceil_y += (*ptr).rows;
            ceil_x = 0;
        }
        else {
            ceil_x += (*ptr).cols;
        }
    }

    return dst;

}

//Ñ°ÕÒµãŒ¯µÄ×îÐ¡ÍâœÓŸØÐÎµÄ×óÉÏµãºÍÓÒÏÂµã
void Straightness(Point& X1, Point& X2, vector<Point>& domain, int height ,int width) {
    RotatedRect rect = minAreaRect(domain);
    Point2f fourPoint2f[4];
    rect.points(fourPoint2f);//×îÐ¡ÍâœÓŸØÐÎµÄËÄžö¶¥µã

    float x[4], y[4];
    for (int i = 0; i < 4; ++i) {
        x[i] = fourPoint2f[i].x;
        y[i] = fourPoint2f[i].y;
    }
    X1.x = cvFloor(*min_element(x, x + 4))-1 < 0 ? 0 : cvFloor(*min_element(x, x + 4))-1;
    X2.x = cvCeil(*max_element(x, x + 4))+1 > (width-1) ? (width-1) : cvCeil(*max_element(x, x + 4))+1;
    X1.y = cvFloor(*min_element(y, y + 4))-1 < 0 ? 0 : cvFloor(*min_element(y, y + 4))-1;
    X2.y = cvCeil(*max_element(y, y + 4))+1 > (height-1) ? (height-1) : cvCeil(*max_element(y, y + 4))+1;
}

void binaryzation(Mat & srcImg) {
    Mat lookUpTable(1, 256, CV_8U, Scalar(255));
    lookUpTable.data[0] = 0;
    LUT(srcImg, lookUpTable, srcImg);
}


void findConnectedDomain(Mat & srcImg, vector<vector<Point>>& connectedDomains, int width_th, int area, float WHRatio, vector<int> & DomainsWidth) {
    Mat_<uchar> tempImg = (Mat_<uchar> &)srcImg;

    for (int i = 0; i < tempImg.rows; ++i) {
        uchar* row = tempImg.ptr(i);
        for (int j = 0; j < tempImg.cols; ++j) {
            if (row[j] == 255) {
                stack<Point> connectedPoints;
                vector<Point> domain;
                connectedPoints.push(Point(j, i));
                while (!connectedPoints.empty()) {
                    Point currentPoint = connectedPoints.top();
                    domain.push_back(currentPoint);

                    int colNum = currentPoint.x;
                    int rowNum = currentPoint.y;

                    tempImg.ptr(rowNum)[colNum] = 0;
                    connectedPoints.pop();

                    if (rowNum - 1 >= 0 && colNum - 1 >= 0 && tempImg.ptr(rowNum - 1)[colNum - 1] == 255) {
                        tempImg.ptr(rowNum - 1)[colNum - 1] = 0;
                        connectedPoints.push(Point(colNum - 1, rowNum - 1));
                    }
                    if (rowNum - 1 >= 0 && tempImg.ptr(rowNum - 1)[colNum] == 255) {
                        tempImg.ptr(rowNum - 1)[colNum] = 0;
                        connectedPoints.push(Point(colNum, rowNum - 1));
                    }
                    if (rowNum - 1 >= 0 && colNum + 1 < tempImg.cols && tempImg.ptr(rowNum - 1)[colNum + 1] == 255) {
                        tempImg.ptr(rowNum - 1)[colNum + 1] = 0;
                        connectedPoints.push(Point(colNum + 1, rowNum - 1));
                    }
                    if (colNum - 1 >= 0 && tempImg.ptr(rowNum)[colNum - 1] == 255) {
                        tempImg.ptr(rowNum)[colNum - 1] = 0;
                        connectedPoints.push(Point(colNum - 1, rowNum));
                    }
                    if (colNum + 1 < tempImg.cols && tempImg.ptr(rowNum)[colNum + 1] == 255) {
                        tempImg.ptr(rowNum)[colNum + 1] = 0;
                        connectedPoints.push(Point(colNum + 1, rowNum));
                    }
                    if (rowNum + 1 < tempImg.rows && colNum - 1 > 0 && tempImg.ptr(rowNum + 1)[colNum - 1] == 255) {
                        tempImg.ptr(rowNum + 1)[colNum - 1] = 0;
                        connectedPoints.push(Point(colNum - 1, rowNum + 1));
                    }
                    if (rowNum + 1 < tempImg.rows && tempImg.ptr(rowNum + 1)[colNum] == 255) {
                        tempImg.ptr(rowNum + 1)[colNum] = 0;
                        connectedPoints.push(Point(colNum, rowNum + 1));
                    }
                    if (rowNum + 1 < tempImg.rows && colNum + 1 < tempImg.cols && tempImg.ptr(rowNum + 1)[colNum + 1] == 255) {
                        tempImg.ptr(rowNum + 1)[colNum + 1] = 0;
                        connectedPoints.push(Point(colNum + 1, rowNum + 1));
                    }
                }
                if (domain.size() > area) {
                    RotatedRect rect = minAreaRect(domain);
                    //Point2f fourPoint2f[4];
                    //rect.points(fourPoint2f);//×îÐ¡ÍâœÓŸØÐÎµÄËÄžö¶¥µã
                    //Straightness(fourPoint2f, domain);

                    float width = rect.size.width;
                    float height = rect.size.height;
                    if (width < height) {
                        float temp = width;
                        width = height;
                        height = temp;
                    }
                    if (width > height * WHRatio && width > width_th) {
                        for (auto cit = domain.begin(); cit != domain.end(); ++cit) {
                            tempImg.ptr(cit->y)[cit->x] = 128;
                        }
                        connectedDomains.push_back(domain);
                        DomainsWidth.push_back(int(width));
                    }
                }
            }
        }
    }

    binaryzation(srcImg);
}

bool FindEndpoint(Mat & srcImg, vector<Point> & ske) {
    Mat lookUpTable(1, 256, CV_8U,Scalar(1));
    lookUpTable.at<uchar>(0,0) = 0;
    LUT(srcImg, lookUpTable, srcImg);
    vector<Point> duandian, jiedian;//¶Ëµã£¬œÚµã
    for (int i = 0;i < srcImg.rows;++i) {
        uchar* row = srcImg.ptr(i);
        for (int j = 0;j < srcImg.cols;++j) {
            if (row[j]) {
                int cnt = 0;
                cnt = (row[j - 1] + row[j + 1] + (row - srcImg.step)[j - 1] + (row - srcImg.step)[j] + (row - srcImg.step)[j + 1] + (row + srcImg.step)[j - 1] + (row + srcImg.step)[j] + (row + srcImg.step)[j + 1]);
                if (cnt == 1) {
                    duandian.push_back(Point(j, i));
                }
            }
        }
    }
    lookUpTable.at<uchar>(0, 1) = 255;
    LUT(srcImg, lookUpTable, srcImg);
    if (duandian.size() != 0) {
        ske = duandian;
        return true;
    }
    else {
        return false;
    }
}

//ŽÓÐ¡µœŽóÅÅÐò²¢Ž¢ŽæË÷Òý
template <typename T> vector<size_t> sort_indexes(const vector<T> &v) {
    // ³õÊŒ»¯Ë÷ÒýÏòÁ¿
    vector<size_t> idx(v.size());
    //Ê¹ÓÃiota¶ÔÏòÁ¿ž³0~£¿µÄÁ¬ÐøÖµ
    iota(idx.begin(), idx.end(), 0);
    // Íš¹ý±ÈœÏvµÄÖµ¶ÔË÷ÒýidxœøÐÐÅÅÐò
    sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});
    return idx;
}

void zhang_thinimage_improve(Mat &srcimage)//µ¥ÍšµÀ¡¢¶þÖµ»¯ºóµÄÍŒÏñ
{
    vector<cv::Point> deletelist1;
    int Zhangmude[9];
    int nl = srcimage.rows;
    int nc = srcimage.cols;
    while (true)
    {
        for (int j = 1; j < (nl - 1); j++)
        {
            uchar* data_last = srcimage.ptr<uchar>(j - 1);
            uchar* data = srcimage.ptr<uchar>(j);
            uchar* data_next = srcimage.ptr<uchar>(j + 1);
            for (int i = 1; i < (nc - 1); i++)
            {
                if (data[i] == 255)
                {
                    Zhangmude[0] = 1;
                    if (data_last[i] == 255) Zhangmude[1] = 1;
                    else  Zhangmude[1] = 0;
                    if (data_last[i + 1] == 255) Zhangmude[2] = 1;
                    else  Zhangmude[2] = 0;
                    if (data[i + 1] == 255) Zhangmude[3] = 1;
                    else  Zhangmude[3] = 0;
                    if (data_next[i + 1] == 255) Zhangmude[4] = 1;
                    else  Zhangmude[4] = 0;
                    if (data_next[i] == 255) Zhangmude[5] = 1;
                    else  Zhangmude[5] = 0;
                    if (data_next[i - 1] == 255) Zhangmude[6] = 1;
                    else  Zhangmude[6] = 0;
                    if (data[i - 1] == 255) Zhangmude[7] = 1;
                    else  Zhangmude[7] = 0;
                    if (data_last[i - 1] == 255) Zhangmude[8] = 1;
                    else  Zhangmude[8] = 0;
                    int whitepointtotal = 0;
                    for (int k = 1; k < 9; k++)
                    {
                        //µÃµœ1µÄžöÊý
                        whitepointtotal = whitepointtotal + Zhangmude[k];
                    }
                    if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
                    {
                        //µÃµœ01µÄžöÊý
                        int ap = 0;
                        if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;
                        if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;
                        if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;
                        if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;
                        if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;
                        if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;
                        if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;
                        if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;
                        //ŒÆËãbp
                        int bp = 0;
                        bp += Zhangmude[1];
                        bp += Zhangmude[2] << 1;
                        bp += Zhangmude[3] << 2;
                        bp += Zhangmude[4] << 3;
                        bp += Zhangmude[5] << 4;
                        bp += Zhangmude[6] << 5;
                        bp += Zhangmude[7] << 6;
                        bp += Zhangmude[8] << 7;

                        if (ap == 1 || bp == 65 || bp == 5 || bp == 20 || bp == 80 || bp == 13 || bp == 22 || bp == 52 || bp == 133 || bp == 141 || bp == 54)
                        {
                            if ((Zhangmude[1] * Zhangmude[7] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[5] * Zhangmude[7] == 0))
                            {
                                deletelist1.push_back(cv::Point(i, j));  //Âú×ãÌõŒþ£¬È¥³ýžÃµã
                            }
                        }
                    }
                }
            }
        }
        if (deletelist1.size() == 0) break;
        for (size_t i = 0; i < deletelist1.size(); i++)
        {
            cv::Point tem;
            tem = deletelist1[i];
            uchar* data = srcimage.ptr<uchar>(tem.y);
            data[tem.x] = 0;
        }
        deletelist1.clear();


        for (int j = 1; j < (nl - 1); j++)
        {
            uchar* data_last = srcimage.ptr<uchar>(j - 1);
            uchar* data = srcimage.ptr<uchar>(j);
            uchar* data_next = srcimage.ptr<uchar>(j + 1);
            for (int i = 1; i < (nc - 1); i++)
            {
                if (data[i] == 255)
                {
                    Zhangmude[0] = 1;
                    if (data_last[i] == 255) Zhangmude[1] = 1;
                    else  Zhangmude[1] = 0;
                    if (data_last[i + 1] == 255) Zhangmude[2] = 1;
                    else  Zhangmude[2] = 0;
                    if (data[i + 1] == 255) Zhangmude[3] = 1;
                    else  Zhangmude[3] = 0;
                    if (data_next[i + 1] == 255) Zhangmude[4] = 1;
                    else  Zhangmude[4] = 0;
                    if (data_next[i] == 255) Zhangmude[5] = 1;
                    else  Zhangmude[5] = 0;
                    if (data_next[i - 1] == 255) Zhangmude[6] = 1;
                    else  Zhangmude[6] = 0;
                    if (data[i - 1] == 255) Zhangmude[7] = 1;
                    else  Zhangmude[7] = 0;
                    if (data_last[i - 1] == 255) Zhangmude[8] = 1;
                    else  Zhangmude[8] = 0;
                    int whitepointtotal = 0;
                    for (int k = 1; k < 9; k++)
                    {
                        whitepointtotal = whitepointtotal + Zhangmude[k];
                    }
                    if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
                    {
                        int ap = 0;
                        if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;
                        if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;
                        if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;
                        if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;
                        if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;
                        if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;
                        if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;
                        if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;
                        int bp = 0;
                        bp += Zhangmude[1];
                        bp += Zhangmude[2] << 1;
                        bp += Zhangmude[3] << 2;
                        bp += Zhangmude[4] << 3;
                        bp += Zhangmude[5] << 4;
                        bp += Zhangmude[6] << 5;
                        bp += Zhangmude[7] << 6;
                        bp += Zhangmude[8] << 7;
                        if (ap == 1 || bp == 65 || bp == 5 || bp == 20 || bp == 80 || bp == 13 || bp == 22 || bp == 52 || bp == 133 || bp == 141 || bp == 54)
                        {
                            if ((Zhangmude[1] * Zhangmude[3] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[1] * Zhangmude[7] == 0))
                            {
                                deletelist1.push_back(cv::Point(i, j));
                            }
                        }
                    }
                }
            }
        }
        if (deletelist1.size() == 0) break;
        for (size_t i = 0; i < deletelist1.size(); i++)
        {
            cv::Point tem;
            tem = deletelist1[i];
            uchar* data = srcimage.ptr<uchar>(tem.y);
            data[tem.x] = 0;
        }
        deletelist1.clear();
    }
}

void FindMinPoint(Mat & srcImg, Point crack, Point & new_crack) {
    //ÏñËØÖµÑ¹Èë¹æÔò£¬Ê×ÏÈÊÇnew_crack£¬È»ºóÊÇcrack
    //	4 3 2
    //	5 0 1
    //	6 7 8
    vector<uchar> gray;
    gray.push_back(srcImg.at<uchar>(crack));
    gray.push_back(srcImg.at<uchar>(crack.y, crack.x + 1));
    gray.push_back(srcImg.at<uchar>(crack.y - 1, crack.x + 1));
    gray.push_back(srcImg.at<uchar>(crack.y - 1, crack.x));
    gray.push_back(srcImg.at<uchar>(crack.y - 1, crack.x - 1));
    gray.push_back(srcImg.at<uchar>(crack.y, crack.x - 1));
    gray.push_back(srcImg.at<uchar>(crack.y + 1, crack.x - 1));
    gray.push_back(srcImg.at<uchar>(crack.y + 1, crack.x));
    gray.push_back(srcImg.at<uchar>(crack.y + 1, crack.x + 1));
    //ÉÏÃæÊÇcrack£¬ÏÂÃæÊÇnew_crack
    gray.push_back(srcImg.at<uchar>(new_crack));
    if (new_crack.x + 1 < srcImg.cols) {
        gray.push_back(srcImg.at<uchar>(new_crack.y, new_crack.x + 1));
    }
    else {
        gray.push_back(255);
    }
    if (new_crack.x + 1 < srcImg.cols && new_crack.y - 1 >= 0) {
        gray.push_back(srcImg.at<uchar>(new_crack.y - 1, new_crack.x + 1));
    }
    else {
        gray.push_back(255);
    }
    if (new_crack.y - 1 >= 0) {
        gray.push_back(srcImg.at<uchar>(new_crack.y - 1, new_crack.x));
    }
    else {
        gray.push_back(255);
    }
    if (new_crack.y - 1 >= 0 && new_crack.x - 1 >= 0) {
        gray.push_back(srcImg.at<uchar>(new_crack.y - 1, new_crack.x - 1));
    }
    else {
        gray.push_back(255);
    }
    if (new_crack.x - 1 >= 0) {
        gray.push_back(srcImg.at<uchar>(new_crack.y, new_crack.x - 1));
    }
    else {
        gray.push_back(255);
    }
    if (new_crack.x - 1 >= 0 && new_crack.y + 1 < srcImg.rows) {
        gray.push_back(srcImg.at<uchar>(new_crack.y + 1, new_crack.x - 1));
    }
    else {
        gray.push_back(255);
    }
    if (new_crack.y + 1 < srcImg.rows) {
        gray.push_back(srcImg.at<uchar>(new_crack.y + 1, new_crack.x));
    }
    else {
        gray.push_back(255);
    }
    if (new_crack.y + 1 < srcImg.rows && new_crack.x + 1 < srcImg.cols) {
        gray.push_back(srcImg.at<uchar>(new_crack.y + 1, new_crack.x + 1));
    }
    else {
        gray.push_back(255);
    }
    //idxŽ¢ŽæË÷Òý
    vector<size_t>idx;
    idx = sort_indexes(gray);

    switch (idx[0])
    {
    case 0:
        new_crack = Point(crack.x, crack.y);
        break;
    case 1:
        new_crack = Point(crack.x+1, crack.y);
        break;
    case 2:
        new_crack = Point(crack.x + 1, crack.y - 1);
        break;
    case 3:
        new_crack = Point(crack.x, crack.y - 1);
        break;
    case 4:
        new_crack = Point(crack.x - 1, crack.y - 1);
        break;
    case 5:
        new_crack = Point(crack.x - 1, crack.y);
        break;
    case 6:
        new_crack = Point(crack.x - 1, crack.y + 1);
        break;
    case 7:
        new_crack = Point(crack.x, crack.y + 1);
        break;
    case 8:
        new_crack = Point(crack.x + 1, crack.y + 1);
        break;
    case 9:
        new_crack = Point(new_crack.x, new_crack.y);
        break;
    case 10:
        new_crack = Point(new_crack.x + 1, new_crack.y);
        break;
    case 11:
        new_crack = Point(new_crack.x + 1, new_crack.y - 1);
        break;
    case 12:
        new_crack = Point(new_crack.x, new_crack.y - 1);
        break;
    case 13:
        new_crack = Point(new_crack.x - 1, new_crack.y - 1);
        break;
    case 14:
        new_crack = Point(new_crack.x - 1, new_crack.y);
        break;
    case 15:
        new_crack = Point(new_crack.x - 1, new_crack.y + 1);
        break;
    case 16:
        new_crack = Point(new_crack.x, new_crack.y + 1);
        break;
    case 17:
        new_crack = Point(new_crack.x + 1, new_crack.y + 1);
        break;
    default:
        break;
    }
}


int bSums(Mat src)
{

    int counter = 0;
    //µüŽúÆ÷·ÃÎÊÏñËØµã
    Mat_<uchar>::iterator it = src.begin<uchar>();
    Mat_<uchar>::iterator itend = src.end<uchar>();
    for (; it != itend; ++it)
    {
        if ((*it) > 0) counter += 1;//¶þÖµ»¯ºó£¬ÏñËØµãÊÇ0»òÕß255
    }
    return counter;
}

void findwidth(Mat & cannyImg, Point last_point, Point this_point, float & width) {
    const float radical_2 = 1.41421356;
    int flag = 0;
    int th = width + 2;//ÕÒ±ßÔµµãµÄ×îŽóŸàÀë
    float left_W[4] = { 0,0,0,0 }, right_W[4] = { 0,0,0,0 }, W[4] = { 0,0,0,0 };
    //n=2Ê±·šÏòÎªË®Æœ·œÏò£¬ÔÚË®Æœ·œÏòœøÐÐÑ°ÕÒ,žÄ±äxµÄÖµ
    for (int i = 1; i <= th; ++i) {
        if (this_point.x - i >= 0) {	//Ïò×óižöÏñËØµã
            if (cannyImg.ptr(this_point.y)[this_point.x - i] == 255) {
                left_W[0] = i;
                break;
            }
        }
        else {
            left_W[0] = 0;
            break;
        }
    }
    for (int i = 1; i <= th; ++i) {
        if (this_point.x + i < cannyImg.cols) {	//ÏòÏÂižöÏñËØµã
            if (cannyImg.ptr(this_point.y)[this_point.x + i] == 255) {
                right_W[0] = i;
                break;
            }
        }
        else {
            right_W[0] = 0;
            break;
        }
    }
    //n=0Ê±·šÏò·œÏòÎªŽ¹Ö±·œÏò£¬ÔÚŽ¹Ö±·œÏòœøÐÐÑ°ÕÒ£¬žÄ±äxµÄÖµ
    for (int i = 1; i <= th; ++i) {
        if (this_point.y - i >= 0) {	//ÏòÉÏižöÏñËØµã
            if (cannyImg.ptr(this_point.y - i)[this_point.x] == 255) {
                left_W[1] = i;
                break;
            }
        }
        else {
            left_W[1] = 0;
            break;
        }
    }
    for (int i = 1; i <= th; ++i) {
        if (this_point.y + i < cannyImg.rows) {	//ÏòÏÂižöÏñËØµã
            if (cannyImg.ptr(this_point.y + i)[this_point.x] == 255) {
                right_W[1] = i;
                break;
            }
        }
        else {
            right_W[1] = 0;
            break;
        }
    }
    //n=1·šÏò·œÏòÎª¶þËÄÏóÏÞ¶ÔœÇÏß·œÏò£¬×¢ÒâÔÚÍŒÏñ×ø±êÏµÖÐy·ŽÏò
    for (int i = 1; i <= th; ++i) {
        if (this_point.x + i < cannyImg.cols  && this_point.y - i >= 0) {
            if (cannyImg.ptr(this_point.y - i)[this_point.x + i] == 255) {
                right_W[2] = radical_2*i;
                break;
            }
        }
        else {
            right_W[2] = 0;
            break;
        }
    }
    for (int i = 1; i <= th; ++i) {
        if (this_point.x - i >= 0 && this_point.y + i < cannyImg.rows) {
            if (cannyImg.ptr(this_point.y + i)[this_point.x - i] == 255) {
                left_W[2] = radical_2*i;
                break;
            }
        }
        else {
            left_W[2] = 0;
            break;
        }
    }
    //·šÏò·œÏòÎªÒ»ÈýÏóÏÞ¶ÔœÇÏß·œÏò
    for (int i = 1; i <= th; ++i) {
        if (this_point.x - i >= 0 && this_point.y - i >= 0) {
            if (cannyImg.ptr(this_point.y - i)[this_point.x - i] == 255) {
                left_W[3] = radical_2*i;
                break;
            }
        }
        else {
            left_W[3] = 0;
            break;
        }
    }
    for (int i = 1; i <= th; ++i) {
        if (this_point.x + i < cannyImg.cols && this_point.y + i < cannyImg.rows) {
            if (cannyImg.ptr(this_point.y + i)[this_point.x + i] == 255) {
                right_W[3] = radical_2*i;
                break;
            }
        }
        else {
            right_W[3] = 0;
            break;
        }
    }

    for (int i = 0; i < 4; ++i) {
        if (left_W[i] && right_W[i]) {
            W[i] = left_W[i] + right_W[i];
            flag = 1;
        }
    }
    if (flag == 0) {
        for (int i = 0; i < 4; ++i) {
            if (left_W[i] || right_W[i]) {
                W[i] = 2 * (left_W[i] + right_W[i]);
                flag = 1;
            }
        }
    }

    switch (flag)
    {
    case 0:
        width = 0;
        break;
    case 1:
        width = 100;
        for (int i = 0; i < 4; i++) {
            if (W[i] < width && W[i] != 0) {
                width = W[i];
            }
        }
        break;
    default:
        width = 0;
        break;
    }

}


int main() {
    Mat srcImg = imread("1.jpg",-1);//ºÄÊ±201ms
    //cvtColor(srcImg, srcImg, CV_BGR2GRAY); //Èç¹û¶ÁÈëµÄÊÇ²ÊÉ«ÍŒÏñÐèÒª×ª»»ÎªºÚ°×ÍŒ
    Mat output(srcImg.rows, srcImg.cols, CV_8UC3, Scalar(0, 0, 255));//ºÄÊ±105ms
    Mat srcImg_clone = srcImg.clone();

    int height = srcImg.rows;
    int width = srcImg.cols;

    //œ«ÍŒÏñ·ÖÎªcut_rows*cut_colsžöÐ¡¿éœøÐÐcannyÔËËãµÃµœ±ßÔµ
    vector<Mat> ceilImg,ceilCanny;
    CutPics(srcImg, ceilImg, height,width);

    for(auto ptr = ceilImg.begin(); ptr != ceilImg.end(); ++ptr){
        double low = 0; double high = 0;
        Mat dstImg;
        AdaptiveFindThreshold(*ptr, low, high);
        Canny(*ptr, dstImg, low, high);
        ceilCanny.push_back(dstImg);
    }//ºÄÊ±2.2s

    Mat edge=StitchPics(ceilCanny, height, width);


    //±ßÔµµÄÁ¬ÍšÓòÉžÑ¡
    vector<vector<Point>> connectDomains;
    vector<int> DomainsWidth;
    //vector<vector<Point>> Rect_X1_X2;
    //labelÄÚ²¿Ž¢ŽæÁÑ·ìµÄ±êÇ©£¬Í¬Ò»žö±êÇ©±íÊŸÎªÒ»ÌõÁÑ·ì
    //vector<int> label;
    vector<size_t>idx;
    int area = 20;float WHRatio = 2;
    float width_th = 50.0;
    findConnectedDomain(edge, connectDomains, width_th, area, WHRatio, DomainsWidth);//ºÄÊ±3434ms

    Mat edge_copy = edge.clone();




    idx = sort_indexes(DomainsWidth);//¶ÔÍâœÓŸØÐÎµÄ³€±ß³€¶ÈœøÐÐÅÅÐò£¬idxÊÇÔ­vectorµÄË÷Òý£¬ŽÓÐ¡µœŽó
    sort(DomainsWidth.begin(), DomainsWidth.end());

    //¶ÔËùÓÐµÄÁ¬ÍšÓò¿ªÊŒ¶Ô±ßÔµœøÐÐŽŠÀí£¬
    for (int i = 1;i < idx.size();i++) {
        //label.push_back(i);
        Point X1, X2;//X1ÊÇ×óÉÏµã£¬X2ÊÇÓÒÏÂµã
        vector<Point> X1_X2;//Ž¢ŽæX1ºÍX2µÄÈÝÆ÷
        int edge_num = connectDomains[idx[idx.size() - i]].size();//±ßÔµµãµÄÊýÁ¿
        Straightness(X1, X2, connectDomains[idx[idx.size() - i]], height,width);//ŒÆËãÍâœÓŸØÐÎµÄ×óÉÏµãºÍÓÒÏÂµã×ø±ê
        //œ«×ø±êŽæÈëRect_X1_X2ÖÐ
        X1_X2.push_back(X1);X1_X2.push_back(X2);
        //Rect_X1_X2.push_back(X1_X2);
        //¶ÔÍâœÓŸØÐÎRIOÄÚ²¿µÄÍŒÏñœøÐÐŽŠÀí
        Mat imageROI(X2.y - X1.y + 1, X2.x - X1.x + 1, CV_8UC1, Scalar(0));
        for (auto ptr = connectDomains[idx[idx.size() - i]].begin(); ptr != connectDomains[idx[idx.size() - i]].end(); ++ptr) {
            imageROI.at<uchar>((ptr->y) - X1.y, (ptr->x) - X1.x) = 255;
        }
        //Mat srcROI = srcImg(Range(X1.y - 1, X2.y + 2), Range(X1.x - 1, X2.x + 2));//ÒòÎªRange²»°üº¬×îºóÒ»žö
        Mat srcROI(imageROI.rows + 2, imageROI.cols + 2, imageROI.type(), Scalar(255));
        srcImg(Range(X1.y, X2.y + 1), Range(X1.x, X2.x + 1)).copyTo(srcROI(Range(1, imageROI.rows + 1), Range(1, imageROI.cols + 1)));

        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));// œá¹¹ÔªËØ3*3
        dilate(imageROI, imageROI, kernel);//ÅòÕÍ
        morphologyEx(imageROI, imageROI, CV_MOP_CLOSE, kernel);//±Õ
        kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
        morphologyEx(imageROI, imageROI, CV_MOP_CLOSE, kernel);//±Õ

        //Ïž»¯µÃµœ×î³õµÄ¹ÇŒÜ£¬²¢ÇÒ¶ÔÇøÓòµÄËÄÖÜÖÃ0
        zhang_thinimage_improve(imageROI);
        imageROI.col(imageROI.cols - 1) = Scalar(0);
        imageROI.col(0) = Scalar(0);
        imageROI.row(0) = Scalar(0);
        imageROI.row(imageROI.rows - 1) = Scalar(0);
        int ske_num = bSums(imageROI);   //¹ÇŒÜµãµÄÊýÁ¿
        double edge_ske = edge_num / ske_num;

        Mat imageROI_E(imageROI.rows + 2, imageROI.cols + 2, imageROI.type(),Scalar(0));
        imageROI.copyTo(imageROI_E(Range(1, imageROI.rows + 1), Range(1, imageROI.cols + 1)));

        stack<Point> crack;			//Ž¢ŽæŸÉ¹ÇŒÜµã
        stack<Point> new_crack;		//Ž¢ŽæÐÂ¹ÇŒÜµã
        vector<Point> ske;			//Ž¢Žæ¶Ëµã
        if (FindEndpoint(imageROI_E, ske)) {	//ÕÒµœ¶Ëµã
            crack.push(ske[0]);	new_crack.push(ske[0]);
            Mat skeROI = Mat(imageROI_E.rows, imageROI_E.cols, imageROI_E.type(), Scalar(0));
            //œøÐÐ¹ÇŒÜÐ£Õý
            while (!crack.empty()) {
                Point current_crack_point = crack.top();
                Point new_current_crack_point = new_crack.top();
                imageROI_E.ptr(current_crack_point.y)[current_crack_point.x] = 0;
                crack.pop();
                new_crack.pop();
                FindMinPoint(srcROI, current_crack_point, new_current_crack_point);
                skeROI.ptr(new_current_crack_point.y)[new_current_crack_point.x] = 255;
                srcROI.ptr(new_current_crack_point.y)[new_current_crack_point.x] = 255;
                //ÒÔ°ËÁÚÓòÑ°ÕÒÏÂÒ»žöµã
                int i = 1;//switchÓïŸäµÄ¿ª¹Ø±äÁ¿
                switch (i)
                {
                case 1:
                    if (imageROI_E.ptr(current_crack_point.y)[current_crack_point.x + 1] == 255 && new_current_crack_point.x + 1 < srcROI.cols) {
                        crack.push(current_crack_point + Point(1, 0));
                        new_crack.push(new_current_crack_point + Point(1, 0));
                    }
                case 2:
                    if (imageROI_E.ptr(current_crack_point.y - 1)[current_crack_point.x + 1] == 255 && new_current_crack_point.x + 1 < srcROI.cols && new_current_crack_point.y - 1 >= 0) {
                        crack.push(current_crack_point + Point(1, -1));
                        new_crack.push(new_current_crack_point + Point(1, -1));
                    }
                case 3:
                    if (imageROI_E.ptr(current_crack_point.y - 1)[current_crack_point.x] == 255 && new_current_crack_point.y - 1 >= 0) {
                        crack.push(current_crack_point + Point(0, -1));
                        new_crack.push(new_current_crack_point + Point(0, -1));
                    }
                case 4:
                    if (imageROI_E.ptr(current_crack_point.y - 1)[current_crack_point.x - 1] == 255 && new_current_crack_point.x - 1 >= 0 && new_current_crack_point.y - 1 >= 0) {
                        crack.push(current_crack_point + Point(-1, -1));
                        new_crack.push(new_current_crack_point + Point(-1, -1));
                    }
                case 5:
                    if (imageROI_E.ptr(current_crack_point.y)[current_crack_point.x - 1] == 255 && new_current_crack_point.x - 1 >= 0) {
                        crack.push(current_crack_point + Point(-1, 0));
                        new_crack.push(new_current_crack_point + Point(-1, 0));
                    }
                case 6:
                    if (imageROI_E.ptr(current_crack_point.y + 1)[current_crack_point.x - 1] == 255 && new_current_crack_point.x - 1 >= 0 && new_current_crack_point.y + 1 < srcROI.rows) {
                        crack.push(current_crack_point + Point(-1, 1));
                        new_crack.push(new_current_crack_point + Point(-1, 1));
                    }
                case 7:
                    if (imageROI_E.ptr(current_crack_point.y + 1)[current_crack_point.x] == 255 && new_current_crack_point.y + 1 < srcROI.rows) {
                        crack.push(current_crack_point + Point(0, 1));
                        new_crack.push(new_current_crack_point + Point(0, 1));
                    }
                case 8:
                    if (imageROI_E.ptr(current_crack_point.y + 1)[current_crack_point.x + 1] == 255 && new_current_crack_point.y + 1 < srcROI.rows && new_current_crack_point.x + 1 < srcROI.cols) {
                        crack.push(current_crack_point + Point(1, 1));
                        new_crack.push(new_current_crack_point + Point(1, 1));
                    }
                default:
                    break;
                }
            }
        }
        srcROI(Range(1, imageROI.rows + 1), Range(1, imageROI.cols + 1)).copyTo(srcImg(Range(X1.y, X2.y + 1), Range(X1.x, X2.x + 1)));
    }




    Mat lookUpTable(1, 256, CV_8U, Scalar(0));
    lookUpTable.at<uchar>(0, 255) = 255;
    LUT(srcImg, lookUpTable, srcImg);

    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));// œá¹¹ÔªËØ3*3
    morphologyEx(srcImg, srcImg, CV_MOP_CLOSE, kernel);//±Õ
    dilate(srcImg, srcImg, kernel);//ÅòÕÍ

    dilate(edge_copy, edge_copy, kernel, cv::Point(-1,-1),  20);//ÅòÕÍ

    findConnectedDomain(edge_copy, connectDomains, 10, 10000, 1.2, DomainsWidth);


    zhang_thinimage_improve(srcImg);
    namedWindow("1",WINDOW_NORMAL);
    imshow("1",srcImg);

    //ÕÒµœËùÓÐµÄ¶Ëµã
    Mat srcImg_1(srcImg.rows + 2, srcImg.cols + 2, srcImg.type(), Scalar(0));
    srcImg.copyTo(srcImg_1(Range(1, srcImg_1.rows-1), Range(1, srcImg_1.cols-1)));
    vector<Point> EndPoint;
    FindEndpoint(srcImg_1, EndPoint);
    //ŽÓµÚÒ»žö¶Ëµã¿ªÊŒÑ°ÕÒ32ÁÚÓòµÄÆäËû¶Ëµã£¬²¢œøÐÐÏàÁ¬




    /*
    //Éè¶šÒ»žöãÐÖµth£¬ÅÐ¶ÏÊÇ·ñÊÇÍ¬Ò»¶ÎÁÑ·ì
    double th=5.0;
    //label_flag±íÊŸ±êÇ©Öµ
    int label_flag = 1;

    for (auto ptr = Rect_X1_X2.begin(); ptr != Rect_X1_X2.end(); ++ptr) {
        //ÎªŽËÁªÍšÓòž³³õÊŒLabelÖµ
        if (label[ptr - Rect_X1_X2.begin()] >= label_flag) {
            label[ptr - Rect_X1_X2.begin()] = label_flag;
            label_flag = label_flag + 1;
        }
        for (auto ctr = ptr + 1; ctr != Rect_X1_X2.end(); ++ctr) {
            //ŒÆËãËÄžöµãÖ®ŒäµÄŸàÀë£¬²¢ÅÐ¶ÏÊÇ·ñÐ¡ÓÚãÐÖµ
            Point X1 = (*ptr)[0];
            Point X2 = (*ptr)[1];
            Point Y1 = (*ctr)[0];
            Point Y2 = (*ctr)[1];
            double dis_X1_Y1 = sqrt((X1.x - Y1.x) ^ 2 + (X1.y - Y1.y) ^ 2);
            double dis_X1_Y2 = sqrt((X1.x - Y2.x) ^ 2 + (X1.y - Y2.y) ^ 2);
            double dis_X2_Y1 = sqrt((X2.x - Y1.x) ^ 2 + (X2.y - Y1.y) ^ 2);
            double dis_X2_Y2 = sqrt((X2.x - Y2.x) ^ 2 + (X2.y - Y2.y) ^ 2);

            vector<int>  label_ctr;
            if (dis_X1_Y1 < th || dis_X1_Y2 < th || dis_X2_Y1 < th || dis_X2_Y2 < th) {
                //¶ÔÂú×ããÐÖµÒªÇóµÄÁÑ·ìÈÏÎªÊÇÍ¬Ò»ÌõÁÑ·ì£¬label¶šÎªÏàÍ¬µÄ
                label_ctr.push_back(label[ctr - Rect_X1_X2.begin()]);
                label[ctr - Rect_X1_X2.begin()] = label[ptr - Rect_X1_X2.begin()];
            }
        }
    }
    */

    Mat s = Mat(srcImg.rows, srcImg.cols, CV_8UC3, Scalar(0, 0, 0));

    connectDomains.clear();  DomainsWidth.clear();

    findConnectedDomain(srcImg, connectDomains, 0, 10, 0, DomainsWidth);
    RNG rng;		//²úÉúËæ»úÖÖ×Ó£¬ŽÓ¶ø²úÉúËæ»úÑÕÉ«
    string length;

    for (auto ctr = connectDomains.begin(); ctr != connectDomains.end(); ++ctr) {
        length = to_string(ctr->size());		//œ«³€¶È×ªÎª×Ö·ûŽ®ÀàÐÍ
        //cout << endl << "µÚ" << ctr - connectDomains.begin() + 1 << "ÌõÁÑ·ì³€¶ÈÎª£º" << ctr->size() << endl;
        //Ëæ»úÑÕÉ«
        int r = rng.uniform(0, 255);	int g = rng.uniform(0, 255);	int b = rng.uniform(0, 255);
        //³€¶È¿òµÄ×óÏÂœÇÎ»ÖÃ
        Point origin;
        //¶ÔÃ¿Ò»žöÁªÍšÓò¶šÒåÒ»žötxtÎÄŒþ
        //ofstream outfile;
        //string FileName;
        //FileName = length + ".txt";
        //outfile.open(FileName, ostream::app);
        //¶ÔÃ¿Ò»žöÁªÍšÓòÖÐµÄµãœøÐÐŽŠÀí
        for (auto ptr = ctr->begin() + 1; ptr != ctr->end(); ++ptr) {
            s.at<Vec3b>(ptr->y, ptr->x)[0] = b;
            s.at<Vec3b>(ptr->y, ptr->x)[1] = g;
            s.at<Vec3b>(ptr->y, ptr->x)[2] = r;
            //ŒÆËã³€¶È¿ò·ÅµÄÎ»ÖÃ
            origin.x += ptr->x;
            origin.y += ptr->y;
            Point last_point = *(ptr - 1);
            Point this_point = *ptr;
            //ŒÆËã¿í¶È
            //float width = 10;
            //findwidth(edge, last_point, this_point, width);
            //cout << this_point << " " << ptr - ctr->begin() << " " << width << endl;

            //œ«¿í¶ÈÖµÊä³öµœtxtÎÄŒþ
            //outfile << width << endl;

        }
        origin.x = origin.x / ctr->size();
        origin.y = origin.y / ctr->size();
        putText(s, length, origin, FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(b, g, r), 1, 8, 0);
        //outfile.close();
    }

    namedWindow("2",WINDOW_NORMAL);
    imshow("2",s);

    Mat output3(srcImg.rows, srcImg.cols, CV_8UC3, Scalar(0, 0, 255));
    Mat output2(srcImg.rows, srcImg.cols, CV_8UC3, Scalar(0, 255, 0));

    edge.convertTo(edge, CV_8UC1, -1.0, 1);
    srcImg.convertTo(srcImg, CV_8UC1, -1.0, 1);
    srcImg_clone.copyTo(output3, edge);
    output3.copyTo(output2, srcImg);

    waitKey(-1);
    return 0;




}
