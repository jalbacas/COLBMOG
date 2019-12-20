 /*M////////////////////////////////////////////////////////////////////////////////////
 //
 // Copyright (c) 2019, Isabel Martins (mis@isep.ipp.pt)
 // All rights reserved.
 // Created by Isabel Martins on 10/01/18.
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
