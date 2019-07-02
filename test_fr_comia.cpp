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
    "{cascade | ../../resources/bald_guys.jpg}"
    "{shape_predictor | ../../resources/bald_guys.jpg}"
    "{res_net | ../../resources/bald_guys.jpg}"

}
