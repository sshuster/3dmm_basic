/* Copyright (c) 2015 USC, IRIS, Computer vision Lab */
///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2012, Tadas Baltrusaitis, all rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//     * The software is provided under the terms of this licence stricly for
//       academic, non-commercial, not-for-profit purposes.
//     * Redistributions of source code must retain the above copyright notice, 
//       this list of conditions (licence) and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright 
//       notice, this list of conditions (licence) and the following disclaimer 
//       in the documentation and/or other materials provided with the 
//       distribution.
//     * The name of the author may not be used to endorse or promote products 
//       derived from this software without specific prior written permission.
//     * As this software depends on other libraries, the user must adhere to 
//       and keep in place any licencing terms of those libraries.
//     * Any publications arising from the use of this software, including but
//       not limited to academic journal and conference publications, technical
//       reports and manuals, must cite the following work:
//
//       Tadas Baltrusaitis, Peter Robinson, and Louis-Philippe Morency. 
//       Constrained Local Neural Fields for robust facial landmark detection in the wild.
//       in IEEE Int. Conference on Computer Vision Workshops, 300 Faces in-the-Wild Challenge, 2013.    
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED 
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
// EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////


#ifndef __CCNF_PATCH_EXPERT_h_
#define __CCNF_PATCH_EXPERT_h_

#include <map>
//#include <cv.h>
#include <opencv2/opencv.hpp>

using namespace cv;

namespace CLMTracker
{

//===========================================================================
/** 
	A single Neuron response
*/
class CCNF_neuron{

public:

	// Type of patch (0=raw,1=grad,3=depth, other types besides raw are not actually used now)
	int     neuron_type; 

	// scaling of weights (needed as the energy of neuron might not be 1) 
	double  norm_weights; 

	// Weight bias
	double  bias;

	// Neural weights
	cv::Mat_<float> weights; 

	// can have neural weight dfts that are calculated on the go as needed, this allows us not to recompute
	// the dft of the template each time, improving the speed of tracking
	std::map<int, cv::Mat> weights_dfts;

	// the alpha associated with the neuron
	double alpha; 

	CCNF_neuron(){;}

	void Read(std::ifstream &stream);
	// The im_dft, integral_img, and integral_img_sq are precomputed images for convolution speedups (they get set if passed in empty values)
	void Response(Mat_<float> &im, Mat &im_dft, Mat &integral_img, Mat &integral_img_sq, Mat_<double> &resp);

};

//===========================================================================
/**
A CCNF patch expert
*/
class CCNF_patch_expert{
public:
    
	// Width and height of the patch expert support region
	int width;
	int height;             
    
	// Collection of neurons for this patch expert
	std::vector<CCNF_neuron> neurons;

	// Information about the vertex features (association potentials)
	std::vector<int>				window_sizes;
	std::vector<Mat_<float> >		Sigmas;
	vector<double>					betas;

	// How confident we are in the patch
	double   patch_confidence;

	CCNF_patch_expert(){;}

	void Read(std::ifstream &stream, vector<int> window_sizes, vector<vector<Mat_<float> > > sigma_components);

	// actual work (can pass in an image and a potential depth image, if the CCNF is trained with depth)
	void Response(Mat_<float> &area_of_interest, Mat_<double> &response);    

	// Helper function to compute relevant sigmas
	void ComputeSigmas(std::vector<Mat_<float> > sigma_components, int window_size);
	
};
  //===========================================================================
}
#endif
