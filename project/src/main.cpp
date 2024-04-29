#include "ofMain.h"
#include "Application.h"

// variables du programme
//int image_width = 0;
//int image_height = 0;
//int ray_per_pixel = 0;
int pixel_count = 0;
int image_width = 256;
int image_height = 256;
int ray_per_pixel = 16;

//========================================================================
int main(int argc, char* argv[])
{

    ofGLWindowSettings settings;
    settings.setSize(1024, 768);
    settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

    auto window = ofCreateWindow(settings);

    std::cout << "raytracer start" << std::endl;

    std::cout << "parse program arguments:" << std::endl;

    std::cout << "argc:\t" << argc << std::endl;

    for (int index = 0; index < argc; ++index)
        std::cout << "arg[" << index << "]:\t" << argv[index] << std::endl;

    switch (argc)
    {
    case 1:
        image_width = 256;
        image_height = 256;
        ray_per_pixel = 16;
        break;

    case 2:
        image_width = 256;
        image_height = 256;
        ray_per_pixel = atoi(argv[1]);
        break;

    case 3:
        image_width = atoi(argv[1]);
        image_height = image_width;
        ray_per_pixel = atoi(argv[2]);
        break;

    default:
    case 4:
        image_width = atoi(argv[1]);
        image_height = atoi(argv[2]);
        ray_per_pixel = atoi(argv[3]);
        break;
    }

    std::cout << "program init done" << std::endl;

    ofRunApp(window, make_shared<Application>());
    ofRunMainLoop();
}
