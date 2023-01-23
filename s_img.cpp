//              SMedia
//  (c) 2023 Sej (github.com/SejDevStuff)
//
//  Implementation of the s_img.hpp header file

#include "s_img.hpp"

sm::SImgData sm::S_Img::to_simg(string filename)
{
    SImgData dat;
    Mat externalImg = imread(filename, IMREAD_COLOR);
    if (externalImg.empty())
    {
        setError("Cannot read image");
        return dat;
    }
    dat.width = externalImg.cols;
    dat.height = externalImg.rows;

    dat.coord_map = new SImgPixelComponent[dat.width * dat.height];

    for (int i = 0; i < externalImg.rows; i++)
    {
        for (int j = 0; j < externalImg.cols; j++)
        {
            Vec3b bgrPixels = externalImg.at<Vec3b>(i,j);
            SImgPixelComponent pixel;
            pixel.x = j;
            pixel.y = i;
            pixel.b = bgrPixels[0];
            pixel.g = bgrPixels[1];
            pixel.r = bgrPixels[2];
            dat.pixelArray.push_back(pixel);
            dat.coord_map[(pixel.y * pixel.x) + pixel.x] = pixel;
        }
    }
    return dat;
}

bool sm::S_Img::from_simg(SImgData simg, string filename)
{
    Mat externalImage = Mat::zeros( Size(simg.height, simg.width), CV_8UC3);
    for (int i = 0; i < simg.pixelArray.size(); i++)
    {
        SImgPixelComponent pix = simg.pixelArray[i];
        externalImage.at<Vec3b>(pix.y, pix.x)[0] = pix.b;
        externalImage.at<Vec3b>(pix.y, pix.x)[1] = pix.g;
        externalImage.at<Vec3b>(pix.y, pix.x)[2] = pix.r;
    }
    if (externalImage.empty())
    {
        setError("Invalid image");
        return false;
    }
    imwrite(filename, externalImage);
    return true;
}

vector<sm::SImgPixelComponent> sm::S_Img::compare_simg(sm::SImgData simgOne, sm::SImgData simgTwo)
{
    // SimgOne Compare From
    // SimgTwo Compare With
    // Different pixels from Simgtwo will be pushed

    vector<SImgPixelComponent> differences;

    for (int i = 0; i < simgOne.pixelArray.size(); i++)
    {
        SImgPixelComponent pix = simgOne.pixelArray[i];
        SImgPixelComponent pixWith = get_pixel(simgTwo, pix.x, pix.y);
        //cout << i << '\r' << flush;
        if (!error())
        {
            if (!( pix.r == pixWith.r && pix.g == pixWith.g && pix.b == pixWith.b )) 
            {
                differences.push_back(pixWith);
            }
        } else {
            getError();
        }
    }

    return differences;
}

sm::SImgData sm::S_Img::load_simg(iostream &infile)
{
    SImgData dat;

    if (!infile)
    {
        setError("Cannot open iostream for reading");
        return dat;
    }

    infile.seekg(0, ios::end);
    int sz = infile.tellg();
    infile.seekg(0);

    string magic;
    for (int i = 0; i < 4; i++)
    {
        char byte;
        infile.read(&byte, 1);
        magic += byte;
    }

    if (magic != "simg")
    {
        setError("Invalid magic bytes");
        return dat;
    }

    infile.read((char*)&dat.width, 4);
    infile.read((char*)&dat.height, 4);

    dat.coord_map = new SImgPixelComponent[dat.width*dat.height];

    int32_t x = -1;
    int32_t y = -1;

    while ((sz - infile.tellg()) >= 4)
    {
        int8_t coordinateByte;
        int8_t r, g, b;
        SImgPixelComponent pixel;

        infile.read((char*)&coordinateByte, 1);
        if (coordinateByte == 1)
        {
            infile.read((char*)&x, sizeof(x));
            infile.read((char*)&y, sizeof(y));
        } else {
            x++;
            if (x >= dat.width)
            {
                x = 0;
                y++;
            }
        }

        infile.read((char*)&r, sizeof(r));
        infile.read((char*)&g, sizeof(g));
        infile.read((char*)&b, sizeof(b));

        pixel.x = x;
        pixel.y = y;
        pixel.r = r;
        pixel.g = g;
        pixel.b = b;

        dat.pixelArray.push_back(pixel);
        dat.coord_map[(pixel.y * pixel.x) + pixel.x] = pixel;
    }

    return dat;
}

sm::SImgData sm::S_Img::load_simg(string filepath)
{
    fstream infile(filepath, ios::in | ios::binary);
    stringstream buffer;
    SImgData dat;
    if (infile)
    {
        buffer << infile.rdbuf();
        dat = load_simg(buffer);
        infile.close();
    } else {
        setError("Cannot open infile for reading");
    }
    return dat;
}

