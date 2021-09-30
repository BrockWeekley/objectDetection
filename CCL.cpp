#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

struct Color {
    int blue, green, red;
};

Color colors [] = {
    {0, 0, 128},
    {40, 110, 170},
    {0, 128, 128},
    {128, 128, 0},
    {0, 0, 128},
    {75, 25, 230},
    {48, 130, 245},
    {25, 225, 225},
    {60, 245, 210},
    {75, 180, 60},
    {240, 240, 70},
    {200, 130, 0},
    {180, 30, 145},
    {230, 50, 240},
    {128, 128, 128},
    {212, 190, 250},
    {180, 215, 255},
    {200, 250, 255},
    {195, 255, 170},
    {255, 190, 220}
};

Color random_color() {
    int random = rand() % 20;
    return colors[random];
}

int main(int argc, char** argv )
{
    if ( argc != 3 )
    {
        printf("usage: ./CCL [-invert] <input_image> <output_image> \n");
        return -1;
    }
    srand(time(0));
    
    bool invert = false;
    
    if (argc == 4 || argc == 6) {
        // Invert flag used
        invert = true;
    }
    
    char* arguments [argc];
    arguments[0] = argv[0];
    if (invert) {
        int i;
        for (i = 1; i < argc - 1; i++) {
            arguments[i] = argv[i + 1];
            
        }
    } else {
        int i;
        for (i = 1; i < argc; i++) {
            arguments[i] = argv[i];
        }
    }
    
    cv::Mat image = cv::imread(arguments[1], 0);
    if (image.channels() > 2) {
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    }
    
    cv::threshold(image, image, 100, 255, cv::THRESH_BINARY);
    
    int height = image.size[0];
    int width = image.size[1];
    
    cv::Mat newImage(height, width, CV_8UC3);
    cv::Mat planes [3];
    planes[0] = image;
    planes[1] = image;
    planes[2] = image;
    cv::merge(planes, 3, newImage);
    height = newImage.size[0];
    width = newImage.size[1];
    
    std::vector<int> parent;
    
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
//            cv::Vec3b color = newImage.at<cv::Vec3b>(row, col);
//            Color randomColor = random_color();
//            printf("\n Blue: %d, Green: %d, Red: %d", randomColor.blue, randomColor.green, randomColor.red);
//            // B
//            color[0] = randomColor.blue;
//            // G
//            color[1] = randomColor.green;
//            // R
//            color[2] = randomColor.red;
//            newImage.at<cv::Vec3b>(row, col) = color;
        }
    }
    
    cv::imshow("Display", newImage);
    cv::waitKey(0);
    
    if (argc > 5) {
        // Also do part C
    }
}
