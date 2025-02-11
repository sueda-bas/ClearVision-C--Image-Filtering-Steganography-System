#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.


    width = image.get_width();
    height = image.get_height();
    int upperSize = (height + 1 ) * height/2 ;
    int lowerSize = (height - 1) * height/2 ;
    upper_triangular = new int[upperSize];
    lower_triangular = new int[lowerSize];
    for(int i = 0; i < upperSize; i++) {
        upper_triangular[i] = 0;
    }
    for(int i = 0; i < lowerSize; i++) {
        lower_triangular[i] = 0;
    }

    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    int counter = 0;
    for(int i = 0; i < height; i++) {
        for(int j = i; j < width; j++) {
            upper_triangular[counter] = image.get_pixel(i, j);
            counter += 1;
        }
    }
    counter = 0;
    for(int i = 1; i < height; i++) {
        for(int j = 0; j <= i-1; j++) {
            lower_triangular[counter] = image.get_pixel(i,j);
            counter += 1;
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    width = w;
    height = h;
    upper_triangular = upper;
    lower_triangular = lower;
}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);
    int counter = 0;
    for(int i = 0; i < height; i++) {
        for(int j = i; j < width; j++) {
            image.set_pixel(i,j,upper_triangular[counter]);
            counter ++;
        }
    }
    int lcounter = 0;
    for(int i = 1; i < height; i++) {
        for(int j = 0; j < i; j++) {
            image.set_pixel(i,j,lower_triangular[lcounter]);
            lcounter ++;
        }
    }
    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // Update the lower and upper triangular matrices 
    // based on the GrayscaleImage given as the parameter.
    int lowerCounter = 0;
    int upperCounter = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(i>j) {
                lower_triangular[lowerCounter] = image.get_pixel(i,j);
                lowerCounter += 1;
            }else {
                upper_triangular[upperCounter] = image.get_pixel(i,j);
                upperCounter += 1;
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    std::ofstream outFile(filename);
    // 1. Write width and height on the first line, separated by a single space.
    outFile << width << " " << height << std::endl;

    int sizeOfUpper = width * (width + 1) / 2;

    // 2. Write the upper_triangular array to the second line.
    // Ensure that the elements are space-separated.
    // If there are 15 elements, write them as: "element1 element2 ... element15"
    for(int i = 0; i < sizeOfUpper; i++) {
        outFile << upper_triangular[i];
        if(i != sizeOfUpper - 1) {
            outFile << " ";
        }
    }
    outFile << std::endl;
    int sizeOfLower = width * (width - 1) / 2;

    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.
    for (size_t i = 0; i < sizeOfLower; i++) {
        outFile << lower_triangular[i];
        if (i != sizeOfLower - 1) {
            outFile << " ";
        }
    }
    outFile << std::endl;

    outFile.close();

}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // 1. Open the file and read width and height from the first line, separated by a space.
    std::ifstream inFile(filename);
    int width;
    int height;
    inFile >> width >> height;

    // 2. Calculate the sizes of the upper and lower triangular arrays.
    int upperSize = (height + 1 ) * height/2 ;
    int lowerSize = (height - 1) * height/2 ;

    // 3. Allocate memory for both arrays.
    int* upper_triangular = new int[upperSize];
    int* lower_triangular = new int[lowerSize];

    // 4. Read the upper_triangular array from the second line, space-separated.
    for(int i = 0; i < upperSize; i++) {
        inFile >> upper_triangular[i];
    }

    // 5. Read the lower_triangular array from the third line, space-separated.
    for(int i = 0; i < lowerSize; i++) {
        inFile >> lower_triangular[i];
    }

    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.
    inFile.close();
    SecretImage secret_image(width,height,upper_triangular,lower_triangular);
    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