bool sm::S_Img::write_simg(SImgData simg, iostream &outfile)
{
    if (!outfile)
    {
        setError("Cannot open iostream for writing");
        return false;
    }
    if (!(simg.height > 0 && simg.width > 0))
    {
        setError("Invalid SImgData");
        return false;
    }

    // magic bytes
    char magic[] = {'s', 'i', 'm', 'g'};
    for (int i = 0; i < 4; i++)
    {
        outfile.write(&magic[i], 1);
    }

    outfile.write((char*)&simg.width, sizeof(simg.width));
    outfile.write((char*)&simg.height, sizeof(simg.height));

    int lastX = -1;
    int lastY = -1;

    int8_t COORDS_FLAG = 1;
    int8_t NO_COORDS_FLAG = 0;

    for (int i = 0; i < simg.pixelArray.size(); i++)
    {
        SImgPixelComponent pix = simg.pixelArray[i];

        if (pix.x != lastX && pix.x != (lastX + 1))
        {
            if (!(pix.x == 0 && pix.y == (lastY + 1)))
            {
                outfile.write((char*)&COORDS_FLAG, 1);
                outfile.write((char*)&pix.x, sizeof(pix.x));
                outfile.write((char*)&pix.y, sizeof(pix.y));
            } else {
                outfile.write((char*)&NO_COORDS_FLAG, 1);
            }
        } else if (pix.y != lastY)
        {
            outfile.write((char*)&COORDS_FLAG, 1);
            outfile.write((char*)&pix.x, sizeof(pix.x));
            outfile.write((char*)&pix.y, sizeof(pix.y));
        } else {
            outfile.write((char*)&NO_COORDS_FLAG, 1);
        }

        outfile.write((char*)&pix.r, sizeof(pix.r));
        outfile.write((char*)&pix.g, sizeof(pix.g));
        outfile.write((char*)&pix.b, sizeof(pix.b));
        
        lastX = pix.x;
        lastY = pix.y;
    }
    return true;
}

bool sm::S_Img::write_simg(SImgData simg, string filepath)
{
    fstream outfile(filepath, ios::out | ios::binary);
    bool status = write_simg(simg, outfile);
    if (outfile.is_open())
    {
        outfile.close();
    }
    return status;
}

bool sm::S_Img::set_pixel(SImgData &simg, int32_t x, int32_t y, int8_t r, int8_t g, int8_t b)
{
    for (int i = 0; i < simg.pixelArray.size(); i++)
    {
        if (simg.pixelArray[i].x == x && simg.pixelArray[i].y == y)
        {
            simg.pixelArray[i].r = r;
            simg.pixelArray[i].g = g;
            simg.pixelArray[i].b = b;
            return true;
        }
    }
    setError("Cannot find pixel");
    return false;
}

sm::SImgPixelComponent sm::S_Img::get_pixel(SImgData simg, int32_t x, int32_t y)
{
    SImgPixelComponent empty;
    // for (vector<SImgPixelComponent>::iterator it = simg.pixelArray.begin(); it != simg.pixelArray.end(); ++it)
    // {
    //     if (it->x == x && it->y == y)
    //     {
    //         return *it;
    //     }
    // }

    if (x <= simg.width && y <= simg.height)
    {
        if (&simg.coord_map[(y*x)+x] != 0)
        {
            return simg.coord_map[(y*x)+x];
        }
    }

    setError("Cannot find pixel");
    return empty;
}

sm::SImgData sm::S_Img::merge_simg(SImgData simgOne, SImgData simgTwo)
{
    SImgData mergedImage;
    mergedImage.width = simgOne.width;
    mergedImage.height = simgOne.height;

    mergedImage.coord_map = new SImgPixelComponent[mergedImage.width*mergedImage.height];
    
    for (int i = 0; i < simgOne.pixelArray.size(); i++)
    {
        SImgPixelComponent pix = simgOne.pixelArray[i];
        SImgPixelComponent secondPix = get_pixel(simgTwo, pix.x, pix.y);
        if (error())
        {
            getError(); // clear the error
            mergedImage.coord_map[(pix.y * pix.x) + pix.x] = pix;
            //mergedImage.coord_map[pix.x][pix.y] = make_shared<SImgPixelComponent>(pix);
            mergedImage.pixelArray.push_back(pix);
        } else
        {
            //mergedImage.coord_map[secondPix.x][secondPix.y] = make_shared<SImgPixelComponent>(secondPix);
            mergedImage.coord_map[(pix.y * pix.x) + pix.x] = secondPix;
            mergedImage.pixelArray.push_back(secondPix);
        }
    }
    return mergedImage;
}

sm::S_Img::S_Img() {}

bool sm::S_Img::error()
{
    if (lastError == "")
    {
        return false;
    }
    return true;
}

string sm::S_Img::getError()
{
    string _error = lastError;
    lastError = "";
    return _error;
}

void sm::S_Img::setError(string error)
{
    lastError = error;
}