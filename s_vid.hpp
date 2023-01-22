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
            bool add_frame(SVidData &svid, SImgData frame);
            SImgData get_frame(SVidData &svid, int frame_id, SImgData previous_frame);
            bool duplicate_last_frame(SVidData &svid);

            bool set_fps(SVidData &svid, int fps) { svid.fps = fps; return true; };
            bool set_dimensions(SVidData &svid, int32_t w, int32_t h) { svid.width = w; svid.height = h; return true; }

            bool save_svid(SVidData &svid, string filepath);
            SVidData load_svid(string filepath);

            bool error();
            string getError();

        private:
            void setError(string error);
            string lastError = "";
            S_Img imageProcess;
    };
};