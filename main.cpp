/*M///////////////////////////////////////////////////////////////////////////////////////
 //
 //  main.cpp
 //
 //  Copyright (c) 2016 Isabel Martins. All rights reserved.
 //  Created by Isabel Martins on 10/10/16.
 /⁄
 ///////////////////////////////////////////////////////////////////////////////////////*/
//
//
// opencv include files
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"

// C++ include files
#include <iostream>
#include <sstream>
#include<fstream>

// My include files
#include "config.hpp"
#include "save_videos.hpp"
#include "BMOG.hpp"
#include "Colinear_9D.hpp"



using namespace cv;
using namespace std;
using namespace COLINEARITY_9D;



void  PostProcessing(InputOutputArray _mask, int PPmedianFilterSize, int PPminObjSize);
void  AngleBMOGPostProcessing(InputOutputArray _mask, int PPmedianFilterSize, int PPminObjSize);

void CompareMasks(Mat& RGBmask, Mat& mask1, Mat& mask2, Mat& mask3, Mat& mask4);


#ifndef FILENAME_MAX_LEN
#define FILENAME_MAX_LEN  350
#endif


// Global variables

bool dataset_SABS       = false;
bool dataset_BMC        = false;

bool read_GT            = false;

// Colinearity flags
bool run_colinearity    = true;
bool run_colinearity_ab = false;
bool compute_angle      = true;  // false = div_std
int  Pattern            = 3;
bool use_LPF            = true;


