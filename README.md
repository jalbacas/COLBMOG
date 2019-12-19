# Texture collinearity foreground segmentation for night videos

## Paper authors:
Isabel Martins, Pedro Carvalho, Luıs Corte-Real, Jose Luis Alba-Castro

*(paper under review for Computer Vision and Image Understanding)*

## Abstract

One of the most dificult scenarios for unsupervised segmentation of moving objects is found in nighttime
videos where the main challenges are the poor illumination conditions resulting in low-visibility
of objects, very strong lights, surface-reflected light, a great variance of light intensity, sudden illumination
changes, hard shadows, camouflaged objects, and noise. This paper proposes a novel method,
coined COLBMOG (COLlinearity Boosted MOG), devised specifically for the foreground segmentation
in nighttime videos, that shows the ability to overcome some of the limitations of state-of-the-art
methods and still perform well in daytime scenarios. It is a texture-based classification method, using
local texture modeling, complemented by a color-based classification method. The local texture at
the pixel neighborhood is modeled as an N-dimensional vector. For a given pixel, the classification is
based on the collinearity between this feature in the input frame and the reference background frame.
For this purpose, a multimodal temporal model of the collinearity between texture vectors of background
pixels is maintained. COLBMOG was objectively evaluated using the ChangeDetection.net
(CDnet) 2014, Night Videos category, benchmark. COLBMOG ranks first among all the unsupervised
methods. A detailed analysis of the results revealed the superior performance of the proposed method
compared to the best performing state-of-the-art methods in this category, particularly evident in the
presence of the most complex situations where all the algorithms tend to fail.

## Requirements and Dependencies

* OpenCV 3.X (it works with OpenCV 4.X with simple changes in CV_ names)
* It compiles seamlessly both with XCode and MSVisual Studio with the proper links to OpenCV libraries
* The executable just needs the config_file.txt that includes the path and name of the input video and the set of parameters that control the behaviour of the foreground segmentation algorithm (with the default values)

## Testing the COLBMOG algorithm

This algorithm has been tested on the category of Night Videos of the CDNet 2014 dataset: http://jacarini.dinf.usherbrooke.ca/dataset2014/
ranking first (by december 2019) among all the non-supervised algorithms: http://jacarini.dinf.usherbrooke.ca/results2014/

