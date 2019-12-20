/*M///////////////////////////////////////////////////////////////////////////////////////
///
//  save_videos.cpp
//
//
//  Created by Isabel Martins on 13/01/13.
//  Copyright (c) 2013 Isabel Martins. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//   * The name of the copyright holders may not be used to endorse or promote
//     products derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
//   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//   POSSIBILITY OF SUCH DAMAGE.  //
//M*/

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