int main(int argc, const char * argv[])
{
    Config	mainCfg;
    char inputImageFile[FILENAME_MAX_LEN];
    char inputVideoFile[FILENAME_MAX_LEN];
    char outputImageFile[FILENAME_MAX_LEN];
    char fileOutputMask[FILENAME_MAX_LEN];
    char gtFile[FILENAME_MAX_LEN];              // <video_path>/groundtruth/gt000001.png

    char *inputMediaType;
    char outputMediaType[8];
    char saveOutputFlag[3];
    char displayOutputFlag[3];
    int  firstFrameNumber, stopFrameNumber, skipFrame;
    
    // in case a video media is used as input, its manager is declared here
    VideoCapture videoCap;
    // declare manager of a video media used to store the result
    VideoWriter  OutputVideoOutputMask;
    
    Size    frameSize;
    double  fps_in, fps_out;
    int     fourcc, numChannels=0;
    bool    isColor=false;
    
    Mat originalFrame;
    Mat CombinedResultsFGimg, CombinedResultsFGmask;
    Mat PrevFrame;
    Mat prevInputLab;
    Mat RGBmasks;
    Mat imGT;

    int    f, frame_count=0, numFrames=1;
    
    double time_start, time_end, timeDiff_Total=0;
  
    
    // input arguments checking
    if (argc<2)
    {
        cout<< "Usage : "<< argv[0]<<" <video configuration file> "<<" [retina configuration file] "<<" [output file] \n"<<endl;
        return -1;
    }
    
    
    // Read configuration parameters from disk
    string configFileName = argv[1];
    char *filename;
    filename = new char [configFileName.size()+1];
    strcpy (filename, configFileName.c_str());
    ReadConfigFile(filename, &mainCfg);
    cout << "\n ReadConfigFile " << filename << " done \n" << endl;
    // Echo the configuration information read
    PrintConfig(&mainCfg);
    strcpy(outputMediaType, mainCfg.GetConfigOutputMediaType());
    strcpy(saveOutputFlag, mainCfg.GetConfigSaveOuput());
    strcpy(displayOutputFlag, mainCfg.GetConfigDisplayOuput());
    
    if (argc==4) {
        configFileName = argv[3];
        strcpy (outputImageFile, configFileName.c_str());
    } else
        strcpy(outputImageFile, mainCfg.GetConfigOutputFiles());
    
    
    
    // set the parameters of the frame loop
    firstFrameNumber = mainCfg.GetConfigFirstFrame();
    stopFrameNumber = mainCfg.GetConfigLastFrame();
    skipFrame = mainCfg.GetConfigFrameSkip();
    
    
    
    // checking input media type (still image or video file)
    inputMediaType = mainCfg.GetConfigInputMediaType();
    if (!strcmp(inputMediaType, "image"))
    {
        if (dataset_SABS)
            sprintf(inputImageFile, "%s%04d.png", mainCfg.GetConfigInputSeq(), firstFrameNumber);
        else
            // CDnet
            sprintf(inputImageFile, "%s%06d.jpg", mainCfg.GetConfigInputSeq(), firstFrameNumber);
        
        cout << "Processing image " << inputImageFile << endl;
        // image processing case
        originalFrame = imread(string(inputImageFile), IMREAD_UNCHANGED); // load image as is
        frameSize = originalFrame.size();
        numChannels = originalFrame.channels();
        if (numChannels>1)
            isColor=true;
        cout << "\n\nOriginal Frame Size: " << "cols=" << frameSize.width << "    rows=" << frameSize.height << endl;
        cout << "Original Frame Channels: " << numChannels << endl;
        // set frame rate
        fps_in = 25;  // WallFlower seqs: 4 Hz
        cout << "Input Frame Rate is set to: " << fps_in << endl;
        
        frame_count = firstFrameNumber;
        if (stopFrameNumber == 0)
            stopFrameNumber = 100000;
        
    }else
        if (!strcmp(inputMediaType, "video"))
        {
            strcpy(inputVideoFile, mainCfg.GetConfigInputSeq());
            cout << "\nProcessing video stream " << inputVideoFile << endl;
            videoCap.open(inputVideoFile);
            // grab a first frame to check if everything is ok and point again to first frame
            videoCap>>originalFrame;
            videoCap.set(CAP_PROP_POS_FRAMES,0);
            // get input video properties
            frameSize = originalFrame.size();
            numFrames = (int)videoCap.get(CV_CAP_PROP_FRAME_COUNT);
            //frameSize = Size((int) videoCap.get(CAP_PROP_FRAME_WIDTH),(int) videoCap.get(CAP_PROP_FRAME_HEIGHT));
            fourcc = static_cast<int>(videoCap.get(CAP_PROP_FOURCC));
            fps_in = videoCap.get(CV_CAP_PROP_FPS);
            numChannels = originalFrame.channels();
            if (numChannels>1)
                isColor=true;
            // Transform from int to char via bitwise operators
            char EXT[] = {(char)(fourcc & 0XFF) , (char)((fourcc & 0XFF00) >> 8),(char)((fourcc & 0XFF0000) >> 16),(char)((fourcc & 0XFF000000) >> 24), 0};
            
            cout << "\n\nOriginal Frame Size: " << "cols=" << frameSize.width << "    rows=" << frameSize.height << endl;
            cout << "Original Frame Rate: " << fps_in << endl;
            cout << "Original Frame Channels: " << numChannels << endl;
            cout << "Original Number of Frames: " << numFrames << endl;
            cout << "Original codec type: " <<  EXT << "\n" << endl;
            
            frame_count=0;
            while (frame_count < firstFrameNumber)
            {
                videoCap>>originalFrame;
                if( originalFrame.empty() )
                    break;
                cout << "skiping frame " <<  frame_count << "\n" << endl;
                frame_count++;
            }
            //frame_count++;
            if (stopFrameNumber == 0)
                stopFrameNumber = numFrames-1;
            
        } else
        {
            // bad command parameter
            cout << "Bad input media type parameter" << "\n" << endl;
            return -1;
        }
  
    
    PrevFrame.create(originalFrame.size(), originalFrame.type());
    prevInputLab.create(originalFrame.size(), originalFrame.type());
    cvtColor(originalFrame, prevInputLab, CV_BGR2Lab);

    
    string gtDir;
    if (read_GT)
    {
        //////////////////////////////////////
        //   opens GT file
        gtDir.assign(mainCfg.GetConfigInputSeq());
        gtDir.replace(gtDir.find("input/in"),8,"groundtruth/gt");
        std::cout << endl << endl << gtDir.c_str() << endl << endl;
        
        // testa
        sprintf(gtFile, "%s%06d.png", gtDir.c_str(), firstFrameNumber);
        imGT = imread(string(gtFile), IMREAD_GRAYSCALE );
        if( imGT.empty() )
            cout << "\n\n imGT empty frame!" << endl;
    }

    
    
    if ( (!strcmp(saveOutputFlag, "y")) && (!strcmp(outputMediaType, "video")) )
    {
        fps_out = fps_in/skipFrame;
        
         // open output video file for MOG2 FG mask
         strcpy(fileOutputMask, outputImageFile); strcat(fileOutputMask, "_MOG2FGmask.avi");
         OutputVideoOutputMask = prepareOutputVideo(fileOutputMask, fps_out, frameSize, false);
    }
    
    fprintf(stdout,"Display output (y/n) :%s\n", (char *)displayOutputFlag);
    if  (!strcmp(displayOutputFlag, "y"))
    {
        //std::cout << "\n\nSetup Windows! \n\n" << std::endl;
        // setup windows
        //namedWindow("window 1", CV_WINDOW_AUTOSIZE);
        namedWindow("window 1", CV_WINDOW_NORMAL);
        cvMoveWindow("window 1", 25, 1);
        namedWindow("window 2", CV_WINDOW_NORMAL);
        cvMoveWindow("window 2", 25, frameSize.height+40);
        namedWindow("window 3", CV_WINDOW_NORMAL);
        cvMoveWindow("window 3", 25+frameSize.width, 1);
        namedWindow("window 4", CV_WINDOW_NORMAL);
        cvMoveWindow("window 4", 25+frameSize.width, frameSize.height+40);
        namedWindow("window 5", CV_WINDOW_NORMAL);
        cvMoveWindow("window 5", 25+2*frameSize.width, 1);
        namedWindow("window 6", CV_WINDOW_NORMAL);
        cvMoveWindow("window 6", 25+2*frameSize.width, frameSize.height+40);
        namedWindow("window 7", CV_WINDOW_NORMAL);
        cvMoveWindow("window 7", 25+3*frameSize.width, 1);
        namedWindow("window 8", CV_WINDOW_NORMAL);
        cvMoveWindow("window 8", 25+3*frameSize.width, frameSize.height+40);
        
    }
    
    
    
    ///////////////////////////////////////////////////////////
    ////////////  BG color model using BMOG: SETUP  ///////////
    ///////////////////////////////////////////////////////////
    
    Ptr<BackgroundSubtractorBMOG> bgLabMOG=createBackgroundSubtractorBMOG();
    
    /*
    cout << "\n\nLabMOG Default Values: " << endl;
    cout << "LabMOG Color Conversion: " << bgLabMOG->getColorSpaceConversion() << endl;
    cout << "LabMOG NMixtures: " << bgLabMOG->getNMixtures() << endl;
    cout << "LabMOG VarThreshold_L: " << bgLabMOG->getVarThreshold_L() << endl;
    cout << "LabMOG VarThreshold_a: " << bgLabMOG->getVarThreshold_a() << endl;
    cout << "LabMOG VarThreshold_b: " << bgLabMOG->getVarThreshold_b() << endl;
    cout << "LabMOG BackgroundRatio: " << bgLabMOG->getBackgroundRatio() << endl;
    cout << "LabMOG VarThresholdGen: " << bgLabMOG->getVarThresholdGen() << endl;
    cout << "LabMOG Initial Variance: " << bgLabMOG->getVarInit() << endl;
    cout << "LabMOG Minimum Variance: " << bgLabMOG->getVarMin() << endl;
    cout << "LabMOG Maximum Variance: " << bgLabMOG->getVarMax() << endl;
    cout << "LabMOG History: " << bgLabMOG->getHistory() << endl;
    cout << "LabMOG ComplexityReductionThreshold: " << bgLabMOG->getComplexityReductionThreshold() << endl;
    cout << "LabMOG BkgLearningRate: " << bgLabMOG->getBkgLearningRate() << endl;
    cout << "LabMOG UncoveredBkgLearningRate: " << bgLabMOG->getUncoveredBkgLearningRate() << endl;
    cout << "LabMOG FgLearningRate: " << bgLabMOG->getFgLearningRate() << endl;
    cout << "LabMOG StepUncoveredBkgLearningRate: " << bgLabMOG->getStepUncoveredBkgLearningRate() << endl;
    cout << "LabMOG Hysteresis delta L: " << bgLabMOG->getDeltaTb_L() << endl;
    cout << "LabMOG Hysteresis delta a: " << bgLabMOG->getDeltaTb_a() << endl;
    cout << "LabMOG Hysteresis delta b: " << bgLabMOG->getDeltaTb_b() << endl;
    cout << "LabMOG PostProcessing Median Filter Size (0=n): " << bgLabMOG->getPostProcessingMedianFilterSize() << endl;
    cout << "LabMOG PostProcessing Minimum Object Size: " << bgLabMOG->getPostProcessingMinObjectSize() << endl;
    cout << "LabMOG TransientFrames: " << bgLabMOG->getTransientFrames() << endl;
    cout << "LabMOG Color Space Conversion: " << bgLabMOG->getColorSpaceConversion() << endl;
    */
    
    // set new BMOG parameters
    bgLabMOG->setVarThreshold_L( 45.0f );   //35.0f
    // not needed - these are the default values
    bgLabMOG->setNMixtures( 5 );
    bgLabMOG->setVarThreshold_a( 12.0f );   //12.0f
    bgLabMOG->setVarThreshold_b( 12.0f );   //12.0f
    
    bgLabMOG->setBackgroundRatio( 0.9f );
    bgLabMOG->setVarThresholdGen( 8.0f );
    bgLabMOG->setVarInit( 11.0f );
    bgLabMOG->setVarMin(4.0f);
    bgLabMOG->setVarMax(55.0f);
    bgLabMOG->setHistory( 100 );
    bgLabMOG->setComplexityReductionThreshold( 0.05f );
    
    bgLabMOG->setBkgLearningRate( 0.001f );
    bgLabMOG->setUncoveredBkgLearningRate( 0.01f );
    bgLabMOG->setFgLearningRate( 0.0001f);                  // 0.0005f
    bgLabMOG->setStepUncoveredBkgLearningRate( 0.005f );
    
    bgLabMOG->setDeltaTb_L( 5.0f );
    bgLabMOG->setDeltaTb_a( 5.0f );
    bgLabMOG->setDeltaTb_b( 5.0f );
    
    bgLabMOG->setPostProcessingMedianFilterSize( 9 );
    bgLabMOG->setPostProcessingMinObjectSize( 0 );
    bgLabMOG->setTransientFrames( 50 );
    bgLabMOG->setColorSpaceConversion(0);  // 0=no_conversion  1=BGR->L*a*b*

    cout << "\n\nLabMOG Set Values: " << endl;
    cout << "LabMOG Color Conversion: " << bgLabMOG->getColorSpaceConversion() << endl;
    cout << "LabMOG NMixtures: " << bgLabMOG->getNMixtures() << endl;
    cout << "LabMOG VarThreshold_L: " << bgLabMOG->getVarThreshold_L() << endl;
    cout << "LabMOG VarThreshold_a: " << bgLabMOG->getVarThreshold_a() << endl;
    cout << "LabMOG VarThreshold_b: " << bgLabMOG->getVarThreshold_b() << endl;
    cout << "LabMOG BackgroundRatio: " << bgLabMOG->getBackgroundRatio() << endl;
    cout << "LabMOG VarThresholdGen: " << bgLabMOG->getVarThresholdGen() << endl;
    cout << "LabMOG Initial Variance: " << bgLabMOG->getVarInit() << endl;
    cout << "LabMOG Minimum Variance: " << bgLabMOG->getVarMin() << endl;
    cout << "LabMOG Maximum Variance: " << bgLabMOG->getVarMax() << endl;
    cout << "LabMOG History: " << bgLabMOG->getHistory() << endl;
    cout << "LabMOG ComplexityReductionThreshold: " << bgLabMOG->getComplexityReductionThreshold() <<endl;
    cout << "LabMOG BkgLearningRate: " << bgLabMOG->getBkgLearningRate() << endl;
    cout << "LabMOG UncoveredBkgLearningRate: " << bgLabMOG->getUncoveredBkgLearningRate() << endl;
    cout << "LabMOG FgLearningRate: " << bgLabMOG->getFgLearningRate() << endl;
    cout << "LabMOG StepUncoveredBkgLearningRate: " << bgLabMOG->getStepUncoveredBkgLearningRate() << endl;
    cout << "LabMOG Hysteresis delta L: " << bgLabMOG->getDeltaTb_L() << endl;
    cout << "LabMOG Hysteresis delta a: " << bgLabMOG->getDeltaTb_a() << endl;
    cout << "LabMOG Hysteresis delta b: " << bgLabMOG->getDeltaTb_b() << endl;
    cout << "LabMOG PostProcessing Median Filter Size (0=n): " << bgLabMOG->getPostProcessingMedianFilterSize() << endl;
    cout << "LabMOG PostProcessing Minimum Object Size: " << bgLabMOG->getPostProcessingMinObjectSize() << endl;
    cout << "LabMOG TransientFrames: " << bgLabMOG->getTransientFrames() << endl;
    cout << "LabMOG Color Space Conversion: " << bgLabMOG->getColorSpaceConversion() << endl;

    
    // declare LabMOG output buffers
    Mat PrevFGmaskLabMOG, FGmaskLabMOG, backLabMOG, FGimgLabMOG;
    
    FGmaskLabMOG.create(frameSize, CV_8UC1);
    PrevFGmaskLabMOG.create(frameSize, CV_8UC1);
    FGimgLabMOG.create(frameSize, originalFrame.type());
    FGmaskLabMOG.setTo(0);
    PrevFGmaskLabMOG.setTo(0);
    
    ///////////////////////////////////////////////////////////

    
    
    ////////////////////////////////////////////////////////////////////////////////
    //////////// BG Texture Vectors Collinearity Model using BMOG: SETUP ///////////
    ////////////////////////////////////////////////////////////////////////////////
    
    Ptr<BackgroundSubtractorBMOG> bgAngleMOG=createBackgroundSubtractorBMOG();
    
    /*
    cout << "\n\nAdaptativeAngleMOG Default Values: " << endl;
    cout << "AngleMOG NMixtures: " << bgAngleMOG->getNMixtures() << endl;
    cout << "AngleMOG VarThreshold_L: " << bgAngleMOG->getVarThreshold_L() << endl;
    cout << "AngleMOG VarThreshold_a: " << bgAngleMOG->getVarThreshold_a() << endl;
    cout << "AngleMOG VarThreshold_b: " << bgAngleMOG->getVarThreshold_b() << endl;
    cout << "AngleMOG BackgroundRatio: " << bgAngleMOG->getBackgroundRatio() << endl;
    cout << "AngleMOG VarThresholdGen: " << bgAngleMOG->getVarThresholdGen() << endl;
    cout << "AngleMOG Initial Variance: " << bgAngleMOG->getVarInit() << endl;
    cout << "AngleMOG Minimum Variance: " << bgAngleMOG->getVarMin() << endl;
    cout << "AngleMOG Maximum Variance: " << bgAngleMOG->getVarMax() << endl;
    cout << "AngleMOG History: " << bgAngleMOG->getHistory() << endl;
    cout << "AngleMOG ComplexityReductionThreshold: " << bgAngleMOG->getComplexityReductionThreshold() << endl;
    cout << "AngleMOG BkgLearningRate: " << bgAngleMOG->getBkgLearningRate() << endl;
    cout << "AngleMOG UncoveredBkgLearningRate: " << bgAngleMOG->getUncoveredBkgLearningRate() << endl;
    cout << "AngleMOG FgLearningRate: " << bgAngleMOG->getFgLearningRate() << endl;
    cout << "AngleMOG StepUncoveredBkgLearningRate: " << bgAngleMOG->getStepUncoveredBkgLearningRate() << endl;
    cout << "AngleMOG Hysteresis delta L: " << bgAngleMOG->getDeltaTb_L() << endl;
    cout << "AngleMOG Hysteresis delta a: " << bgAngleMOG->getDeltaTb_a() << endl;
    cout << "AngleMOG Hysteresis delta b: " << bgAngleMOG->getDeltaTb_b() << endl;
    cout << "AngleMOG PostProcessing MedianFilterSize (0 = no filter): " << bgAngleMOG->getPostProcessingMedianFilterSize() << endl;
    cout << "AngleMOG PostProcessing MinObjectSizeSize: " << bgAngleMOG->getPostProcessingMinObjectSize() << endl;
    cout << "AngleMOG TransientFrames: " << bgAngleMOG->getTransientFrames() << endl;
    */
    
    // set new bgAngleMOG parameters
    bgAngleMOG->setNMixtures( mainCfg.GetAngleMOGNMixtures() );
    bgAngleMOG->setVarThreshold_L( mainCfg.GetAngleMOGVarThreshold_L() );
    bgAngleMOG->setVarThreshold_a( mainCfg.GetAngleMOGVarThreshold_A() );
    bgAngleMOG->setVarThreshold_b( mainCfg.GetAngleMOGVarThreshold_B() );
    
    bgAngleMOG->setBackgroundRatio( mainCfg.GetAngleMOGBackgroundRatio() );  // 0.9
    bgAngleMOG->setVarThresholdGen( mainCfg.GetAngleMOGVarThresholdGen() );
    bgAngleMOG->setVarInit( mainCfg.GetAngleMOGVarInit() );
    bgAngleMOG->setVarMin(0.8f);
    bgAngleMOG->setVarMax(20.0f);
    bgAngleMOG->setHistory( mainCfg.GetAngleMOGHistory() );
    bgAngleMOG->setComplexityReductionThreshold( mainCfg.GetAngleMOGComplexityReductionThreshold() );
    
    /***
    bgAngleMOG->setDeltaTb_L( 0.95f );  // ok 0.8f
    bgAngleMOG->setDeltaTb_a( 0.95f );
    bgAngleMOG->setDeltaTb_b( 0.95f );
  
    bgAngleMOG->setBkgLearningRate( 0.004f );  // ok=0.001
    bgAngleMOG->setUncoveredBkgLearningRate( 0.01f );
    bgAngleMOG->setFgLearningRate( 0.0003f);   // OK=0.0003 ou ok=0.0005  // no COLBMOG CDnet 0.0035
    //bgAngleMOG->setFgLearningRate( 0.0035f);
    bgAngleMOG->setStepUncoveredBkgLearningRate( 0.005f );
    ***/
    
    bgAngleMOG->setBkgLearningRate( mainCfg.GetAngleMOG_BG_LearningRate() );  // ok=0.001
    bgAngleMOG->setUncoveredBkgLearningRate( mainCfg.GetAngleMOG_UBG_LearningRate() );
    bgAngleMOG->setFgLearningRate( mainCfg.GetAngleMOG_FG_LearningRate() );   // OK=0.0003 ou ok=0.0005  // no COLBMOG CDnet 0.0035
    bgAngleMOG->setStepUncoveredBkgLearningRate( mainCfg.GetAngleMOG_StepUBG_LearningRate() );
   
    bgAngleMOG->setDeltaTb_L( mainCfg.GetAngleMOGVarThreshold_Delta() );  // ok 0.95f
    bgAngleMOG->setDeltaTb_a( mainCfg.GetAngleMOGVarThreshold_Delta() );
    bgAngleMOG->setDeltaTb_b( mainCfg.GetAngleMOGVarThreshold_Delta() );
    
    //bgAngleMOG->setPostProcessingMedianFilterSize( mainCfg.GetAngleMOGPostProcMedianFilterSize() );
    //bgAngleMOG->setPostProcessingMinObjectSize( mainCfg.GetAngleMOGPostProcMinObjectSize() );
    // setting Post-Processing parameters to 0  we get the mask without Post-Processing
    bgAngleMOG->setPostProcessingMedianFilterSize( 0 );
    bgAngleMOG->setPostProcessingMinObjectSize( 0 );

    bgAngleMOG->setTransientFrames( mainCfg.GetAngleMOGTransientFrames() );
    bgAngleMOG->setColorSpaceConversion(0);  // 0=no_conversion 

    cout << "\n\nAngleMOG Set Values: " << endl;
    cout << "AngleMOG NMixtures: " << bgAngleMOG->getNMixtures() << endl;
    cout << "AngleMOG VarThreshold_L: " << bgAngleMOG->getVarThreshold_L() << endl;
    cout << "AngleMOG VarThreshold_a: " << bgAngleMOG->getVarThreshold_a() << endl;
    cout << "AngleMOG VarThreshold_b: " << bgAngleMOG->getVarThreshold_b() << endl;
    cout << "AngleMOG BackgroundRatio: " << bgAngleMOG->getBackgroundRatio() << endl;
    cout << "AngleMOG VarThresholdGen: " << bgAngleMOG->getVarThresholdGen() << endl;
    cout << "AngleMOG Initial Variance: " << bgAngleMOG->getVarInit() << endl;
    cout << "AngleMOG Minimum Variance: " << bgAngleMOG->getVarMin() << endl;
    cout << "AngleMOG Maximum Variance: " << bgAngleMOG->getVarMax() << endl;
    cout << "AngleMOG History: " << bgAngleMOG->getHistory() << endl;
    cout << "AngleMOG ComplexityReductionThreshold: " << bgAngleMOG->getComplexityReductionThreshold() <<endl<<endl;
    cout << "AngleMOG BkgLearningRate: " << bgAngleMOG->getBkgLearningRate() << endl;
    cout << "AngleMOG UncoveredBkgLearningRate: " << bgAngleMOG->getUncoveredBkgLearningRate() << endl;
    cout << "AngleMOG FgLearningRate: " << bgAngleMOG->getFgLearningRate() << endl;
    cout << "AngleMOG StepUncoveredBkgLearningRate: " << bgAngleMOG->getStepUncoveredBkgLearningRate() << endl;
    cout << "AngleMOG Hysteresis delta L: " << bgAngleMOG->getDeltaTb_L() << endl;
    cout << "AngleMOG Hysteresis delta a: " << bgAngleMOG->getDeltaTb_a() << endl;
    cout << "AngleMOG Hysteresis delta b: " << bgAngleMOG->getDeltaTb_b() << endl;
    cout << "AngleMOG PostProcessing MedianFilterSize (0 = no filter): " << bgAngleMOG->getPostProcessingMedianFilterSize() << endl;
    cout << "AngleMOG PostProcessing MinObjectSizeSize: " << bgAngleMOG->getPostProcessingMinObjectSize() << endl;
    cout << "AngleMOG TransientFrames: " << bgAngleMOG->getTransientFrames() << endl;
    
    // declare AngleMOG output buffers
    Mat PrevFGmaskAngleMOG, FGmaskAngleMOG; // masks generated by AngleMOG method
    
    FGmaskAngleMOG.create(frameSize, CV_8UC1);
    PrevFGmaskAngleMOG.create(frameSize, CV_8UC1);
    FGmaskAngleMOG.setTo(0);
    PrevFGmaskAngleMOG.setTo(0);
    
    ///////////////////////////////////////////////////////////


    
    
    ///////////////////////////////////////////////////////////////////////////
    ////////////    Texture Vectors Colinearity initialization     ////////////
    ///////////////////////////////////////////////////////////////////////////
    Pattern = mainCfg.GetColinPattern();
    
    char useLPF_Flag[3];
    strcpy(useLPF_Flag, mainCfg.GetColinUseLPF());
    if  (!strcmp(useLPF_Flag, "y"))
        use_LPF = true;
    else
        use_LPF = false;
    
    Mat inputLab, input[3];
    Mat anglesMat;
    Mat anglesPrevMat;
    Mat bkgTexureMeanMat;
    Mat bkgTexureMeanPrevMat;
    
    Mat anglesThresholdMat;
    anglesThresholdMat.create(frameSize, CV_32FC1);
    
    Ptr<TextureVectors> TextVectColBkg=createTextureVectors(frameSize);
    Ptr<TextureVectors> TextVectColPrev=createTextureVectors(frameSize);

    ///////////////////////////////////////////////////////////

    
    RGBmasks.create(frameSize, CV_8UC3);
    Mat finalMask, prevFinalMask;
    finalMask.create(frameSize, CV_8UC1);
    prevFinalMask.create(frameSize, CV_8UC1);
    
    Mat lowAvgMask;
    lowAvgMask.create(frameSize, CV_8UC1);
    
    ///////////////////////////////////////////////////////////
    // set thresholds for Dark Areas and Stopped Objects

    double darkAreas_Threshold = mainCfg.Get_Dark_Threshold();
    cout << "\n\nDarkAreas_Threshold: " << darkAreas_Threshold << endl;
    
    double stoppedObjects_Threshold = mainCfg.Get_Stopped_Threshold();
    cout << "\nStoppedObjects_Threshold: " << stoppedObjects_Threshold << endl;

    int finalPPfilterSize = mainCfg.GetFinalPostProcMedianFilterSize();
    cout << "\nFinal PP filter size: " << finalPPfilterSize << endl;

    ///////////////////////////////////////////////////////////
    

    
    cout << "\n\nInitialization done! \n\n" << endl;
    
    cout << "frame_count=" << frame_count << endl;
    cout << "stopFrameNumber=" << stopFrameNumber << endl;
    

    // processing MAIN LOOP
    bool process=true;
    
    cout << "Start loop! frame_count=" << frame_count << endl;
    
    
    while ((frame_count <= stopFrameNumber) && (process))
    {
        ///  READ NEW INPUT FRAME
        if (!strcmp(inputMediaType, "video"))
        {
            //cout << "\n\nReading video frame #" << videoCap.get(CV_CAP_PROP_POS_FRAMES) << endl;
            //cout << "." << flush ;
            // reads a new frame from video
            if (videoCap.isOpened())
                videoCap >> originalFrame;
            cout << "." << flush ;
        } else {
            // if (!strcmp(inputMediaType, "image"))
            if (dataset_SABS)
                sprintf(inputImageFile, "%s%04d.png", mainCfg.GetConfigInputSeq(), frame_count);
            else
                // CDnet
                sprintf(inputImageFile, "%s%06d.jpg", mainCfg.GetConfigInputSeq(), frame_count);
            
            originalFrame = imread(string(inputImageFile), 1); // load image in RGB mode
            //cout << "\n\nProcessing image " << inputImageFile << endl;
            cout << "." << flush ;
            if (frame_count == mainCfg.GetAngleMOGTransientFrames())
                cout << frame_count << " " << flush ;
            
        }
        if( originalFrame.empty() )
        {   cout << "\n\n Empty frame!" << endl;
            process=false;
        }
        
        if (read_GT)
        {
            //////////////////////////////////////////////////////////
            ///////// READS Ground Truth  //////////////////////////
            sprintf(gtFile, "%s%06d.png", gtDir.c_str(), frame_count);
            imGT = imread(string(gtFile), IMREAD_GRAYSCALE );
            if( imGT.empty() )
            {   cout << "\n\n imGT empty frame!" << endl;
                process = false;
            }
        }

        // gets start processing time in Ticks
        //time_start=(double)getTickCount();
        
        
        /*
         if (use_LPF)
            GaussianBlur(originalFrame, originalFrame, Size(3,3), 0, 0);
         */
        
        ///////////////////////////////////////////////////////////////////
        ///// convert to Lab color space
        ///////////////////////////////////////////////////////////////////
        cvtColor(originalFrame, inputLab, CV_BGR2Lab);
        
        
        /////////////////////////////////////////////////////////////////////////////
        ///  UPDATES BACKGROUND COLOR MODEL USING BMOG AND GETS BACKGROUND IMAGE  ///
        /////////////////////////////////////////////////////////////////////////////
        bgLabMOG->apply(inputLab, FGmaskLabMOG);
        bgLabMOG->getBackgroundImage(backLabMOG); // already in Lab color space

        
        if ( frame_count>1 ) // if we have a background image
        {
            // gets start processing time in Ticks
            //time_start=(double)getTickCount();

            ///////////////////////////////////////////////////////////////////
            // TEXTURE VECTORS COLINEARITY with Background image
            ///////////////////////////////////////////////////////////////////
            TextVectColBkg->apply(inputLab, backLabMOG, Pattern, use_LPF);
            TextVectColBkg->getAnglesMat(anglesMat);
            TextVectColBkg->getBkgTextureMeansMat(bkgTexureMeanMat);

            ///////////////////////////////////////////////////////////////////
            ///  COMPUTES FG SEGMENTATION using Texture Vectors Collinearity Model (BMOG without Post-Processing)
            ///////////////////////////////////////////////////////////////////
            
            // updates BG collinearity model using BMOG
            bgAngleMOG->apply(anglesMat, FGmaskAngleMOG);
            
            
            /*
            // gets end processing TIME in Ticks
            time_end=(double)getTickCount();
            // computes execution times in seconds
            timeDiff_Total += (time_end-time_start)/getTickFrequency();
            // computes average execution times in seconds
            cout << endl;
            cout << "Elapsed time/frame: " <<  (timeDiff_Total/frame_count) << "seconds \n" << endl;
            cout << "Frames/s: " <<  1.0/(timeDiff_Total/frame_count) << endl;
            */
            

            // Post-Processing: median filter, Morphology close, Filling closed contours, Erosion
            AngleBMOGPostProcessing(FGmaskAngleMOG, mainCfg.GetAngleMOGPostProcMedianFilterSize(), mainCfg.GetAngleMOGPostProcMinObjectSize());
            // updates mask inside model
            bgAngleMOG->setPreviousMask(FGmaskAngleMOG);

                        
            FGmaskAngleMOG.copyTo(finalMask);
            
            // switches to color mask (LabMOG) when Avg value is low            
            lowAvgMask.setTo(0);
            lowAvgMask = (bkgTexureMeanMat <= (float)darkAreas_Threshold);  // if AvgTextureVector<45 uses color BMOG
            FGmaskLabMOG.copyTo(finalMask, lowAvgMask);
            
            if ( frame_count>50 ) // if we have an acceptable FG/BG mask
            {
                /////////////////////////////////////////////////////////////////////////////
                // TEXTURE VECTORS COLINEARITY with Previous input when previous is FG (Textureless FG Objects)
                /////////////////////////////////////////////////////////////////////////////
                TextVectColPrev->apply(inputLab, prevInputLab, Pattern, use_LPF);
                TextVectColPrev->getAnglesMat(anglesPrevMat);
                
                //cout << "\n anglesPrevMat: " << anglesPrevMat.size().width << " , " << anglesPrevMat.size().height << " , " << anglesPrevMat.type() << endl;
                
                Mat displayAngles;
                Mat Angles[3];
                Mat aux, aux1;
                displayAngles.create(anglesPrevMat.size(), CV_8UC1);
                aux.create(anglesPrevMat.size(), CV_8UC1);
                aux.setTo(255);
                
                split(anglesPrevMat, Angles);
                threshold(Angles[0], aux, stoppedObjects_Threshold, 255.0, THRESH_BINARY_INV); 
                aux.convertTo(aux1, CV_8UC1);
                bitwise_and(prevFinalMask, aux1, aux1);
                
                bitwise_or(finalMask, aux1, finalMask);
             
                //Angles[0].convertTo(displayAngles, CV_8U, 100);
                //imshow("window 5", displayAngles);
                //imshow("window 6", aux1);
                
             }
            
            PostProcessing(finalMask, finalPPfilterSize, 0);
        
        }

       
        /*
        // gets end processing time in Ticks
        time_end=(double)getTickCount();
        // computes execution times in seconds
        timeDiff_Total += (time_end-time_start)/getTickFrequency();
        // computes average execution times in seconds
        cout << endl;
        cout << "Elapsed time/frame: " <<  (timeDiff_Total/frame_count) << "seconds \n" << endl;
        cout << "Frames/s: " <<  1.0/(timeDiff_Total/frame_count) << endl;
         */
        
        //CompareMasks(RGBmasks, imGT, FGmaskAngleMOG, FGmaskLabMOG, imGT);
        
        ///  DISPLAY RESULTS
        //Mat imgDisplay[3];
        Mat imgDisplay;

         if  (!strcmp(displayOutputFlag, "y"))
        {
            //std::cout << "\n\nDisplay Output! \n\n" << std::endl;
            imshow("window 1", originalFrame);
            //split(inputLab, imgDisplay);
            //imshow("window 1", imgDisplay[0]);
            cvtColor(backLabMOG, imgDisplay, CV_Lab2BGR);
            //split(backLabMOG, imgDisplay);
            imshow("window 2", imgDisplay);
            imshow("window 3", FGmaskLabMOG);
            imshow("window 4", FGmaskAngleMOG);
            //imshow("window 5", RGBmasks);
            imshow("window 5", finalMask);
            //imshow("window 6", imGT);
            imshow("window 6", lowAvgMask);
        }
        

        
        // SAVE RESULTS TO DISK
        if (!strcmp(saveOutputFlag, "y"))
        {
            if (!strcmp(outputMediaType, "video"))
            {
                 saveOutputVideoFrame(OutputVideoOutputMask,   finalMask);
            }
            else  // if (!strcmp(outputMediaType, "image"))
            {
                if (dataset_BMC)
                    sprintf(fileOutputMask, "%s%06d.png", mainCfg.GetConfigOutputFiles(), frame_count+3);   ////  +3 para BMC
                else
                    sprintf(fileOutputMask, "%s%06d.png", mainCfg.GetConfigOutputFiles(), frame_count);  
                //imwrite(fileOutputMask, FGmaskAngleMOG);
                imwrite(fileOutputMask, finalMask);            // final FG mask
                //cvtColor(backLabMOG, imgDisplay, CV_Lab2BGR);
                //imwrite(fileOutputMask, imgDisplay);
            }
        }
        
        inputLab.copyTo(prevInputLab);
        finalMask.copyTo(prevFinalMask);
        
        
        
        /// SKIP FRAMES if needed
        if (!strcmp(inputMediaType, "video"))
        {
            for (f=1; f<skipFrame; f++) {
                frame_count++;
                videoCap >> originalFrame;
                cout << "skiping frame " <<  frame_count << "\n" << endl;
            }
            frame_count++;
        } else
            frame_count=frame_count+skipFrame;
        
        
        if (!process)
            break;
        
        if  (!strcmp(displayOutputFlag, "y"))
            waitKey(1); // 100000
        
    }
    
    // computes average execution times in seconds
    //cout << endl;
    //cout << "Elapsed time/frame: " <<  (timeDiff_Total/frame_count) << "seconds \n" << endl;
    //cout << "Frames/s: " <<  1.0/(timeDiff_Total/frame_count) << endl;
    
    
    if ( (!strcmp(saveOutputFlag, "y")) && (!strcmp(outputMediaType, "video")) )
    {
        if (OutputVideoOutputMask.isOpened())
            OutputVideoOutputMask.release();
     }
    

    return 0;
}



