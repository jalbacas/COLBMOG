//
//  config.hpp
//
//  Created by Isabel Martins on 13/01/13.
//  Copyright (c) 2013 Isabel Martins. All rights reserved.
//

#ifndef __Retina__config__
#define __Retina__config__

//#include <iostream>

#ifndef FILENAME_MAX_LEN
#define FILENAME_MAX_LEN  350
#endif


class Config {
    
    /* general */
   	char	inputSeqFile[FILENAME_MAX_LEN];
    char	inputMediaType[10];
    int		firstFrame;
    int		lastFrame;
    int		frameSkip;
    char	outputFiles[FILENAME_MAX_LEN];
    char	outputMediaType[10];
    char	saveOutput[3];
    char	displayOutput[3];
    /* Retina and Retina Segm */
    char	retinaCfgFile[FILENAME_MAX_LEN];
    char	retinaLowResolution[3];
    int		MagnoChangedThreshold;
    int		MagnoMinObjectSize;
    
    /* Colinearity config */
    char	COLIN_use_LPF[3];
    int		COLIN_pattern;

    /* AngleMOG config */
    int		AngleMOG_NMixtures;
    double	AngleMOG_VarThreshold_L;
    double	AngleMOG_VarThreshold_A;
    double	AngleMOG_VarThreshold_B;
    double	AngleMOG_VarThreshold_Delta;
    double	AngleMOG_BackgroundRatio;
    double  AngleMOG_VarThresholdGen;
    double  AngleMOG_VarInit;
    int		AngleMOG_History;
    double	AngleMOG_ComplexityReductionThreshold;
    
    double	AngleMOG_BG_LearningRate;
    double	AngleMOG_UBG_LearningRate;
    double	AngleMOG_FG_LearningRate;
    double	AngleMOG_StepUBG_LearningRate;
    
    double	Dark_Threshold;
    double	Stopped_Threshold;

    int     AngleMOG_PostProcMedianFilterSize;
    int     AngleMOG_PostProcMinObjSize;
    int     AngleMOGTransientFrames;
 
    int     Final_PostProcMedianFilterSize;

    int     AngleMOGTemporalAnalysis;
    int     AngleMOGSpatialAnalysis;
    
    
public:
    
    char    *GetConfigInputSeq() {
        return inputSeqFile;
    }
    char    *GetConfigInputMediaType() {
        return inputMediaType;
    }
    int  	GetConfigFirstFrame() {
        return firstFrame;
    }
    int  	GetConfigLastFrame() {
        return lastFrame;
    }
    int  	GetConfigFrameSkip() {
        return frameSkip;
    }
    char    *GetConfigOutputFiles() {
        return outputFiles;
    }
    char    *GetConfigOutputMediaType() {
        return outputMediaType;
    }
    char    *GetConfigSaveOuput() {
        return saveOutput;
    }
    char    *GetConfigDisplayOuput() {
        return displayOutput;
    }
    
    char    *GetConfigRetinaFile() {
        return retinaCfgFile;
    }
    char    *GetRetinaLowResolution() {
        return retinaLowResolution;
    }
    int  	GetMagnoChangedThreshold() {
        return MagnoChangedThreshold;
    }
    int  	GetMagnoMinObjectSize() {
        return MagnoMinObjectSize;
    }
    

    int  	GetColinPattern() {
        return COLIN_pattern;
    }

    char    *GetColinUseLPF() {
        return COLIN_use_LPF;
    }

    int  	GetAngleMOGNMixtures() {
        return AngleMOG_NMixtures;
    }
    double  GetAngleMOGVarThreshold_L() {
        return AngleMOG_VarThreshold_L;
    }
    double  GetAngleMOGVarThreshold_A() {
        return AngleMOG_VarThreshold_A;
    }
    double  GetAngleMOGVarThreshold_B() {
        return AngleMOG_VarThreshold_B;
    }
    double  GetAngleMOGVarThreshold_Delta() {
        return AngleMOG_VarThreshold_Delta;
    }
    double  GetAngleMOGBackgroundRatio() {
        return AngleMOG_BackgroundRatio;
    }
    double  GetAngleMOGVarThresholdGen() {
        return AngleMOG_VarThresholdGen;
    }
    double  GetAngleMOGVarInit() {
        return AngleMOG_VarInit;
    }
    int  	GetAngleMOGHistory() {
        return AngleMOG_History;
    }
    double  GetAngleMOGComplexityReductionThreshold() {
        return AngleMOG_ComplexityReductionThreshold;
    }
    
    double  GetAngleMOG_BG_LearningRate() {
        return AngleMOG_BG_LearningRate;
    }
    double  GetAngleMOG_UBG_LearningRate() {
        return AngleMOG_UBG_LearningRate;
    }
    double  GetAngleMOG_FG_LearningRate() {
        return AngleMOG_FG_LearningRate;
    }
    double  GetAngleMOG_StepUBG_LearningRate() {
        return AngleMOG_StepUBG_LearningRate;
    }
    
    double  Get_Dark_Threshold() {
        return Dark_Threshold;
    }
    double  Get_Stopped_Threshold() {
        return Stopped_Threshold;
    }
    
    int     GetAngleMOGPostProcMedianFilterSize() {
        return AngleMOG_PostProcMedianFilterSize;
    }
    int     GetAngleMOGPostProcMinObjectSize() {
        return AngleMOG_PostProcMinObjSize;
    }
    
