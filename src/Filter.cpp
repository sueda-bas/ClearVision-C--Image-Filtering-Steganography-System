#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <ostream>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // 1. Copy the original image for reference.
    GrayscaleImage originalImage = GrayscaleImage(image);
    int countOfRows = (kernelSize - 1) / 2;

    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.
    for (int i = 0; i < originalImage.get_height(); i++) {
        for(int j = 0; j < originalImage.get_width(); j++) {
            int sum = 0;
            for(int r = i-countOfRows; r <= i+countOfRows; r++) {
                for(int c = j-countOfRows; c <= j+countOfRows; c++) {
                    if (r<0 || r>=originalImage.get_height() || c<0 || c>=originalImage.get_width()) {
                        sum += 0;
                    }else {
                        sum += originalImage.get_pixel(r,c);
                    }
                }
            }
            int mean = sum / (kernelSize*kernelSize);
            image.set_pixel(i, j, mean);
        }
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    GrayscaleImage originalImage = GrayscaleImage(image);

    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    double** kernel = new double*[kernelSize];
    for (int i = 0; i < kernelSize; i++) {
        kernel[i] = new double[kernelSize];
    }
    int countOfRows =( kernelSize - 1 )/ 2;
    double sum = 0;
    for (int i = -countOfRows; i <= countOfRows; i++) {
        for (int j = -countOfRows; j <= countOfRows; j++) {
            double coefficient = 1.0 / (2 * M_PI * sigma * sigma);
            double exponent = -(i * i + j * j) / (2 * sigma * sigma);
            double kernelValue = coefficient * std::exp(exponent);
            kernel[i + countOfRows][j + countOfRows] = kernelValue;
            sum+= kernelValue;
        }

    }
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] /= sum;
        }
    }

    for (int i = 0; i < originalImage.get_height(); i++) {
        for(int j = 0; j < originalImage.get_width(); j++) {
            double kernelSum = 0;
            int startingRow = i - countOfRows;
            int endingRow = i + countOfRows;
            int startingCol = j - countOfRows;
            int endingCol = j + countOfRows;

            // 3. For each pixel, compute the weighted sum using the kernel.
            for(int r = startingRow; r <= endingRow; r++) {
                for(int c = startingCol; c <= endingCol; c++) {
                    if (r >= 0 && r < originalImage.get_height() &&
                        c >= 0 && c < originalImage.get_width()) {
                        kernelSum += kernel[r + countOfRows-i][c + countOfRows-j] * originalImage.get_pixel(r, c);
                        } else {
                            kernelSum += 0;
                        }
                }
            }
            int gaussian = (int) kernelSum ;
            // 4. Update the pixel values with the smoothed results.
            image.set_pixel(i, j, gaussian);
        }
    }


    for (int i = 0; i < kernelSize; i++) {
        delete[] kernel[i];
    }
    delete[] kernel;
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    GrayscaleImage originalImage = GrayscaleImage(image);

    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    apply_gaussian_smoothing(image,kernelSize,1.0);

    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            int originalIndexValue = originalImage.get_pixel(i, j);
            int blurredIndexValue = image.get_pixel(i, j);
            int newIndexValue = (int) (originalIndexValue + amount * (originalIndexValue-blurredIndexValue));

            // 3. Clip values to ensure they are within a valid range [0-255].
            if(newIndexValue > 255) {
                newIndexValue = 255;
            }else if(newIndexValue < 0) {
                newIndexValue = 0;
            }
            image.set_pixel(i, j, newIndexValue);
        }
    }



}
