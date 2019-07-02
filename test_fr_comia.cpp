#include <iostream>

static void help_fr_comia()
{
    std::cout << "\n****** Workshop DeepLearning 2019 ******\n\n"
              << "- Face detection\n"
              << "- Landmark detection and face alignment\n"
              << "- Face descriptor computation\n";
    std::cout << "\nPress any key to exit" << std::endl
              << std::endl;
}

const char* keys_fr_comia = 
{
    "{image | ../../resources/bald_guys.jpg}"
    "{cascade | ../../resources/haarcascade_frontalface_default.xml}"
    "{shape_predictor | ../../shape_predictor_68_face_landmarks.dat}"
    "{res_net | ../../resources/dlib_face_recognition_resnet_model_v1.dat}"
}

int main(int argc, const char * argv[])
{
    help_fr_comia();

    
}