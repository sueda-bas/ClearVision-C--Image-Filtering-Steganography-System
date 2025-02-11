#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage reconstructed_image = secret_image.reconstruct();

    // 2. Calculate the image dimensions.
    int height = reconstructed_image.get_height();;
    int width = reconstructed_image.get_width();

    // 3. Determine the total bits required based on message length.
    int totalBitsMessage = message_length * 7;
    int totalPixels = height * width;

    // 4. Ensure the image has enough pixels; if not, throw an error.
    if (totalPixels < totalBitsMessage) {
        throw std::length_error("The image does not have enough pixels.");
    }

    // 5. Calculate the starting pixel from the message_length knowing that  
    //    the last LSB to extract is in the last pixel of the image.
    int startingIndex = totalPixels - totalBitsMessage;
    int startingRow = startingIndex / width;
    int startingColumn = startingIndex % width;

    // 6. Extract LSBs from the image pixels and return the result.
    for (int i = startingRow; i < height; i++) {
        for (int j = (i == startingRow ? startingColumn : 0 ); j < width; j++) {
            int pixelValue = reconstructed_image.get_pixel(i,j) ;
            int lsb = pixelValue % 2;
            LSB_array.push_back(lsb);
        }
        if (LSB_array.size() == totalBitsMessage) {
            return LSB_array;
        }
    }

    return LSB_array;
}

// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if (LSB_array.size() % 7 != 0) {
        throw std::length_error("LSB array size must be multiple of 7.");
    }

    // 2. Convert each group of 7 bits into an ASCII character.
    // 3. Collect the characters to form the decrypted message.
    for (size_t i = 0; i < LSB_array.size(); i += 7) {
        int asciiValue = 0;
        for (int j = 0; j < 7; ++j) {
            if (LSB_array[i + j]) {
                int value = 1 ;
                for (int i = 1; i <= (6-j); i++) {
                    value *= 2;
                }
                asciiValue += value;
            }
        }
        char character = (char) asciiValue;
        message += character;
    }

    // 4. Return the resulting message.
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    for (char character : message) {
        std::bitset<7> binary(character);
        for (int i = 6; i >= 0; i--) {
            LSB_array.push_back(binary[i]);
        }
    }

    // 3. Return the array of bits.
    return LSB_array;
}

SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {

    int height = image.get_height();
    int width = image.get_width();
    int maxSize =height * width;
    int LSB_size = LSB_array.size();

    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    if(LSB_size > maxSize) {
        throw std :: length_error("The image does not have enough pixels.");
    }

    // 2. Find the starting pixel based on the message length knowing that
    //    the last LSB to embed should end up in the last pixel of the image.
    int startingIndex = maxSize - LSB_size;
    int startingRow = startingIndex / height;
    int startingColumn = startingIndex % height;
    int lsbIndex = 0;

    // 3. Iterate over the image pixels, embedding LSBs from the array.
    for (int i = startingRow; i < height; i++) {
        for (int j = (i == startingRow ? startingColumn : 0); j < width; j++) {
            if (lsbIndex >= LSB_size) {
                break;
            }
            int pixel = image.get_pixel(i, j);
            if(LSB_array[lsbIndex] == 1) {
                pixel |= 1;
            }else {
                pixel &= ~1;
            }
            image.set_pixel(i, j, pixel);
            lsbIndex ++;
        }
    }
    SecretImage secret_image(image);

    // 4. Return a SecretImage object constructed from the given GrayscaleImage
    //    with the embedded message.
    return secret_image;
}
