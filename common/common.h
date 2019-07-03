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

std::vector<std::string> ReadFileLinesAsString(std::string filename);

int BuildPath(std::string basePath, std::string namePrefix, std::string name, std::string ext, std::string &outputPath);

int LoadVideo(std::string videoPath, std::vector<cv::Mat> &video, std::vector<std::pair<cv::Point2f, cv::Point2f> > &eyes_coords, std::vector<std::string> &frames_names, int max_count);

int SaveDescrAsText(std::string path, std::vector<float> descr, int dim_desc);
int SaveDescrAsText(std::string path, std::vector<float> descr);
int SaveDescrAsTextInRow(std::string path, std::vector<float> descr, std::ios_base::openmode mode = std::ios_base::out);

int SaveDescrBinary(std::string path, std::vector<float> descr, int dim_desc);

void WriteMatToFile(std::ofstream &f, cv::Mat &m);

int LoadDescrBinary(std::string path, std::vector<float> &descr, int size);

int LoadDescrBinaryInfo(std::string path, int &size);

cv::Mat LoadFeaturesFromTextFile(std::string filename);

int GetNumberOfLinesInTextFile(std::string path);

std::vector<std::string> &split_string(const std::string &s, char delim, std::vector<std::string> &elems);

void replaceAll(std::string &str, const std::string &from, const std::string &to);

//fbecerra
void draw_points(cv::Mat img, std::vector<cv::Point2f> points);

//fbecerra 
void replace_last(std::string &str, const std::string &from, const std::string &to);

//fbecerra 
std::string get_extension(const std::string &file_path);

//fbecerra
void save_filter_mask(const std::string filter_mask_path, const cv::Mat filter_mask, const std::string ID);

//fbecerra
int load_filter_mask(const std::string filter_mask_path, cv::Mat& filter_mask, const std::string ID);

//fbecerra
int load_video(const std::string video_path, std::vector<cv::Mat> &video, std::vector<cv::Mat> &masks, std::vector<std::pair<cv::Point2f, cv::Point2f> > &eyes_coords, std::vector<std::string> &frames_names, int max_count, const bool load_masks = false);

//fbecerra
int get_files(const std::string folder_path, const std::string extension, std::vector<std::string> &files, int max_number = INT_MAX, const bool deeper_search = true);

std::string to_string_(int value);

int LoadTextFile(std::string path, std::vector<float> &output);

int ComputeDistance(std::vector<float> x, std::vector<float> y, int type_dist, float &dist);

int ComputeMaxScales(cv::Size sz, int patch_size, float featureScaleMul);

float* convertImagetoFloat(const cv::Mat& img);
float* convertImagetoFloat2(const cv::Mat& img);
float* convertImagetoFloat2_CV32F(const cv::Mat& img);
float* convertImagetoFloat3(const cv::Mat& img);

void ComputeNormL2(const std::vector<float> &descr_denseSift, int dim_descr, std::vector<float> &feat_norm);

//Leyanis
//method used in hatDetection and shoulderAlignment
cv::Mat EstandarizeImg(cv::Mat& img, int target_heght);

//Leyanis
double NormalizeValue_between_01(double value, double middle, double deviation);
//Bouza
double CalculateScore(double min, double max, double thresh, double value);//between 01 

//Leyanis from Janus_FacePointsDetector
std::vector<cv::Point2f> GetPointEyes(std::vector<cv::Point2f> obj);

//Leyanis from Janus_FacePointsDetector
std::vector<cv::Point2f> GetCentroEyes(std::vector<cv::Point2f> a);

//Leyanis from Janus_FacePointsDetector
std::vector<cv::Point2f> GetPointMouth(std::vector<cv::Point2f> obj);

void DrawLabelToRectangle(cv::Mat &img, cv::Rect r, std::string text, cv::Scalar text_color, cv::Scalar bg_color, double fontScale = 1.0, int thickness = 1);

float IntersectionOverUnionRatio(cv::Rect a, cv::Rect b);

void DrawFaceLines(cv::Mat &img, std::vector<cv::Point2f> pts);

void DrawContour(cv::Mat &img, std::vector<cv::Point2f> pts, cv::Scalar color, int thickness, bool closed_contour);

std::vector<float> Mat2VectorFloat(cv::Mat mat);

void GetInterval(int size, int block_count, int block_idx, int &block_init, int &block_end);

std::vector<cv::Mat> my_jitter_img(cv::Mat im, int count,
                          float min_scale, float max_scale,
                          float min_rotation, float max_rotation,
                          float min_translation, float max_translation);

float Dist_L2(cv::Point2f p1, cv::Point2f p2);
float Dist_L2(cv::Point p1, cv::Point p2);

float L2_Dist(cv::Mat &a, int i, int j, float sq_thres);
float L2_Dist_vectorized(cv::Mat &a, int i, int j, float thres);

void ClosestPoints(cv::Rect A, cv::Rect B, cv::Point &ptA, cv::Point &ptB);

double AngleBetweenTwoVectors(cv::Point2f p1, cv::Point2f p2);

double AngleBetweenTwoVectors(cv::Point p1, cv::Point p2);

double AngleBetweenTwoVectors_orderless(cv::Point2f p1, cv::Point2f p2);

double AngleBetweenVectorAndXAxis(cv::Point p1);

double AngleBetweenVectorAndXAxis2(cv::Point p1);

cv::Point TransformedPoint(cv::Point p, cv::Point origin, cv::Point traslation, float angle, float scale);

cv::Point2f NormalizeVector(cv::Point2f v);

cv::Point2f NormalizeVector(cv::Point v);

cv::Point2f NormalizeVector(cv::Point v, float &magnitude);

cv::Point2f VectorComplement(cv::Point2f v);

cv::Mat JoinImages(cv::Mat im1, cv::Mat im2, int orientation /* 0:horizontal, 1:vertical*/);

#endif
