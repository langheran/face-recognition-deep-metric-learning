#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "math.h" 
#include <algorithm>
#include <limits>

void DrawFaceLines(cv::Mat &img, std::vector<cv::Point2f> pts);

void DrawContour(cv::Mat &img, std::vector<cv::Point2f> pts, cv::Scalar color, int thickness, bool closed_contour);

float Dist_L2(cv::Point2f p1, cv::Point2f p2);
float Dist_L2(cv::Point p1, cv::Point p2);

float L2_Dist(cv::Mat &a, int i, int j, float sq_thres);
float L2_Dist_vectorized(cv::Mat &a, int i, int j, float thres);

void ClosestPoints(cv::Rect A, cv::Rect B, cv::Point &ptA, cv::Point &ptB);

#endif
