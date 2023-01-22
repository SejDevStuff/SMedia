//              SMedia
//  (c) 2023 Sej (github.com/SejDevStuff)
//
//  Implementation of the s_vid.hpp header file

#include "s_vid.hpp"

sm::SVidData sm::S_Vid::load_svid(string filepath)
{
    SVidData video;
    ifstream infile(filepath, ios::in | ios::binary);

    if (!infile)
    {
        setError("Cannot open infile for reading");
        return video;
    }

    infile.seekg(0, infile.end);
    int sz = infile.tellg();
    infile.seekg(0, infile.beg);

    string magicBytes;
    for (int i = 0; i < 4; i++)
    {
        char byte;
        infile.read(&byte, 1);
        magicBytes += byte;
    }
    if (magicBytes != "svid")
    {
        setError("Invalid magic bytes");
        return video;
    }

    // metadata
    infile.read((char*)&video.width, sizeof(video.width));
    infile.read((char*)&video.height, sizeof(video.height));
    infile.read((char*)&video.framesTotal, sizeof(video.framesTotal));
    infile.read((char*)&video.fps, sizeof(video.fps));

    // frames
    int16_t lastFrameId;

    while (infile.tellg() < sz)
    {
        infile.read((char*)&lastFrameId, sizeof(lastFrameId));
        if (lastFrameId <= video.framesTotal && lastFrameId > 0)
        {
            SVidFrameComponent frame;
            frame.lastFrameId = lastFrameId;

            int32_t frameSz;
            infile.read((char*)&frameSz, sizeof(frameSz));

            stringstream frameData;
            
            for (int i = 0; i < frameSz; i++)
            {
                char byte;
                infile.read(&byte, 1);
                frameData.write(&byte, 1);
            }

            SImgData frameImg = imageProcess.load_simg(frameData);

            if (imageProcess.error())
            {
                setError("S_IMG ERROR: " + imageProcess.getError());
                return video;
            }

            frame.image = frameImg;

            video.frames.push_back(frame); 
        }
    }

    infile.close();

    return video;
}

bool sm::S_Vid::save_svid(SVidData &svid, string filepath)
{
    ofstream outfile(filepath, ios::out | ios::binary);

    if (!outfile)
    {
        setError("Cannot open outfile for writing");
        return false;
    }

    char magic[] = {'s', 'v', 'i', 'd'};
    for (int i = 0; i < 4; i++)
    {
        outfile.write(&magic[i], 1);
    }

    // metadata
    outfile.write((char*)&svid.width, sizeof(svid.width));
    outfile.write((char*)&svid.height, sizeof(svid.height));
    outfile.write((char*)&svid.framesTotal, sizeof(svid.framesTotal));
    outfile.write((char*)&svid.fps, sizeof(svid.fps));

    // frames
    for (int i = 0; i < svid.frames.size(); i++)
    {
        SVidFrameComponent frame = svid.frames[i];
        outfile.write((char*)&frame.lastFrameId, sizeof(frame.lastFrameId));
        
        stringstream compiled_img;
        imageProcess.write_simg(frame.image, compiled_img);

        if (imageProcess.error())
        {
            setError("S_IMG ERROR: " + imageProcess.getError());
            return false;
        }

        int32_t frameSz = compiled_img.str().length();

        outfile.write((char*)&frameSz, sizeof(frameSz));
        outfile.write(compiled_img.str().c_str(), frameSz);
    }

    outfile.close();

    return true;
}

sm::SImgData sm::S_Vid::get_frame(SVidData &svid, int frame_id, SImgData previous_frame)
{
    SImgData current_frame;

    if (frame_id > svid.framesTotal || frame_id < 1)
    {
        setError("Invalid frame id");
        return previous_frame;
    }

    bool found = false;

    for (int i = 0; i < svid.frames.size(); i++)
    {
        if (frame_id <= svid.frames[i].lastFrameId)
        {
            found = true;
            current_frame = svid.frames[i].image;
        }
    }

    if (!found)
    {
        setError("Cannot find frame");
        return previous_frame;
    }

    if (frame_id > 1)
    {
        return imageProcess.merge_simg(previous_frame, current_frame);
    }

    return current_frame;
}

bool sm::S_Vid::duplicate_last_frame(SVidData &svid)
{
    svid.framesTotal++;
    svid.frames[svid.frames.size() - 1].lastFrameId++;
    return true;
}

bool sm::S_Vid::add_frame(SVidData &svid, SImgData frame)
{
    if (frame.width != svid.width || frame.height != svid.height)
    {
        setError("Frame dimensions does not match video dimensions");
        return false;
    }

    SVidFrameComponent to_add_frame;

    if (svid.frames.size() == 0)
    {
        to_add_frame.image = frame;
    } else
    {
        SImgData diff_frame;
        SImgData lastFrame = svid.frames.back().image;
        diff_frame.width = frame.width;
        diff_frame.height = frame.height;
        diff_frame.pixelArray = imageProcess.compare_simg(lastFrame, frame);
        if (imageProcess.error()) {
            setError("S_IMG ERROR: " + imageProcess.getError());
            return false;
        }

        // Duplicate last frame if less than 1% of pixels changed
        if (diff_frame.pixelArray.size() <= floor((float)(diff_frame.width * diff_frame.height) * 0.01))
        {
            duplicate_last_frame(svid);
            return true;
        }

        to_add_frame.image = diff_frame;
    }

    svid.framesTotal++;
    to_add_frame.lastFrameId = svid.framesTotal;
    svid.frames.push_back(to_add_frame);
    
    return true;
} 

void sm::S_Vid::setError(string error)
{
    lastError = error;
}

bool sm::S_Vid::error()
{
    if (lastError == "")
    {
        return false;
    }
    return true;
}

string sm::S_Vid::getError()
{
    string _error = lastError;
    lastError = "";
    return _error;
}