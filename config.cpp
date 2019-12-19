//
//  config.cpp
//
//  Created by Isabel Martins on 13/01/13.
//  Copyright (c) 2013 Isabel Martins. All rights reserved.
//
#include <iostream>

#include <stdio.h>
#include <string.h>


#include "config.hpp"



/***********************************************************CommentBegin******
 *
 * -- ReadConfigFile -- Reads the contents of the parameters file on disk
 *
 * Purpose :	Configuration parameters are read from the config file on disk
 *
 * Arguments in :	char *config_file - name of the configuration file
 *
 ***********************************************************CommentEnd********/
void  ReadConfigFile(char * config_file, Config *cfg)
{
    FILE	*fp;
    char 	c, str[FILENAME_MAX_LEN];
    int		i_par;
    double  d_par;
    
    if((fp = fopen(config_file,"r")) == NULL)	{
        std::cout << "ERROR[ReadConfigFile()] : could not open " << config_file << "\n\n";
        exit(1);
    }
    
    /* Read parameters */
    
    /* Input sequence file */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%s", str);
    cfg->PutConfigInputSeq(str);
    
    /* Input media type */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%s", str);
    cfg->PutConfigInputMediaType(str);
    
    /* First image */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutConfigFirstFrame(i_par);
    
    /* Last image */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutConfigLastFrame(i_par);
    
    /* Skip */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutConfigFrameSkip(i_par);
    
    /* Name for output files */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%s", str);
    cfg->PutConfigOutputFiles(str);
    
    /* Output media type */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%s", str);
    cfg->PutConfigOutputMediaType(str);
    
    /* Save output flag */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%s", str);
    cfg->PutConfigSaveOutput(str);
    
    /* Display output flag */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%s", str);
    cfg->PutConfigDisplayOutput(str);
    
    /* Retina parameters configuration file */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%s", str);
    cfg->PutConfigRetinaFile(str);
    
    /* Use retina low resolution */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%s", str);
    cfg->PutRetinaLowResolution(str);
    
    /* Magno Changed Threshold */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutMagnoChangedThreshold(i_par);
    
    /* Magno Min Object Size */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutMagnoMinObjectSize(i_par);
    
    
    
        /* COLIN Pattern */
        do
            fscanf(fp, "%c", &c);
        while(c != '#') ;
        fscanf(fp, "%d", &i_par);
        cfg->PutColinPattern(i_par);

         /* Use COLIN LPF */
         do
             fscanf(fp, "%c", &c);
         while(c != '#') ;
         fscanf(fp, "%s", str);
         cfg->PutColinUseLPF(str);

    
    
    /* AngleMOG NMixtures */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutAngleMOGNMixtures(i_par);
    
    /* AngleMOG VarThreshold */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOGVarThreshold_L(d_par);
 
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOGVarThreshold_A(d_par);

    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOGVarThreshold_B(d_par);

    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOGVarThreshold_Delta(d_par);
    
    
    /* AngleMOG BackgroundRatio */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOGBackgroundRatio(d_par);
    
    /* AngleMOG VarThresholdGen */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOGVarThresholdGen(d_par);
    
    /* AngleMOG VarInit */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOGVarInit(d_par);
    
    /* AngleMOG History */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutAngleMOGHistory(i_par);
    
    /* AngleMOG ComplexityReductionThreshold */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOGComplexityReductionThreshold(d_par);
    
    /* AngleMOG Learning Rates */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOG_BG_LearningRate(d_par);

    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOG_UBG_LearningRate(d_par);

    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOG_FG_LearningRate(d_par);

    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->PutAngleMOG_StepUBG_LearningRate(d_par);

    
    /* Dark Areas and Stopped Objects Thresholds */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->Put_Dark_Threshold(d_par);
    
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%lf", &d_par);
    cfg->Put_Stopped_Threshold(d_par);
    
    
    /* AngleMOG Post Processing Median Filter Size */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutAngleMOGPostProcMedianFilterSize(i_par);
    
    /* AngleMOG Post Processing Minimum Object Size */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutAngleMOGPostProcMinObjectSize(i_par);
 
    
    /* Final Post Processing Median Filter Size */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutFinalPostProcMedianFilterSize(i_par);
    
    
    /* AngleMOG transient frames */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutAngleMOGTransientFrames(i_par);
    
    /* AngleMOG Temporal analysis (prev/next frames) */
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutAngleMOGTemporalAnalysis(i_par);
    
    /* AngleMOG Spatial analysis (prev/next pixels)*/
    do
        fscanf(fp, "%c", &c);
    while(c != '#') ;
    fscanf(fp, "%d", &i_par);
    cfg->PutAngleMOGSpatialAnalysis(i_par);
    
    fclose(fp);
}




/***********************************************************CommentBegin******
 *
 * -- PrintConfig -- Prints the contents of a Config to stdout
 *
 * Purpose :	To check that correct parameters are being inputted
 *
 * Arguments in : 	Config *cfg - pointer to Config structure
 *
 ***********************************************************CommentEnd********/

