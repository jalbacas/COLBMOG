/*M///////////////////////////////////////////////////////////////////////////////////////
 //
 //  Copyright (c) 2016 Isabel Martins. All rights reserved.
 //  Created by Isabel Martins on 10/10/16.
 /⁄
 ///////////////////////////////////////////////////////////////////////////////////////*/

/// opencv include files
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <cmath>
#include <iostream>

#include "Colinear_9D.hpp"


using namespace cv;
using namespace std;
using namespace COLINEARITY_9D;


namespace COLINEARITY_9D
{


constexpr double  deg2Rad(const double deg)
{
    return (deg * (M_PI / 180.0));
}

constexpr double rad2Deg(const double rad)
{
    return ((180.0 / M_PI) * rad);
}



//! constructor
TextureVectors::TextureVectors(Size frameSize)
{
    prev9DL = new uchar **[frameSize.height];
    curr9DL = new uchar **[frameSize.height];
    prev9Da = new uchar **[frameSize.height];
    curr9Da = new uchar **[frameSize.height];
    prev9Db = new uchar **[frameSize.height];
    curr9Db = new uchar **[frameSize.height];
    for (int r=0; r<frameSize.height; r++)
    {
        prev9DL[r]=new uchar *[frameSize.width];
        curr9DL[r]=new uchar *[frameSize.width];
        prev9Da[r]=new uchar *[frameSize.width];
        curr9Da[r]=new uchar *[frameSize.width];
        prev9Db[r]=new uchar *[frameSize.width];
        curr9Db[r]=new uchar *[frameSize.width];
        
        for (int c=0; c<frameSize.width; c++)
        {
            prev9DL[r][c]=new uchar[9];
            curr9DL[r][c]=new uchar[9];
            prev9Da[r][c]=new uchar[9];
            curr9Da[r][c]=new uchar[9];
            prev9Db[r][c]=new uchar[9];
            curr9Db[r][c]=new uchar[9];
            for (int d=0; d<9; d++)
            {
                prev9DL[r][c][d]= (uchar) 0;
                curr9DL[r][c][d]= (uchar) 0;
                prev9Da[r][c][d]= (uchar) 0;
                curr9Da[r][c][d]= (uchar) 0;
                prev9Db[r][c][d]= (uchar) 0;
                curr9Db[r][c][d]= (uchar) 0;
            }
        }
    }

    bkgMean_L.create(frameSize,CV_32FC1);
    bkgMean_L.setTo(0.0f);
    bkgStDev_L.create(frameSize,CV_32FC1);
    bkgStDev_L.setTo(0.0f);
    
    colinearity_L.create(frameSize,CV_32FC1);
    colinearity_L.setTo(0.0f);
    /*
     colinearity_a.create(frameSize,CV_32FC1);
     colinearity_a.setTo(0.0f);
     colinearity_b.create(frameSize,CV_32FC1);
     colinearity_b.setTo(0.0f);
     */

}


//! destructor
TextureVectors::~TextureVectors() {}


//! the operator
void TextureVectors::apply(InputArray _inputLab, InputArray _bkgImgLab, int Pattern, bool useLPF)
{
    Mat Lab[3], aux[3];
    
    Size frameSize = _inputLab.size();
    
    Mat inputLab = _inputLab.getMat();
    Mat bkgImgLab = _bkgImgLab.getMat();

    ///// split Lab color components of input frame and background image
    split(inputLab,  Lab);
    split(bkgImgLab, aux);
    
    ///// L component Low Pass Filtering
    if (useLPF)
    {
        GaussianBlur(Lab[0], Lab[0], Size(3,3), 0, 0);
        GaussianBlur(aux[0], aux[0], Size(3,3), 0, 0);
    }
    
    ///// create 9-dimensional texture vectors using a specific Pattern
    Create9Dvectors( Lab[0], curr9DL, Pattern);
    Create9Dvectors( aux[0], prev9DL, Pattern);
    
    //if ( frame_count>1 ) // if we have a background image
    {
        ///// texture vectors colinearity computation
        Colinearity_9D_angle( frameSize, prev9DL, curr9DL, colinearity_L);
        
        // copy the angles to channels a and b because currently AngleMOG is using 3 channels
        colinearity_L.copyTo(aux[0]);
        {   // repeats for channels a and b
            colinearity_L.copyTo(aux[1]);
            colinearity_L.copyTo(aux[2]);
        }
        merge(aux, 3, anglesMat);
 
        bkg_9D_mean_stDev( frameSize, prev9DL, bkgMean_L, bkgStDev_L);
        bkgMean_L.copyTo(bkgMean);
        bkgStDev_L.copyTo(bkgStDev);
        
        
    } // if (frame_count>1)

}


void TextureVectors::getAnglesMat(OutputArray outputAnglesMat)
{
    anglesMat.copyTo(outputAnglesMat);
}


void TextureVectors::getBkgTextureMeansMat(OutputArray outputBkgMeansMat)
{
    bkgMean.copyTo(outputBkgMeansMat);
}


/*******************************************************************************
 * Compute 9-D colinearity returning angle between two 9-D vectors in degrees
 ******************************************************************************/

void TextureVectors::Colinearity_9D_angle(Size framesize, uchar*** img9Dprev, uchar*** img9Dcurr, Mat& colinearity)
{

    colinearity.create( framesize, CV_32FC1 );
    colinearity.setTo(0.0);

    Mat* dst = &colinearity;
    float* dstPtr;
   
    int nrows = framesize.height;
    int ncols = framesize.width;


    for(int i = 0; i < nrows; i++)
    {
        dstPtr = dst->ptr<float>(i);
        
        for(int j = 0; j < ncols; j++)
        {
                double dotProduct=0.0, mag1=0.0, mag2=0.0;
                for (int k=0; k<9; k++)
                {
                    dotProduct += (double)(img9Dprev[i][j][k]* img9Dcurr[i][j][k]);
                    mag1 += (double)(img9Dprev[i][j][k]*img9Dprev[i][j][k]);
                    mag2 += (double)(img9Dcurr[i][j][k]*img9Dcurr[i][j][k]);
                }
                mag1 = sqrt(mag1);
                mag2 = sqrt(mag2);
    
                double angle = acos(dotProduct/(mag1*mag2));
                angle = rad2Deg(angle);
    
                dstPtr[j] = (float)angle;
           
        }
        
    }
 }


/*******************************************************************************
 * Compute 9-D colinearity returning stdDev of division of 9-D vectors
 ******************************************************************************/

void TextureVectors::Colinearity_9D_stdev(Size framesize, uchar*** img9Dprev, uchar*** img9Dcurr, Mat& colinearity)
{
    
    colinearity.create( framesize, CV_32FC1 );
    colinearity.setTo(0.0);
    
    Mat* dst = &colinearity;
    float* dstPtr;
    
    int nrows = framesize.height;
    int ncols = framesize.width;
    
    for(int i = 0; i < nrows; i++)
    {
        dstPtr = dst->ptr<float>(i);
        
        for(int j = 0; j < ncols; j++)
        {
                Scalar    mean;
                Scalar    std_dev;
                std::vector<float> vect_div(9);
                for (int k=0; k<9; k++)
                    vect_div[k] = ((float)img9Dprev[i][j][k]/(float)img9Dcurr[i][j][k]);
                cv::meanStdDev (vect_div, mean, std_dev );
                //dstPtr[j] = (float)std_dev.val[0];
                dstPtr[j] = static_cast<float>(std_dev.val[0]);
                //cout <<  "stDev:" << dstPtr[j] << endl;
        }
    }
    
}


    
    
