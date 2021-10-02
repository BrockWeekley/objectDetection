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

int find(int currentLabel, int parent []) {
    while (parent[currentLabel] != 0) {
//        printf("\nLoop Value: %d", parent[currentLabel]);
        currentLabel = parent[currentLabel];
    }
    return currentLabel;
}

void union_labels(int firstLabel, int secondLabel, int parent []) {
    int foundFirst = find(firstLabel, parent);
    int foundSecond = find(secondLabel, parent);
    if (foundFirst != foundSecond) {
        parent[foundSecond] = foundFirst;
    }
}


int main(int argc, char** argv )
{
    if ( argc < 3 || argc > 6 )
    {
        printf("usage: ./CCL [-invert] <input_image> <output_image> \n");
        printf("or: ./CCL [-invert] -size <number> <input_image> <output_image>");
        return -1;
    }
    srand(time(0));
    
    bool invert = false;
    
    if (argc == 4 || argc == 6) {
        // Invert flag used
        invert = true;
    }
    cv::Mat image;
    char* arguments [argc];
    arguments[0] = argv[0];
    if (invert) {
        int i;
        for (i = 1; i < argc - 1; i++) {
            arguments[i] = argv[i + 1];
            
        }
        image = cv::imread(arguments[argc - 3], 0);
        printf("\n\n%s\n\n", arguments[argc - 3]);
    } else {
        int i;
        for (i = 1; i < argc; i++) {
            arguments[i] = argv[i];
        }
        image = cv::imread(arguments[argc - 2], 0);
        printf("\n\n%s\n\n", arguments[argc - 2]);
    }


    if (image.channels() > 2) {
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    }
    
    cv::threshold(image, image, 125, 255, cv::THRESH_BINARY);
    
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

    printf("Height of Image: %d, Width of Image: %d", height, width);
    
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (invert) {
                if (image.at<uchar>(row, col) == 255) {
                    image.at<uchar>(row, col) = 0;
                    cv::Vec3b pixel = newImage.at<cv::Vec3b>(row, col);
                    pixel[0] = 0;
                    pixel[1] = 0;
                    pixel[2] = 0;
                    newImage.at<cv::Vec3b>(row, col) = pixel;
                } else {
                    image.at<uchar>(row, col) = 255;
                    cv::Vec3b pixel = newImage.at<cv::Vec3b>(row, col);
                    pixel[0] = 255;
                    pixel[1] = 255;
                    pixel[2] = 255;
                    newImage.at<cv::Vec3b>(row, col) = pixel;
                }
            }
        }
    }
    
    // Initialize all labels to 0
    int labels [height][width];
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            labels[row][col] = 0;
        }
    }

    int parent [2000];
    for (int i = 0; i < 2000; i++) {
        parent[i] = 0;
    }
    int label = 1;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // e is the current pixel
            // 8 connected structure is:
            // a b c
            // d e
            int e = image.at<uchar>(row, col);
            if (e == 255) {
                int d = 0;
                int a = 0;
                int b = 0;
                int c = 0;
                if (col - 1 >= 0 && image.at<uchar>(row, col - 1) == 255) {
                    d = 1;
                }
                if (row - 1 >= 0 && col - 1 >= 0 && image.at<uchar>(row - 1, col - 1) == 255) {
                    a = 1;
                }
                if (row - 1 >= 0 && image.at<uchar>(row - 1, col) == 255) {
                    b = 1;
                }
                if (row - 1 >= 0 && col + 1 <= width && image.at<uchar>(row - 1, col + 1) == 255) {
                    c = 1;
                }
                
                int dLabel = labels[row][col - 1];
                int aLabel = labels[row - 1][col - 1];
                int bLabel = labels[row - 1][col];
                int cLabel = labels[row - 1][col + 1];
                
                // Decision tree 8-connected
                if (b == 1) {
                    // 0 1 0
                    // 0 b
                    // or
                    // 0 1 0
                    // 0 b
                    // or
                    // 0 1 1
                    // 0 b
                    labels[row][col] = bLabel;
                } else if (c == 1) {
                    if (a == 1) {
                        // 1 0 1
                        // 0 a
                        // Union(a, c)
                        labels[row][col] = cLabel;
                        union_labels(
                                     aLabel,
                                     cLabel,
                                     parent
                                     );
                    } else if (d == 1) {
                        // 0 0 1
                        // 1 c
                        // Union(c, d)
                        labels[row][col] = cLabel;
                        union_labels(
                                     cLabel,
                                     dLabel,
                                     parent
                                     );
                    } else {
                        // 0 0 1
                        // 0 c
                        labels[row][col] = cLabel;
                    }
                } else if (a == 1) {
                    // 1 0 0
                    // 0 a
                    labels[row][col] = aLabel;
                } else if (d == 1) {
                    // 0 0 0
                    // 1 d
                    labels[row][col] = dLabel;
                } else {
                    labels[row][col] = label;
                    label++;
                }
            }
