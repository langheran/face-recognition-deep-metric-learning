//
//  FaceDescriptor_dking.h
//  xcode_proj
//
//  Use this library to compute the face descriptor
//  of a given image or video sequence.
//  The implemented descriptor is based on Dlib's
//  deep face descriptor. www.dlib.net
//
//
//
//  Created by LChang on 4/9/18.
//  Copyright © 2018 AAA. All rights reserved.
//

#ifndef FaceDescriptor_dking_h
#define FaceDescriptor_dking_h

#include <stdio.h>
#include <opencv2/opencv.hpp>
// #include "../common/common.h"

#include <dlib/gui_widgets.h>
#include <dlib/clustering.h>
#include <dlib/string.h>

#include <dlib/dnn.h>
#include <dlib/image_io.h>
#include <dlib/misc_api.h>
#include <dlib/opencv.h>

#include <dlib/image_processing/frontal_face_detector.h>

// ----------------------------------------------------------------------------------------

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = dlib::add_prev1<block<N,BN,1,dlib::tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = dlib::add_prev2<dlib::avg_pool<2,2,2,2,dlib::skip1<dlib::tag2<block<N,BN,2,dlib::tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block  = BN<dlib::con<N,3,3,1,1,dlib::relu<BN<dlib::con<N,3,3,stride,stride,SUBNET>>>>>;


template <int N, typename SUBNET> using ares      = dlib::relu<residual<block,N,dlib::affine,SUBNET>>;
template <int N, typename SUBNET> using ares_down = dlib::relu<residual_down<block,N,dlib::affine,SUBNET>>;

// ----------------------------------------------------------------------------------------

template <typename SUBNET> using alevel0 = ares_down<256,SUBNET>;
template <typename SUBNET> using alevel1 = ares<256,ares<256,ares_down<256,SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128,ares<128,ares_down<128,SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64,ares<64,ares<64,ares_down<64,SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32,ares<32,ares<32,SUBNET>>>;

using anet_type = dlib::loss_metric<dlib::fc_no_bias<128,dlib::avg_pool_everything<
alevel0<
alevel1<
alevel2<
alevel3<
alevel4<
dlib::max_pool<3,3,2,2,dlib::relu<dlib::affine<dlib::con<32,7,7,2,2,
dlib::input_rgb_image_sized<150>
>>>>>>>>>>>>;

// ----------------------------------------------------------------------------------------

class FaceDescriptor_dking
{
private:
    //-- deep face model
    anet_type net;
    
    //-- face detector model
    dlib::frontal_face_detector detector;
    
    //-- face landmarks detection model
    dlib::shape_predictor sp;
    
public:
    FaceDescriptor_dking() {};
    FaceDescriptor_dking(std::string resnet_file, std::string shape_file);
    std::vector<dlib::matrix<dlib::rgb_pixel>> jitter_image(const dlib::matrix<dlib::rgb_pixel>& img);
    dlib::matrix<dlib::rgb_pixel> get_lfw_face_chip (
                                                     const dlib::matrix<dlib::rgb_pixel>& img,
                                                     const dlib::rectangle& rect,
                                                     dlib::frontal_face_detector& detector,
                                                     dlib::shape_predictor& sp
                                                     );
    
    dlib::matrix<float> get_face_descriptor(const dlib::matrix<dlib::rgb_pixel>& img);
    cv::Mat get_face_descriptor(std::vector<cv::Mat>& img, std::vector<cv::Rect> facerect, bool do_jitter, bool do_align);

    std::vector<cv::Point> extract_image_shape(cv::Mat& img, cv::Rect facerect);
    
};



#endif /* FaceDescriptor_dking_h */
