#pragma once

#include <cstdio>
#include <iostream>
#include <queue>
#include <execinfo.h>
#include <csignal>

#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>

#include "camodocal/camera_models/CameraFactory.h"
#include "camodocal/camera_models/CataCamera.h"
#include "camodocal/camera_models/PinholeCamera.h"

#include "parameters.h"
#include "tic_toc.h"

using namespace std;
using namespace camodocal;
using namespace Eigen;

bool inBorder(const cv::Point2f &pt);

void reduceVector(vector<cv::Point2f> &v, vector<uchar> status);
void reduceVector(vector<int> &v, vector<uchar> status);

class FeatureTracker
{
  public:
    FeatureTracker();

    void readImage(const cv::Mat &_img,double _cur_time);

    void setMask();

    void addPoints();

    bool updateID(unsigned int i);

    void readIntrinsicParameter(const string &calib_file);

    void showUndistortion(const string &name);

    void rejectWithF();

    void undistortedPoints();

    cv::Mat mask;     //图像掩码
    cv::Mat fisheye_mask;       //鱼眼相机mask，用来除去边缘噪点
    cv::Mat prev_img, cur_img, forw_img;  //是上上次发布的帧的图像数据/光流跟踪上一帧的图像数据/当前的图像数据
    vector<cv::Point2f> n_pts;  //每一帧中新提取的特征点
    vector<cv::Point2f> prev_pts, cur_pts, forw_pts;    //对应的图像特征点
    vector<cv::Point2f> prev_un_pts, cur_un_pts;    //归一化相机坐标系下的坐标
    vector<cv::Point2f> pts_velocity;   //当前帧相对前一帧特征点沿x,y方向的像素移动速
    vector<int> ids;    //能够被跟踪到的特征点的id
    vector<int> track_cnt;  //当前帧forw_img中每个特征点被追踪的时间次数
                            //track_cnt不需要传递的原因，每次只会维护同一组数据，每次都会有有数据被筛选调，然后新的光流特征点被筛选出来
    map<int, cv::Point2f> cur_un_pts_map;   //构建id与归一化坐标的id，见undistortedPoints()
    map<int, cv::Point2f> prev_un_pts_map;
    camodocal::CameraPtr m_camera;
    double cur_time;
    double prev_time;

    static int n_id;    //用来作为特征点id，每检测到一个新的特征点，就将++n_id作为该特征点
};