//            if (image.at<uchar>(row, col) == 255) {
//                cv::Vec3b color = newImage.at<cv::Vec3b>(row, col);
//                Color randomColor = random_color();
//                printf("\n Blue: %d, Green: %d, Red: %d", randomColor.blue, randomColor.green, randomColor.red);
//                // B
//                color[0] = randomColor.blue;
//                // G
//                color[1] = randomColor.green;
//                // R
//                color[2] = randomColor.red;
//                newImage.at<cv::Vec3b>(row, col) = color;
//            }
        }
    }
    
//    for (int row = 0; row < height; row++) {
//        printf("\n");
//        for (int col = 0; col < width; col++) {
//            printf("%d ", labels[row][col]);
//        }
//    }
    
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (labels[row][col] != 0) {
                labels[row][col] = find(labels[row][col], parent);
            }
        }
    }
    
    Color labelColor [label];
    labelColor[0] = {0, 0, 0};
    for (int currentLabel = 1; currentLabel <= label; currentLabel++) {
        labelColor[currentLabel] = random_color();
    }
    
    int objectAreas [label];
    objectAreas[0] = 0;
    for (int currentLabel = 1; currentLabel <= label; currentLabel++) {
        objectAreas[currentLabel] = 0;
    }
    // Loop through labels array to apply colors
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            for (int currentLabel = 1; currentLabel <= label; currentLabel++) {
                if (labels[row][col] == currentLabel) {
                    cv::Vec3b color = newImage.at<cv::Vec3b>(row, col);
                    Color randomColor = labelColor[currentLabel];
                    // B
                    color[0] = randomColor.blue;
                    // G
                    color[1] = randomColor.green;
                    // R
                    color[2] = randomColor.red;
                    newImage.at<cv::Vec3b>(row, col) = color;
                    
                    objectAreas[currentLabel]++;
                }
            }
        }
    }
    
//    for (int currentLabel = 1; currentLabel <= label; currentLabel++) {
//        printf("\nObject Area: %d", objectAreas[currentLabel]);
//    }
    
    cv::imshow("Display", newImage);
    cv::waitKey(0);
    
    if (argc > 4) {
        // Also do part C
        int maxArea = atoi(arguments[2]);
        for (int currentLabel = 1; currentLabel <= label; currentLabel++) {
            if (objectAreas[currentLabel] < maxArea) {
                for (int row = 0; row < height; row++) {
                    for (int col = 0; col < width; col++) {
                        if (labels[row][col] == currentLabel) {
                            cv::Vec3b color = newImage.at<cv::Vec3b>(row, col);
                            // B
                            color[0] = 0;
                            // G
                            color[1] = 0;
                            // R
                            color[2] = 0;
                            newImage.at<cv::Vec3b>(row, col) = color;
                        }
                    }
                }
            }
        }
        cv::imshow("Display", newImage);
        cv::waitKey(0);
    }
    int objectCount = 0;
    std::vector<int> parentVec;

    for (int i = 1; i < 2000; i++) {
        printf("\nParent item: %d", parent[i]);
        parentVec.push_back(parent[i]);
    }
    std::sort(parentVec.begin(), parentVec.end());
    parentVec.erase(unique(parentVec.begin(), parentVec.end() ), parentVec.end());
    objectCount = parentVec.size();
        
    printf("\nNumber of objects: %d\n", objectCount);
    printf("IDs used during first pass: %d\n", label);
}
