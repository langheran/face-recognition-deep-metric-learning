// #include <iostream>
// #include <string>

// #include <opencv2/opencv.hpp>
// #include <opencv2/core/utility.hpp>

#include <opencv2/opencv.hpp>
#include <string>

#include <dlib/dnn.h>
#include <dlib/image_io.h>
#include <dlib/misc_api.h>
#include <dlib/opencv.h>

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
        "{image|../../resources/bald_guys.jpg}"
        "{cascade|../../resources/haarcascade_frontalface_default.xml}"
        "{shape_predictor|../../shape_predictor_68_face_landmarks.dat}"
        "{res_net|../../resources/dlib_face_recognition_resnet_model_v1.dat}"};

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