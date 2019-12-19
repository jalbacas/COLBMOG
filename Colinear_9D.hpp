/*M///////////////////////////////////////////////////////////////////////////////////////
 //
 //  Copyright (c) 2016 Isabel Martins. All rights reserved.
 //  Created by Isabel Martins on 10/10/16.
 /⁄
 ///////////////////////////////////////////////////////////////////////////////////////*/

#ifndef COLINEAR_9D_HPP_
#define COLINEAR_9D_HPP_

#include "opencv2/core.hpp"

using namespace cv;
using namespace std;


#ifndef M_PI
#define M_PI        3.14159265358979323846264338327950288   /* pi */
#endif


/** Namespace containing all necessary objects and methods for COLINEAR_9D */
namespace COLINEARITY_9D
{
    
    
    class TextureVectors {
        
            uchar ***prev9DL;
            uchar ***curr9DL;
            uchar ***prev9Da;
            uchar ***curr9Da;
            uchar ***prev9Db;
            uchar ***curr9Db;
        
            Mat bkgMean_L;
            Mat bkgMean;
            Mat bkgStDev_L;
            Mat bkgStDev;
      
            Mat colinearity_L;
            //Mat colinearity_a, colinearity_b;

            Mat anglesMat;
        

        public:
        
            TextureVectors(Size frameSize);
        
            ~TextureVectors();

            void apply(InputArray _inputImg, InputArray _bkgImg, int _Pattern, bool _useLPF);

            void getAnglesMat(OutputArray outputAnglesMat);
        
            void getBkgTextureMeansMat(OutputArray outputBkgMeansMat);

            void Create9Dvectors(Mat& img_in, uchar*** img9D, int pattern);

            void Colinearity_9D_stdev(Size framesize, uchar*** img9Dprev, uchar*** img9Dcurr, Mat& colinearity);

            void Colinearity_9D_angle(Size framesize, uchar*** img9Dprev, uchar*** img9Dcurr, Mat& colinearity);
        
            void bkg_9D_mean_stDev(Size framesize, uchar*** img9Dprev, Mat& bkgMean, Mat& bkgStDev);
        
    

    };  // class TextureVectors
    
    
    
    /** @brief Creates TextureVectors
    */
    Ptr<TextureVectors>  createTextureVectors(Size frameSize);
    
}

#endif /* COLINEAR_9D_HPP_ */
