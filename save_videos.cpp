//
//  config.cpp
//
///  Created by Isabel Martins on 13/01/13.
//  Copyright (c) 2013 Isabel Martins. All rights reserved.
//

#include "save_videos.hpp"




VideoWriter  prepareOutputVideo(char * VideoFile, double fps, Size frameSize, bool isColor)
{
    VideoWriter  outputVideoWriter;
    
    // opens a video output file
    outputVideoWriter.open(VideoFile, CV_FOURCC('F', 'L', 'V', '1'), fps, frameSize, isColor);
    //outputVideoWriter.open(VideoFile, CV_FOURCC('D', 'I', 'V', 'X'), fps, frameSize, isColor);
    
    if (outputVideoWriter.isOpened())
        cout << "\nOutput video file " << VideoFile <<" is opened! \n" << endl;
    else
        cout << "\nOutput video file " << VideoFile <<" is NOT opened!!! \n" << endl;
    
    return outputVideoWriter;
}


void saveOutputVideoFrame(VideoWriter outputVideoWriter, Mat frame)
{
    if (outputVideoWriter.isOpened())
        outputVideoWriter << frame;
    else
        std::cout << "Couldn't save output frame!!! \n" << std::endl;
        
}


