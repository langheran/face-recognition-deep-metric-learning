//
//  FaceDescriptor_dking.cpp
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

#include "FaceDescriptor_dking.h"


#include <dlib/gui_widgets.h>
#include <dlib/image_processing/render_face_detections.h>

using namespace std;
using namespace dlib;


FaceDescriptor_dking::FaceDescriptor_dking(std::string resnet_file, std::string shape_file)
{
    //-- Load deep face model
    dlib::deserialize(resnet_file) >> net;
    
    //-- Load face detector model
    detector = dlib::get_frontal_face_detector();
    
    //-- Load face landmarks detection model
    dlib::deserialize(shape_file) >> sp;    
}

matrix<rgb_pixel> FaceDescriptor_dking::get_lfw_face_chip (
                                     const matrix<rgb_pixel>& img,
                                     const rectangle& rect,
                                     frontal_face_detector& detector,
                                     shape_predictor& sp
                                     )
{
    rectangle best_det;
    double best_overlap = 0;
    // The face landmarking works better if the box is aligned as dlib's frontal face
    // detector would align it.  So try to report a face box that uses the detector, but if
    // one can't be found then just the default box.
    for (auto det : detector(img,-0.9))
    {
        auto overlap = box_intersection_over_union(rect, det);
        if (overlap > best_overlap)
        {
            best_det = det;
            best_overlap = overlap;
        }
    }
    
    if (best_overlap < 0.3)
    {
        best_det = rect;// centered_rect(get_rect(img),100,100);
    }
        
    auto shape = sp(img, best_det);

    matrix<rgb_pixel> face_chip;
    extract_image_chip(img, get_face_chip_details(shape,150,0.25), face_chip);
    
    return face_chip;
}


std::vector<matrix<rgb_pixel>> FaceDescriptor_dking::jitter_image(const matrix<rgb_pixel>& img)
{
    std::vector<matrix<rgb_pixel>> crops;
    // don't jitter
    // crops.push_back(img); return crops;
    
    
    thread_local random_cropper cropper;
    cropper.set_chip_dims(150,150);
    cropper.set_randomly_flip(true);
   cropper.set_max_object_size(0.99999);
    // cropper.set_max_object_height(0.99999);
    cropper.set_background_crops_fraction(0);
   cropper.set_min_object_size(0.97*150, 0.97*150);
    // cropper.set_min_object_height(0.97);
    cropper.set_translate_amount(0.02);
    cropper.set_max_rotation_degrees(3);
    
    std::vector<mmod_rect> raw_boxes(1), ignored_crop_boxes;
    raw_boxes[0] = shrink_rect(get_rect(img),3);
    
    matrix<rgb_pixel> temp;
    for (int i = 0; i < 100; ++i)
    {
        cropper(img, raw_boxes, temp, ignored_crop_boxes);
        crops.push_back(move(temp));
    }
    return crops;
}

matrix<float> FaceDescriptor_dking::get_face_descriptor(const matrix<rgb_pixel>& img)
{
    std::vector<matrix<rgb_pixel>> crops;
    crops = jitter_image(img);
    matrix<float> v1 = mean(mat(net(crops)));
    return v1;
}

cv::Mat FaceDescriptor_dking::get_face_descriptor(std::vector<cv::Mat>& img, std::vector<cv::Rect> facerect, bool do_jitter, bool do_align)
{
    // Load image from file
    std::vector<matrix<rgb_pixel>> crops;
    
    for (int i = 0; i < (int)img.size(); i++) {
        
        cv_image<bgr_pixel> image(img[i]);
        matrix<rgb_pixel> matrix, raw_image;
        assign_image(matrix, image);
        
        // Normalize face using dlib face and landmark detection
        if (do_align) {
            int rx = facerect[i].x;
            int ry = facerect[i].y;
            int rw = facerect[i].width;
            int rh = facerect[i].height;
            dlib::rectangle r(rx, ry, rx + rw, ry + rh);
            raw_image = get_lfw_face_chip(matrix, r, detector, sp);
        }
        else {
            raw_image = matrix;
        }
        
        // Augment data by rotating, translating and scaling face image
        if (do_jitter) {
            std::vector<dlib::matrix<rgb_pixel> > jit = jitter_image(raw_image);
            crops.insert(crops.end(), jit.begin(), jit.end());
        }
        else
            crops.push_back(raw_image);
    }
    
    // Obtain representation by averaging the obtained descriptors
    matrix<float> v = mean(mat(net(crops)));
    
    // Convert to OpenCV Mat
    cv::Mat vm = dlib::toMat(v).clone();
    return vm;
}


std::vector<cv::Point> FaceDescriptor_dking::extract_image_shape(cv::Mat& img, cv::Rect facerect)
{    
    cv_image<bgr_pixel> image(img);
    matrix<rgb_pixel> matrix;
    assign_image(matrix, image);

    int rx = facerect.x;
    int ry = facerect.y;
    int rw = facerect.width;
    int rh = facerect.height;
    dlib::rectangle r(rx, ry, rx + rw, ry + rh);

    rectangle best_det;
    double best_overlap = 0;
    // The face landmarking works better if the box is aligned as dlib's frontal face
    // detector would align it.  So try to report a face box that uses the detector, but if
    // one can't be found then just the default box.
    for (auto det : detector(matrix,-0.9))
    {
        auto overlap = box_intersection_over_union(r, det);
        if (overlap > best_overlap)
        {
            best_det = det;
            best_overlap = overlap;
        }
    }
    
    if (best_overlap < 0.3)
    {
        best_det = r;
        // best_det = centered_rect(get_rect(matrix),100,100);        
    }    
        
    auto shape = sp(matrix, best_det);

    std::vector<cv::Point> pts;

    for (size_t i = 0; i < shape.num_parts(); i++)
    {
        pts.push_back(cv::Point(cvRound(float(shape.part(i).x())), cvRound(float(shape.part(i).y()))));
    }

    return pts;
}























