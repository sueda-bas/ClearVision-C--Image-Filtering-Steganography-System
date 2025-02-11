# 🛰️ ClearVision – C++ Image Processing & Steganography System

ClearVision is a **C++-based image processing system** designed for **image enhancement, filtering, encryption, and steganography**. It provides tools for **denoising images**, comparing images, and **embedding & extracting secret messages** for secure communications.

## 📌 Features
- **Grayscale Image Processing**:
  - Supports **Mean, Gaussian, and Unsharp Mask filtering** for noise reduction and sharpening.
  - Implements **addition, subtraction, and comparison** operations on images.
- **Secret Image Handling**:
  - Splits images into **upper and lower triangular matrices** for secure storage.
  - Reconstructs images from stored triangular matrices.
- **Steganography & Encryption**:
  - **Embeds secret messages** in the least significant bits (LSBs) of image pixels.
  - **Extracts encrypted messages** hidden within an image.
- **File-Based Input & Output**:
  - Loads and saves images using **stb_image.h** and **stb_image_write.h**.
  - Reads commands from **CLI** for dynamic operations.

## 🎮 How It Works
1. The program loads a grayscale image (`PNG/JPG`).
2. The user selects an **image operation** via command-line arguments:
   - Apply filters (Mean, Gaussian, Unsharp Mask)
   - Compare two images
   - Embed or extract a secret message
3. The modified image or extracted message is saved to a file.
   
## 🚀 Running the Program
Compile and run the program using:
```bash
g++ -g -std=c++11 -o clearvision main.cpp SecretImage.cpp GrayscaleImage.cpp Filter.cpp Crypto.cpp
./clearvision mean example.png 3

or using Makefile:

make
./clearvision unsharp input.png 5 1.5



