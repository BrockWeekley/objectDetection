#include <stdio.h>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv )
{
    if ( argc != 5 )
    {
        printf("usage: ./rgb2gray <Image_Path> <Output_Image_Path> <Threshold_Value> <Input_Ground_Truth_Image> \n");
        return -1;
    }
    cv::Mat image = cv::imread( argv[1], cv::IMREAD_GRAYSCALE);
    int rows, cols;
    rows = image.size[0];
    cols = image.size[1];
    
    cv::Mat comparison = cv::imread( argv[4], 1 );

    cv::Mat comparisonResults;
    comparisonResults.create(image.size(), CV_8UC1);
    
    int thresh = atoi(argv[3]);
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    
    double truePositive = 0;
    double trueNegative = 0;
    double falsePositive = 0;
    double falseNegative = 0;
    
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (image.at<uchar>(row, col) < thresh) {
                image.at<uchar>(row, col) = 0;
            } else {
                image.at<uchar>(row, col) = 255;
            }
            if (image.at<uchar>(row, col) == 255 && comparison.at<uchar>(row, col) == 255) {
                comparisonResults.at<uchar>(row, col) = 1;
                truePositive += 1;
            }
            if (image.at<uchar>(row, col) == 0 && comparison.at<uchar>(row, col) == 0) {
                comparisonResults.at<uchar>(row, col) = 2;
                trueNegative += 1;
            }
            if (image.at<uchar>(row, col) == 255 && comparison.at<uchar>(row, col) == 0) {
                comparisonResults.at<uchar>(row, col) = 3;
                falsePositive += 1;
            }
            if (image.at<uchar>(row, col) == 0 && comparison.at<uchar>(row, col) == 255) {
                comparisonResults.at<uchar>(row, col) = 4;
                falseNegative += 1;
            }
        }
    }
    
//    TP (1) = both values are one (or non-zero)  (one in the computed binary image AND one in the ground truth image)
//
//    TN (2) = both values are zero (zero in the computed binary image AND zero in the ground truth image)
//
//    FP (3) = one (or non-zero) in the computed binary image AND zero in the ground truth image
//
//    FN (4) = zero in the computer binary image AND one (non-zero) in the ground truth image
    
    printf("True Positive Matches: %.0f\n", truePositive);
    printf("True Negative Matches: %.0f\n", trueNegative);
    printf("False Positive Matches: %.0f\n", falsePositive);
    printf("False Negative Matches: %.0f\n", falseNegative);
    printf("Total Percentage Match: %.2f%%\n", (((truePositive + trueNegative)/(truePositive + trueNegative + falsePositive + falseNegative))) * 100);
    cv::imwrite(argv[2], image);
    cv::imshow("Display Image", image);
    cv::waitKey(0);
    return 0;
}
