//              SMedia
//  (c) 2023 Sej (github.com/SejDevStuff)
//
//  s_vid.hpp
//  File used to process SVID image files.
//  (Requires s_img.hpp & s_img.cpp)

#include "s_img.hpp"

namespace sm {

    /// @brief An individual frame in an SVID
    struct SVidFrameComponent
    {
        SImgData image;
        int16_t lastFrameId;
    };

    /// @brief The struct which contains the data for an SVID
    struct SVidData
    {
        int32_t width;
        int32_t height;
        vector<SVidFrameComponent> frames;
        int16_t framesTotal = 0;
        int8_t fps = 0;
    };

    class S_Vid
    {
        public:

            /// @brief Add a frame into an SVID video object. Automatically handles finding the difference between
            /// frames and only stores the different pixels.
            /// @param svid The video object
            /// @param frame The actual frame
            /// @return true if successful, false if not.
            bool add_frame(SVidData &svid, SImgData frame);

            /// @brief Returns the full frame image. Automatically merges the previous frame and the changed pixels.
            /// @param svid The video object
            /// @param frame_id The frame ID
            /// @param previous_frame The previous frame to merge the changed pixels with. 
            /// If you are getting the first frame (id 1), you can just pass an empty SImgData object.
            /// @return An SImgData object containing the full frame
            SImgData get_frame(SVidData &svid, int frame_id, SImgData previous_frame);
            
            /// @brief Increment the lastFrameId of the previous frame
            /// @param svid The video object
            /// @return true if successful, false if not.
            bool duplicate_last_frame(SVidData &svid);

            /// @brief Set the FPS (Frames Per Second) of a video
            /// @param svid The video object
            /// @param fps The FPS you want to set
            /// @return true if successful, false if not
            bool set_fps(SVidData &svid, int fps) { svid.fps = fps; return true; };
            
            /// @brief Set the dimensions of a video. All frames must match this dimension.
            /// @param svid The video object
            /// @param w The width of the video
            /// @param h The height of the video
            /// @return true if successful, false if not
            bool set_dimensions(SVidData &svid, int32_t w, int32_t h) { svid.width = w; svid.height = h; return true; }

            /// @brief Save the video object to a .SVID file
            /// @param svid The video object
            /// @param filepath The path to the SVID file (will automatically create if it doesn't exist, and will overwrite if it does)
            /// @return true if successful, false if not
            bool save_svid(SVidData &svid, string filepath);
            
            /// @brief Load a video object from an SVID file
            /// @param filepath The path to the SVID file
            /// @return The video object
            SVidData load_svid(string filepath);

            /// @brief Check if there is an error
            /// @return true if error, false if not
            bool error();

            /// @brief Get the latest error message, also clears the error bit
            /// @return The error message, or an empty string ("") if one doesn't exist
            string getError();

        private:
            void setError(string error);
            string lastError = "";
            S_Img imageProcess;
    };
};