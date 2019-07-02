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
        "{shape_predictor |../../shape_predictor_68_face_landmarks.dat}|shape predictor}"
        "{res_net |../../resources/dlib_face_recognition_resnet_model_v1.dat}|resnet model}"};

int main(int argc, const char *argv[])
{
    help_fr_comia();
    cv::CommandLineParser parser(argc, argv, keys_fr_comia);
    std::string fimage = parser.get<std::string>("image");
    std::cout << "Loading "
              << "'" << fimage << "'"
              << " image...";
    fflush(stdout);
    std::cin.get();
}