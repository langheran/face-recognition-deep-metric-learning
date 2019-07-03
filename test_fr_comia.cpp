#include <stdio.h>
#include <string>

#include <opencv2/opencv.hpp>

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>

#include "face/FaceDescriptor_dking.h";
#include "common/common.h";

static void help_fr_comia()
{
    std::cout << "\n****** Workshop DeepLearning 2019 ******\n\n"
              << "- Face detection\n"
              << "- Landmark detection and face alignment\n"
              << "- Face descriptor computation\n";
    std::cout << "\nPress any key to exit" << std::endl
              << std::endl;
}

const char *keys_fr_comia =
    {
        "{image |resources/bald_guys.jpg|input image}"
        "{cascade |resources/haarcascade_frontalface_default.xml|haar cascade}"
        "{shape_predictor |resources/shape_predictor_68_face_landmarks.dat|shape predictor}"
        "{res_net |resources/dlib_face_recognition_resnet_model_v1.dat|resnet model}"
    };

void showImage(cv::Mat& vfeed, cv::Mat& vfeed_rz, float& monitor_height)
{
    vfeed_rz = vfeed.clone();
    if(vfeed.rows > monitor_height)
        cv::resize(vfeed, vfeed_rz, cv::Size(), monitor_height / vfeed.rows, monitor_height / vfeed.rows);
    cv::imshow("Faces", vfeed_rz);
    cv::waitKey();
}

int main(int argc, const char *argv[])
{
    help_fr_comia();
    try
    {
        cv::CommandLineParser parser(argc, argv, keys_fr_comia);
        std::string fimage = parser.get<std::string>("image");
        std::cout << "Loading "
                  << "'" << fimage << "'"
                  << " image...";
        cv::Mat image = cv::imread(fimage, cv::IMREAD_COLOR);
        if(image.empty())
        {
            std::cout << "ERROR in " << fimage << std::endl;
            std::cin.get();
            return -1;
        }
        else
            std::cout << "DONE" << std::endl;
        cv::Mat vfeed = image.clone();
        cv::Mat vfeed_rz;

        //-- convert  to grayscale
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        cv::namedWindow("Faces");
        float monitor_height = 580;
        showImage(vfeed, vfeed_rz, monitor_height);

        ///////////////////////////////////////////////////////
        // STEP 1: Detect faces
        ///////////////////////////////////////////////////////

        //-- Create and load detector
        std::cout << "Detecting faces...";
        fflush(stdout);
        cv::CascadeClassifier cascade;
        std::string fcascade = parser.get<std::string>("cascade");
        if(!cascade.load(fcascade))
        {
            std::cout << "ERROR loading: " << fcascade << std::endl;
            std::cin.get();
            return -1;
        }
        //-- Detect faces
        std::vector<cv::Rect> faces;
        cascade.detectMultiScale(gray, faces, 1.2, 2, 0 
        |cv::CASCADE_SCALE_IMAGE
            // | cv::CASCADE_DO_ROUGH_SEARCH
            // | cv::CASCADE_FIND_BIGGEST_OBJECT
        );
        //-- Visual feedback, draw face bounding box
        for (int i = 0; i < (int)faces.size(); i++) 
        {
            cv::rectangle(vfeed, faces[i], cv::Scalar(rand() % 256, rand() % 256, rand() % 256, 3));
        }

        std::cout << faces.size() << " faces detected" << std::endl;
        showImage(vfeed, vfeed_rz, monitor_height);

        ///////////////////////////////////////////////////////
        // STEP 1: Compute face descriptor
        ///////////////////////////////////////////////////////
        std::cout << "Compuuting face descriptors...";
        std::fflush(stdout);
        //-- Create and load keypoints detector
        std::string fshape_predictor = parser.get<std::string>("shape_predictor");
        dlib::shape_predictor pose_model;
        dlib::deserialize(fshape_predictor) >> pose_model;

        //-- Create and load face descriptor
        std::string fres_net = parser.get<std::string>("res_net");
        FaceDescriptor_dking fd(fres_net, fshape_predictor);

        //-- Create Mat for face descriptors
        cv::Mat facedescrs;

        //-- For each face find keypoints
        for (int i = 0; i < (int)faces.size(); i++)
        {
            std::cout << "\rComputing face descriptors..." << i + 1 << " of " << faces.size();
            std::fflush(stdout);
            ///////////////////////////////////////////////////////
            // STEP 2.1: Detect landmarks
            ///////////////////////////////////////////////////////

            //-- Cast between cv and dlib
            dlib::rectangle dlib_face_i(faces[i].x, faces[i].y, faces[i].br().x, faces[i].br().y);
            dlib::cv_image<unsigned char> cimg(gray);

            //-- Find keypoints
            dlib::full_object_detection shape = pose_model(cimg, dlib_face_i);

            //-- Visual feedback, draw keypoints
            std::vector<cv::Point2f> pts;
            for (int i = 0; i < (int)shape.num_parts(); i++)
            {
                cv::Point2f pt((float)shape.part(i).x(), (float)shape.part(i).y());
                pts.push_back(pt);
            }
            DrawFaceLines(vfeed, pts);
            showImage(vfeed, vfeed_rz, monitor_height);
        }
    }
    catch (cv::Exception &e)
    {
        std::cerr << e.msg << std::endl; // output exception message
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << '\n';
    }
    catch (const std::string &ex)
    {
        std::cerr << ex << '\n';
    }
    catch (...)
    {
        std::cerr << "Error" << '\n';
    }

    fflush(stdout);
    std::cin.get();
}