void  AngleBMOGPostProcessing(InputOutputArray _mask, int PPmedianFilterSize, int PPminObjSize)
{
    Mat tmpMask1, tmpMask2;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    int idx;
    
    tmpMask1.create(_mask.size(), CV_8UC1);
    tmpMask1.setTo(0);
    tmpMask2.create(_mask.size(), CV_8UC1);
    tmpMask2.setTo(0);
    
    Mat mask = _mask.getMat();
    
    medianBlur(mask, tmpMask1, PPmedianFilterSize);
    morphologyEx(tmpMask1, tmpMask1, MORPH_CLOSE, Mat());
    
    //// Remove holes and all objects containing fewer than minObjectSize pixels
    findContours(tmpMask1, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); //CV_CHAIN_APPROX_NONE
    if (contours.size() != 0)
    {
        for (idx=0; idx >= 0; idx = hierarchy[idx][0] )
        {
            const vector<Point>& c = contours[idx];
            double area = fabs(contourArea(Mat(c)));
            if (area > PPminObjSize) // (area > minObjectSize)
            {
                // fill contours area
                drawContours(tmpMask2, contours, idx, 255, CV_FILLED, 8, hierarchy);
            }
        }
    }
    
    erode(tmpMask2, tmpMask2, Mat(), Point(-1,-1), 1);
    
    tmpMask2.copyTo(mask);
    
}