    int     GetFinalPostProcMedianFilterSize() {
        return Final_PostProcMedianFilterSize;
    }

    int     GetAngleMOGTransientFrames() {
        return AngleMOGTransientFrames;
    }
    int     GetAngleMOGTemporalAnalysis() {
        return AngleMOGTemporalAnalysis;
    }
    int     GetAngleMOGSpatialAnalysis() {
        return AngleMOGSpatialAnalysis;
    }
    
    
    
    void    PutConfigInputSeq(char *in_seq) {
        strcpy(inputSeqFile, in_seq);
    }
    void    PutConfigInputMediaType(char *in_media_type) {
        strcpy(inputMediaType, in_media_type);
    }
    void    PutConfigFirstFrame(int start_frame) {
        firstFrame = start_frame;
    }
    void    PutConfigLastFrame(int max_frame) {
        lastFrame = max_frame;
    }
    void    PutConfigFrameSkip(int frame_skip) {
        frameSkip = frame_skip;
    }
    void    PutConfigOutputFiles(char *out_seq) {
        strcpy(outputFiles, out_seq);
    }
    void    PutConfigOutputMediaType(char *out_media_type) {
        strcpy(outputMediaType, out_media_type);
    }
    void    PutConfigSaveOutput(char *save_output) {
        strcpy(saveOutput, save_output);
    }
    void    PutConfigDisplayOutput(char *display_output) {
        strcpy(displayOutput, display_output);
    }
    
    void    PutConfigRetinaFile(char *retina_file) {
        strcpy(retinaCfgFile, retina_file);
    }
    void    PutRetinaLowResolution(char *retina_low_res) {
        strcpy(retinaLowResolution, retina_low_res);
    }
    void    PutMagnoChangedThreshold(int thresh) {
        MagnoChangedThreshold = thresh;
    }
    void    PutMagnoMinObjectSize(int minSize) {
        MagnoMinObjectSize = minSize;
    }
    
    
    void PutColinPattern(int patt) {
        COLIN_pattern = patt;
    }
    void    PutColinUseLPF(char *use_lpf) {
        strcpy(COLIN_use_LPF, use_lpf);
    }
    
    
    void PutAngleMOGNMixtures(int NMix) {
        AngleMOG_NMixtures = NMix;
    }
    void  PutAngleMOGVarThreshold_L(double varThresh) {
        AngleMOG_VarThreshold_L = varThresh;
    }
    void  PutAngleMOGVarThreshold_A(double varThresh) {
        AngleMOG_VarThreshold_A = varThresh;
    }
    void  PutAngleMOGVarThreshold_B(double varThresh) {
        AngleMOG_VarThreshold_B = varThresh;
    }
    void  PutAngleMOGVarThreshold_Delta(double varThreshDelta) {
        AngleMOG_VarThreshold_Delta = varThreshDelta;
    }

    void  PutAngleMOGBackgroundRatio(double bkgRatio) {
        AngleMOG_BackgroundRatio = bkgRatio;
    }
    void  PutAngleMOGVarThresholdGen(double varThresgGen) {
        AngleMOG_VarThresholdGen = varThresgGen;
    }
    void  PutAngleMOGVarInit(double varInit) {
        AngleMOG_VarInit = varInit;
    }
    void  PutAngleMOGHistory(int nHist) {
        AngleMOG_History = nHist;
    }
    void  PutAngleMOGComplexityReductionThreshold(double redThresh) {
        AngleMOG_ComplexityReductionThreshold = redThresh;
    }
    
    void  PutAngleMOG_BG_LearningRate(double learnRate) {
        AngleMOG_BG_LearningRate = learnRate;
    }
    void  PutAngleMOG_UBG_LearningRate(double learnRate) {
        AngleMOG_UBG_LearningRate = learnRate;
    }
    void  PutAngleMOG_FG_LearningRate(double learnRate) {
        AngleMOG_FG_LearningRate = learnRate;
    }
    void  PutAngleMOG_StepUBG_LearningRate(double learnRate) {
        AngleMOG_StepUBG_LearningRate = learnRate;
    }
    
    void  Put_Dark_Threshold(double thresh) {
        Dark_Threshold = thresh;
    }
    void  Put_Stopped_Threshold(double thresh) {
        Stopped_Threshold = thresh;
    }
    
    void  PutAngleMOGPostProcMedianFilterSize(int postProcFilter) {
        AngleMOG_PostProcMedianFilterSize = postProcFilter;
    }
    void  PutAngleMOGPostProcMinObjectSize(int postProcMinObj) {
        AngleMOG_PostProcMinObjSize = postProcMinObj;
    }
    
    void  PutFinalPostProcMedianFilterSize(int postProcFilter) {
        Final_PostProcMedianFilterSize = postProcFilter;
    }

    void  PutAngleMOGTransientFrames(int nframes) {
        AngleMOGTransientFrames = nframes;
    }
    void  PutAngleMOGTemporalAnalysis(int value) {
        AngleMOGTemporalAnalysis = value;
    }
    void  PutAngleMOGSpatialAnalysis(int value) {
        AngleMOGSpatialAnalysis = value;
    }
    
};


void  ReadConfigFile(char * config_file, Config *cfg);

void PrintConfig(Config *cfg);



#endif /* defined(__Retina__config__) */


