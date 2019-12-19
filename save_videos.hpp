//
//  config.hpp
//
//  Created by Isabel Martins on 13/01/13.
///

#ifndef __SAVE_VIDEOS_HPP__
#define __SAVE_VIDEOS_HPP__

#include <iostream>

#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core.hpp"

using namespace cv;
using namespace std;


VideoWriter  prepareOutputVideo(char * VideoFile, double fps, Size frameSize, bool isColor);

void         saveOutputVideoFrame(VideoWriter outputVideoWriter, Mat frame);


#endif  /* __SAVE_VIDEOS_HPP__ */