    /*******************************************************************************
     * Compute 9-D vectors mean and stDev
     ******************************************************************************/
    
void TextureVectors::bkg_9D_mean_stDev(Size framesize, uchar*** img9D, Mat& bkgMeans, Mat& bkgStDevs)
{
    
    bkgMeans.create( framesize, CV_32FC1 );
    bkgMeans.setTo(0.0);

    bkgStDevs.create( framesize, CV_32FC1 );
    bkgStDevs.setTo(0.0);

    Mat* dstMean = &bkgMeans;
    float* dstMeanPtr;
    Mat* dstStDev = &bkgStDevs;
    float* dstStDevPtr;
    
    int nrows = framesize.height;
    int ncols = framesize.width;
    
    Scalar    mean;
    Scalar    std_dev;
    std::vector<float> vect_bkg(9);

    for(int i = 0; i < nrows; i++)
    {
        dstMeanPtr = dstMean->ptr<float>(i);
        dstStDevPtr = dstStDev->ptr<float>(i);
        
        for(int j = 0; j < ncols; j++)
        {
            for (int k=0; k<9; k++)
                vect_bkg[k] = (float)img9D[i][j][k];
            cv::meanStdDev (vect_bkg, mean, std_dev );
            //dstPtr[j] = (float)std_dev.val[0];
            dstMeanPtr[j] = static_cast<float>(mean.val[0]);
            dstStDevPtr[j] = static_cast<float>(std_dev.val[0]);
            //cout <<  "stDev:" << dstPtr[j] << endl;
        }
    }
        
}


/*******************************************************************************
* Compute 9-D colinearity returning stdDev of division of 9-D vectors
******************************************************************************/

void TextureVectors::Create9Dvectors(Mat& img_in,  uchar*** img9D,  int pattern)
{
    int i, j;
    int border;
    
    Mat bigImg;
    
    const Mat* src = &img_in;
    int nrows = src->rows;
    int ncols = src->cols;
    
    const uchar* srcPt_up2;
    const uchar* srcPt_up1;
    const uchar* srcPt;
    const uchar* srcPt_down1;
    const uchar* srcPt_down2;
    
    //cout << "pixel 1: " << (int) img9D[120][160][4] << endl;
   
    switch (pattern)
    {
        case 1:  // executes pattern 1
            //cout << "Pattern 1 \n";
            /*
             X X X
             X X X
             X X X
             */
            // let border be the same in all directions
            border=1;
            // constructs a larger image to fit both the image and the border
            bigImg.create(nrows + border*2, ncols + border*2, src->depth());
            // form a border in-place
            cv::copyMakeBorder(img_in, bigImg, border, border, border, border, BORDER_REPLICATE);
            src = &bigImg;
            nrows = src->rows;
            ncols = src->cols;

            for(i = 1; i < nrows-1; i++)
            {
                srcPt_up1 = src->ptr<uchar>(i-1);
                srcPt = src->ptr<uchar>(i);
                srcPt_down1 = src->ptr<uchar>(i+1);
                
                for(j = 1; j < ncols-1; j++)
                {
                    img9D[i-1][j-1][0] = *srcPt_up1;
                    img9D[i-1][j-1][1] = *(srcPt_up1+1);
                    img9D[i-1][j-1][2] = *(srcPt_up1+2);
                    img9D[i-1][j-1][3] = *srcPt;
                    img9D[i-1][j-1][4] = *(srcPt+1);
                    img9D[i-1][j-1][5] = *(srcPt+2);
                    img9D[i-1][j-1][6] = *srcPt_down1;
                    img9D[i-1][j-1][7] = *(srcPt_down1+1);
                    img9D[i-1][j-1][8] = *(srcPt_down1+2);
                    srcPt_up1++;
                    srcPt++;
                    srcPt_down1++;
                }
                
            }

        case 2: // executes pattern 2
                //cout << "Pattern 2 \n";
            /*
             X O X O X
             O O O O O
             X O X O X
             O O O O O
             X O X O X
             */
                // let border be the same in all directions
                border=2;
                // constructs a larger image to fit both the image and the border
                bigImg.create(nrows + border*2, ncols + border*2, src->depth());
                // form a border in-place
                cv::copyMakeBorder(img_in, bigImg, border, border, border, border, BORDER_REPLICATE);
                src = &bigImg;
                nrows = src->rows;
                ncols = src->cols;

                for(i = 2; i < nrows-2; i++)
                {
                    srcPt_up2 = src->ptr<uchar>(i-2);
                    srcPt = src->ptr<uchar>(i);
                    srcPt_down2 = src->ptr<uchar>(i+2);
                
                    for(j = 2; j < ncols-2; j++)
                    {
                        img9D[i-2][j-2][0] = *(srcPt_up2);
                        img9D[i-2][j-2][1] = *(srcPt_up2+2);
                        img9D[i-2][j-2][2] = *(srcPt_up2+4);
                        img9D[i-2][j-2][3] = *(srcPt);
                        img9D[i-2][j-2][4] = *(srcPt+2);
                        img9D[i-2][j-2][5] = *(srcPt+4);
                        img9D[i-2][j-2][6] = *(srcPt_down2);
                        img9D[i-2][j-2][7] = *(srcPt_down2+2);
                        img9D[i-2][j-2][8] = *(srcPt_down2+4);
                        srcPt_up2++;
                        srcPt++;
                        srcPt_down2++;
                    }
                
                }
                break;
                    
        case 3: // executes pattern 3
            //cout << "Pattern 3 \n";
            /*
                O O X O O
                O X O X O
                X O X O X
                O X O X O
                O O X O O
            */
            // let border be the same in all directions
            border=2;
            // constructs a larger image to fit both the image and the border
            bigImg.create(nrows + border*2, ncols + border*2, src->depth());
            // form a border in-place
            cv::copyMakeBorder(img_in, bigImg, border, border, border, border, BORDER_REPLICATE);
            src = &bigImg;
            nrows = src->rows;
            ncols = src->cols;
            
            for(i = 2; i < nrows-2; i++)
            {
                srcPt_up2 = src->ptr<uchar>(i-2);
                srcPt_up1 = src->ptr<uchar>(i-1);
                srcPt = src->ptr<uchar>(i);
                srcPt_down1 = src->ptr<uchar>(i+1);
                srcPt_down2 = src->ptr<uchar>(i+2);
                
                for(j = 2; j < ncols-2; j++)
                {
                    img9D[i-2][j-2][0] = *(srcPt_up2+2);
                    img9D[i-2][j-2][1] = *(srcPt_up1+1);
                    img9D[i-2][j-2][2] = *(srcPt_up1+3);
                    img9D[i-2][j-2][3] = *(srcPt);
                    img9D[i-2][j-2][4] = *(srcPt+2);
                    img9D[i-2][j-2][5] = *(srcPt+4);
                    img9D[i-2][j-2][6] = *(srcPt_down1+1);
                    img9D[i-2][j-2][7] = *(srcPt_down1+3);
                    img9D[i-2][j-2][8] = *(srcPt_down2+2);
                    srcPt_up2++;
                    srcPt_up1++;
                    srcPt++;
                    srcPt_down1++;
                    srcPt_down2++;
                }
            }
            break;

        default: // executes pattern 3 without borders
            //cout << "Pattern 3 \n";
            for(i = 2; i < nrows-2; i++)
            {
                srcPt_up2 = src->ptr<uchar>(i-2);
                srcPt_up1 = src->ptr<uchar>(i-1);
                srcPt = src->ptr<uchar>(i);
                srcPt_down1 = src->ptr<uchar>(i+1);
                srcPt_down2 = src->ptr<uchar>(i+2);
                
                for(j = 2; j < ncols-2; j++)
                {
                    img9D[i][j][0] = *(srcPt_up2+2);
                    img9D[i][j][1] = *(srcPt_up1+1);
                    img9D[i][j][2] = *(srcPt_up1+3);
                    img9D[i][j][3] = *(srcPt);
                    img9D[i][j][4] = *(srcPt+2);
                    img9D[i][j][5] = *(srcPt+4);
                    img9D[i][j][6] = *(srcPt_down1+1);
                    img9D[i][j][7] = *(srcPt_down1+3);
                    img9D[i][j][8] = *(srcPt_down2+2);
                    srcPt_up2++;
                    srcPt_up1++;
                    srcPt++;
                    srcPt_down1++;
                    srcPt_down2++;
                }
            }
     }
    
    
}


Ptr<TextureVectors> createTextureVectors(Size frameSize)
{
    return makePtr<TextureVectors>(frameSize);
}

}
