# SMedia

## What is it?
SMedia is a set of header files which provides functionality to read, write and process SIMG and SVID files.

*Disclaimer:*  I have tried to document the header (.hpp) files as much as I can to explain the functions, but I have not documented the source (.cpp) files.

## Files explained:

All files require OpenCV2 to compile,  as they use OpenCV to process external images (.png, .jpg, etc) and convert them into SIMG files.

``s_img.hpp`` and ``s_img.cpp``: These provide functionality to process SIMG files. ``s_img.cpp`` must be compiled along with the project.

``s_vid.hpp`` and ``s_vid.cpp``: These provide functionality to process SVID files. These files require ``s_img.hpp`` and ``s_img.cpp`` to be present.

``SImgView.cpp``: A simple image viewer for SIMG files, showing how to use ``s_img.hpp`` and ``s_img.cpp``. Requires SDL2.

``SVidPlay.cpp``: A simple video player for SVID files, showing how to use ``s_vid.hpp`` and ``s_vid.cpp``. Requires SDL2.

``test.cpp``: A simple, documented example file showing how to use both SImg and SVid.

``CMakeLists.txt``: The CMakeLists file to compile ``SImgView``, ``SVidPlay`` and ``test``, requires OpenCV2 and SDL2 to compile. To compile all the files, first run ``cmake .`` and then run ``make``.

``Sample.png``, ``Sample2.png`` and ``sample2.jpg``: These are sample image files used by ``test.cpp`` to show the functionality of SVid and SImg.

## Structure of SImg
This is the structure for an SImg file<br><hr>
``[int32_t] width``<br>
``[int32_t] height``<br>
``[...] Pixel data``<br><hr>
### Pixel data structure
``[int8_t] coordinate flag``
> If coordinate flag == 1<br>
> ``[int32_t] x coordinate``<br>
> ``[int32_t] y coordinate``

``[int8_t] r``<br>
``[int8_t] g``<br>
``[int8_t] b``

## Structure of SVid
This is the structure for an SVid file<br><hr>
``[int32_t] width``<br>
``[int32_t] height``<br>
``[int16_t] total frames``<br>
``[int8_t] fps``<br>
``[...] frames``<br><hr>
### Frame structure
``[int16_t] lastFrameId``<br>
``[int32_t] frame size``<br>
``[SImg] frame data``

## Features of SVid
<ul>
<li>Duplicating a frame does not mean writing the same frame data again. It will simply increment the lastFrameId property of that frame, meaning one frame data can serve multiple frames.</li>
<li>SVid only stores the full image data for the first frame, every subsequent frame is just the pixels which changed from the previous frame.</li>
<li>SVid disregards the frames and duplicates the last frame if less than 1% of the pixels changed.</li>
</ul>

## Limitations of SVid
SVid can only store a max of 65535 frames. At 30fps, this is about 36 minutes of video. This is so the entire video can be loaded into memory at once. If you wish to store more video data, you might want to implement a way to fragment the video over multiple SVid files (I may write an implementation of this, but it isn't planned right now)