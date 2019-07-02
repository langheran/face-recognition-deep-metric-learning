#include <stdio.h>
#include <string>

#include <opencv2/opencv.hpp>

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>

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
        "{image |../../resources/bald_guys.jpg|input image}"
        "{cascade |../../resources/haarcascade_frontalface_default.xml|haar cascade}"
        "{shape_predictor |../../resources/shape_predictor_68_face_landmarks.dat|shape predictor}"
        "{res_net |../../resources/dlib_face_recognition_resnet_model_v1.dat|resnet model}"
    };

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
        vfeed_rz = vfeed.clone();
        if(vfeed.rows > monitor_height)
            cv::resize(vfeed, vfeed_rz, cv::Size(), monitor_height / vfeed.rows, monitor_height / vfeed.rows);
        cv::imshow("Faces", vfeed_rz);
        cv::waitKey();
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