void  PostProcessing(InputOutputArray _mask, int PPmedianFilterSize, int PPminObjSize)
{
    Mat tmpMask1, tmpMask2;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    int idx;
    
    tmpMask1.create(_mask.size(), CV_8UC1);
    tmpMask1.setTo(0);
    tmpMask2.create(_mask.size(), CV_8UC1);
    tmpMask2.setTo(0);
    
    Mat mask = _mask.getMat();
    
    medianBlur(mask, tmpMask1, PPmedianFilterSize);
    morphologyEx(tmpMask1, tmpMask1, MORPH_CLOSE, Mat());
    
    //// Remove holes and all objects containing fewer than minObjectSixe pixel
    findContours(tmpMask1, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    if (contours.size() != 0)
    {
        for (idx=0; idx >= 0; idx = hierarchy[idx][0] )
        {
            const vector<Point>& c = contours[idx];
            double area = fabs(contourArea(Mat(c)));
            if (area > PPminObjSize) // (area > minObjectSize)
            {
                // fill contours area
                drawContours(tmpMask2, contours, idx, 255, CV_FILLED, 8, hierarchy);
            }
        }
    }
    
    erode(tmpMask2, tmpMask2, Mat(), Point(-1,-1), 1);
    
    tmpMask2.copyTo(mask);
    
}





void CompareMasks(Mat& RGBmask, Mat& mask1, Mat& mask2, Mat& mask3, Mat& mask4)
{
    CV_Assert( mask1.size().width==mask2.size().width && mask1.size().width==mask3.size().width && mask1.size().width>0 &&
              mask1.size().height==mask2.size().height && mask1.size().height==mask3.size().height && mask1.size().height==mask4.size().height&& mask1.size().height>0 );
    
    CV_Assert( mask1.type()==mask2.type() && mask1.type()==mask3.type() && mask1.type()==mask4.type()&& mask1.type()==CV_8UC1);
    
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat maskContours, tmpMask;
    Mat RGBchannels[3];
    
    int idx;
    
    RGBchannels[0].create(mask3.size(), CV_8UC1);
    RGBchannels[0].setTo(0);
    RGBchannels[1].create(mask3.size(), CV_8UC1);
    RGBchannels[1].setTo(0);
    RGBchannels[2].create(mask3.size(), CV_8UC1);
    RGBchannels[2].setTo(0);

    maskContours.create(mask3.size(), CV_8UC1);
    tmpMask.create(mask3.size(), CV_8UC1);
    tmpMask.setTo(0);
    
    
    compare(mask1, 255, tmpMask, CMP_EQ);
    //tmpMask.copyTo(RGBchannels[0]);  // Blue
    RGBchannels[0].setTo(255, tmpMask);  // Blue

    //mask2.copyTo(RGBchannels[1]);  // Green
    RGBchannels[1].setTo(255, mask2);  // Green

    //mask3.copyTo(RGBchannels[2]);  // Red
    RGBchannels[2].setTo(255, mask3);  // Red
   
    compare(mask1, 170, tmpMask, CMP_EQ);
    RGBchannels[0].setTo(170, tmpMask);  // Gray
    RGBchannels[1].setTo(170, tmpMask);  // Gray
    RGBchannels[2].setTo(170, tmpMask);  // Gray

    
    tmpMask.setTo(0);
    mask3.copyTo(tmpMask);
    findContours(tmpMask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    maskContours.setTo(0);
    if (contours.size() != 0)
    {
        for (idx=0; idx >= 0; idx = hierarchy[idx][0] )
        {
            drawContours(maskContours, contours, idx, 255, 1, LINE_8, hierarchy);
        }
    }
    merge(RGBchannels, 3, RGBmask);
    
}