void PrintConfig(Config *cfg)
{
    
    fprintf(stdout,"\n\n*****       Configuration Parameters           *****\n\n");
    
    fprintf(stdout,"Name of input sequence : %s\n",cfg->GetConfigInputSeq());
    fprintf(stdout,"Input media type (image/video) : %s\n",cfg->GetConfigInputMediaType());
    fprintf(stdout,"First frame : %d\n",cfg->GetConfigFirstFrame());
    fprintf(stdout,"Last frame : %d\n",cfg->GetConfigLastFrame());
    fprintf(stdout,"Frame skip : %d\n",cfg->GetConfigFrameSkip());
    fprintf(stdout,"Name of output files : %s\n",cfg->GetConfigOutputFiles());
    fprintf(stdout,"Output media type (image/video) : %s\n",cfg->GetConfigOutputMediaType());
    fprintf(stdout,"Save output (y/n) : %s\n",cfg->GetConfigSaveOuput());
    fprintf(stdout,"Display output (y/n) : %s\n\n",cfg->GetConfigDisplayOuput());
    
    fprintf(stdout,"Retina parameters configuration file : %s\n",cfg->GetConfigRetinaFile());
    fprintf(stdout,"Use retina low resolution (y/n) : %s\n",cfg->GetRetinaLowResolution());
    fprintf(stdout,"Magno channel changed threshold : %d\n\n",cfg->GetMagnoChangedThreshold());
    fprintf(stdout,"Magno channel minimum object size : %d\n\n",cfg->GetMagnoMinObjectSize());
    
    fprintf(stdout,"COLIN Pattern : %d\n\n",cfg->GetColinPattern());
    fprintf(stdout,"Use COLIN LPF (y/n) : %s\n",cfg->GetColinUseLPF());
   
    fprintf(stdout,"AngleMOG NMixtures : %d\n\n",cfg->GetAngleMOGNMixtures());
    fprintf(stdout,"AngleMOG VarThreshold L : %lf\n\n",cfg->GetAngleMOGVarThreshold_L());
    fprintf(stdout,"AngleMOG VarThreshold A: %lf\n\n",cfg->GetAngleMOGVarThreshold_A());
    fprintf(stdout,"AngleMOG VarThreshold B: %lf\n\n",cfg->GetAngleMOGVarThreshold_B());
    fprintf(stdout,"AngleMOG VarThreshold Delta: %lf\n\n",cfg->GetAngleMOGVarThreshold_Delta());
    fprintf(stdout,"AngleMOG BackgroundRatio : %lf\n\n",cfg->GetAngleMOGBackgroundRatio());
    fprintf(stdout,"AngleMOG VarThresholdGen : %lf\n\n",cfg->GetAngleMOGVarThresholdGen());
    fprintf(stdout,"AngleMOG VarInit : %lf\n\n",cfg->GetAngleMOGVarInit());
    fprintf(stdout,"AngleMOG History : %d\n\n",cfg->GetAngleMOGHistory());
    fprintf(stdout,"AngleMOG ComplexityReductionThreshold : %lf\n\n",cfg->GetAngleMOGComplexityReductionThreshold());
 
    fprintf(stdout,"AngleMOG BG LearningRate: %lf\n\n",cfg->GetAngleMOG_BG_LearningRate());
    fprintf(stdout,"AngleMOG UBG LearningRate: %lf\n\n",cfg->GetAngleMOG_UBG_LearningRate());
    fprintf(stdout,"AngleMOG FG LearningRate: %lf\n\n",cfg->GetAngleMOG_FG_LearningRate());
    fprintf(stdout,"AngleMOG Step UBG LearningRate: %lf\n\n",cfg->GetAngleMOG_StepUBG_LearningRate());

    fprintf(stdout,"Dark Areas Threshold: %lf\n\n",cfg->Get_Dark_Threshold());
    fprintf(stdout,"Stopped Objects Threshold: %lf\n\n",cfg->Get_Stopped_Threshold());

    fprintf(stdout,"AngleMOG Post Processing Median Filter Size(0=n) : %d\n\n",cfg->GetAngleMOGPostProcMedianFilterSize());
    fprintf(stdout,"AngleMOG Post Processing Minumum Object Size : %d\n\n",cfg->GetAngleMOGPostProcMinObjectSize());
    
    fprintf(stdout,"Final Post Processing Median Filter Size(0=n) : %d\n\n",cfg->GetFinalPostProcMedianFilterSize());
    
    fprintf(stdout,"AngleMOG Transient Frames : %d\n\n",cfg->GetAngleMOGTransientFrames());
    fprintf(stdout,"AngleMOG Temporal Analysis (prev/next frames) : %d\n\n",cfg->GetAngleMOGTemporalAnalysis());
    fprintf(stdout,"AngleMOG Spatial Analysis (prev/next pixels) : %d\n\n",cfg->GetAngleMOGSpatialAnalysis());
    
}




