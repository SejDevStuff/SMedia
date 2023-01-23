//              SMedia
//  (c) 2023 Sej (github.com/SejDevStuff)
//  This file shows how to use s_img and s_vid

#include "s_vid.hpp"

void simpleLoadSaveImage()
{
    // Instance the S_Img class
    sm::S_Img image;
    
    // Load Sample.png into the SIMG format
    sm::SImgData imageOne = image.to_simg("Sample.png");

    // Set the top left pixel (0,0) to green (0,255,0)
    image.set_pixel(imageOne, 0, 0, 0, 255, 0);

    // Write the modified image, in SIMG format to sample.simg
    image.write_simg(imageOne, "sample.simg");

    // Load 'sample.simg' into the SIMG format
    sm::SImgData imageTwo = image.load_simg("sample.simg");

    // Write the SIMG data to a PNG file called sample2.png
    image.from_simg(imageTwo, "sample2.png");
}

void mergeImages()
{
    // Instance the S_Img class
    sm::S_Img image;
    
    // Load Sample.png into the SIMG format
    sm::SImgData imageOne = image.to_simg("Sample.png");

    // Load sample2.jpg into the SIMG format
    sm::SImgData imageTwo = image.to_simg("sample2.jpg");

    // merge the images
    sm::SImgData imageThree = image.merge_simg(imageOne, imageTwo);

    // save output as Sample3.simg
    image.write_simg(imageThree, "Sample3.simg");
}

void createVideo()
{
    // Actual video component
    sm::SVidData video;

    // S_Vid class to process video
    sm::S_Vid videoProcess;

    // S_Img class to process images
    sm::S_Img imageProcess;

    // Set dimensions and fps
    videoProcess.set_dimensions(video, 256, 256);
    videoProcess.set_fps(video, 30);

    // Add frames
    sm::SImgData frameOne = imageProcess.to_simg("Sample.png");
    sm::SImgData frameTwo = imageProcess.to_simg("Sample2.png");
    
    for (int i = 0; i < 30; i++)
    {
        cout << "Processed " << i << " frames\r" << flush;
        videoProcess.add_frame(video, frameOne);
    }

    for (int i = 0; i < 30; i++)
    {
        videoProcess.add_frame(video, frameTwo);
    }

    // Save the video
    videoProcess.save_svid(video, "Video.svid");
}

void loadVideo()
{
    // S_Vid class to process video
    sm::S_Vid videoProcess;

    // S_Img class to process images
    sm::S_Img imageProcess;

    // Load video
    sm::SVidData video = videoProcess.load_svid("Video.svid");

    if (videoProcess.error())
    {
        cout << videoProcess.getError();
    }

    // Extract the frames
    for (int i = 0; i < video.frames.size(); i++)
    {
        sm::SVidFrameComponent frame = video.frames[i];
        imageProcess.from_simg(frame.image, "frame" + to_string(i) + ".png");
    }
}

int main()
{
    //simpleLoadSaveImage();
    //mergeImages();
    createVideo();
    //loadVideo();
}