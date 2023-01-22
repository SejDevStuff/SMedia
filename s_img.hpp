//              SMedia
//  (c) 2023 Sej (github.com/SejDevStuff)
//
//  s_img.hpp
//  File used to process SIMG image files.

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

namespace sm {

    /// @brief An individual pixel in a SImgData struct
    struct SImgPixelComponent
    {
        int8_t r;
        int8_t g;
        int8_t b;
        int32_t x;
        int32_t y;
    };
    
    /// @brief The struct which contains the data for an SIMG
    struct SImgData
    {
        int32_t width;
        int32_t height;
        vector<SImgPixelComponent> pixelArray;
    };
    

    class S_Img
    {
        public:
            S_Img();

            /// @brief Load an external image (.png, .jpg, etc) into an SIMG image
            /// @param filepath The path to the external image
            /// @return The SIMG image in the form of an SImgData struct
            SImgData to_simg(string filepath);

            /// @brief Write to an external image (.png, .jpg, etc) from the SIMG image
            /// @param simg The SImgData struct containing the data for an SIMG image
            /// @param filepath The path to the external image (will be overwritten if it exists)
            /// @return true if successful, or false if not.
            bool from_simg(SImgData simg, string filepath);
            
            /// @brief Save the SIMG image to the disk in a .simg file
            /// @param simg The SImgData struct containing the data for an SIMG image
            /// @param outfile The path to save the .simg file (will be overwritten if it exists)
            /// @return true if successful, or false if not.
            bool write_simg(SImgData simg, string outfile);
            
            /// @brief Save the SIMG image to the disk in a .simg file
            /// @param simg The SImgData struct containing the data for an SIMG image
            /// @param file_handle The file handle to the IOSTREAM to write
            /// @return true if successful, or false if not
            bool write_simg(SImgData simg, iostream &file_handle);
            
            /// @brief Load an SIMG image from a .simg file
            /// @param infile The path to the .simg file
            /// @return An SImgData struct containing the data for the image
            SImgData load_simg(string infile);
            
            /// @brief Load an SIMG image from a .simg file
            /// @param file_handle The file handle to the IOSTREAM to read
            /// @return An SImgData struct containing the data for the image
            SImgData load_simg(iostream &file_handle);

            /// @brief Compare two SIMG images, returns a vector of the differing pixels
            /// @param simgOne The image to compare from (in an SImgData struct)
            /// @param simgTwo The image to compare with (in an SImgData struct)
            /// @return A vector containing the differing SImgPixelComponents from simgTwo
            vector<SImgPixelComponent> compare_simg(SImgData simgOne, SImgData simgTwo);
            
            /// @brief Merge two SIMG images together
            /// @param simgOne The background SIMG (in an SImgData struct)
            /// @param simgTwo The image to put on top of the background (in an SImgData struct)
            /// @return An SImgData struct which contains the data for the merged image
            SImgData merge_simg(SImgData simgOne, SImgData simgTwo);
            
            /// @brief Set a specific pixel in a SIMG
            /// @param simg A reference to the SImgData struct
            /// @param x The X position of the pixel
            /// @param y The Y position of the pixel
            /// @param r The R value of the pixel
            /// @param g The G value of the pixel
            /// @param b The B value of the pixel
            /// @return True if the pixel was set, False if it was not set
            bool set_pixel(SImgData &simg, int32_t x, int32_t y, int8_t r, int8_t g, int8_t b);
            
            /// @brief Get a pixel from a SIMG
            /// @param simg The SImgData struct to get the pixel from
            /// @param x The X coordinate of the pixel
            /// @param y The Y coordinate of the pixel
            /// @return The SImgPixelComponent of the pixel at (X, Y)
            SImgPixelComponent get_pixel(SImgData simg, int32_t x, int32_t y);

            /// @brief Check if there is an error while processing the image
            /// @return True if yes, False if yes
            bool error();

            /// @brief Get the current error
            /// @return Returns the error, if one exists, or returns just an empty string
            string getError();
        
        private:
            void setError(string error);
            string lastError = "";
    };
};
