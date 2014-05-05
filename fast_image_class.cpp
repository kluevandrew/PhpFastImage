//============================================================================
// Name        : fast_image_class.cpp
// Author      : Kluev Andrew
// Version     : 1.0
// License     : BSD
// Description : Fast image processing library depended on OpenCV
//============================================================================

#include "fast_image_class.h"
#include <phpcpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int FastImage::round(float number)
{
	return (int) number < 0.0 ? std::ceil(number - 0.5) : std::floor(number + 0.5);
}

std::vector<int> FastImage::calculateResizedImageSize(int targetWidth, int targetHeight)
	{
		   std::vector<int> size(2);

		   int realWidth = getWidth();
		   int realHeight = getHeight();

		   float aspectRatio = (float) realWidth / (float) realHeight;

		   if (targetWidth == 0) {
			   targetWidth = round((float) targetHeight * aspectRatio);
		   } else if (targetHeight == 0) {
			   targetHeight = round((float) targetWidth * aspectRatio);
		   }

		   int newWidth  = realWidth;
		   int newHeight = realHeight;

		   if (newWidth > targetWidth || newHeight > targetHeight) {
			   if (newWidth > targetWidth && newHeight > targetHeight) {
				   newWidth = targetWidth;
				   newHeight = round((float)newWidth / aspectRatio);
				   if (newHeight > targetHeight) {
					   newHeight = targetHeight;
					   newWidth = round((float)newHeight * aspectRatio);
				   }
			   } else {
				   if (newWidth > targetWidth) {
					   newWidth = targetWidth;
					   newHeight = round((float) newWidth / aspectRatio);
				   } else {
					   newHeight = targetHeight;
					   newWidth = round((float) newHeight * aspectRatio);
				   }
			   }
		   }

		   size[0] = newWidth;
		   size[1] = newHeight;

		   return size;
	};

/**
 *  PHP Constructor
 *  @param Php::Parameters &parameters
 */
void FastImage::__construct(Php::Parameters &parameters)
{
	Php::Value self(this);

	Php::Value input = parameters[0];

	inputFile = input.stringValue();
	cvImage = cv::imread(inputFile);

	if (cvImage.empty()) {
		std::string message = (std::string)"FastImage: File "+ inputFile + (std::string)" not found.";

		throw Php::Exception(message);
	}
}

/**
 *  PHP Destructor
 */
void FastImage::__destruct()
{
	cvImage.release();
}

Php::Value FastImage::getWidth()
{
	Php::Value width = cvImage.size().width;

	return width;
}

Php::Value FastImage::getHeight()
{
	Php::Value height = cvImage.size().height;

	return height;
}

Php::Value FastImage::resize(Php::Parameters &parameters)
{
	Php::Value self(this);

	int width  = parameters[0];
	int height = parameters[1];
	bool save_ascpect_ratio = true;

	if (parameters.size() == 3) {
		save_ascpect_ratio =  parameters[2];
	}

	if (save_ascpect_ratio) {
	   std::vector<int> calculated_size = calculateResizedImageSize(width, height);
	   width  = calculated_size[0];
	   height = calculated_size[1];
	}

	cv::Mat resizedImage;
	if (width > 0 && height > 0) {
	  cv::Size new_size;
	  new_size.width = width;
	  new_size.height = height;
	  cv::resize(cvImage, resizedImage, new_size);
	} else {
		throwToSmallException("FastImage::resize");
	}

	cvImage = resizedImage;
	resizedImage.release();

	return self;
}

Php::Value FastImage::save(Php::Parameters &parameters)
{
    Php::Value self(this);

    Php::Value output = parameters[0];

    Php::Value flags;
    if (parameters.size() == 2) {
    	flags = parameters[1];
    }

    cv::imwrite(output, cvImage, flags);

    return self;
}

Php::Value FastImage::crop(Php::Parameters &parameters)
{
	Php::Value self(this);

	int x = parameters[0];
	int y = parameters[1];
	int w = parameters[2];
	int h = parameters[3];

    if (x < 0) throwMustBePositiveOrNilException("1", "FastImage::crop", parameters[0].stringValue());
    if (y < 0) throwMustBePositiveOrNilException("2", "FastImage::crop", parameters[1].stringValue());
    if (w <= 0) throwMustBePositiveException("3", "FastImage::crop", parameters[2].stringValue());
    if (h <= 0) throwMustBePositiveException("4", "FastImage::crop", parameters[3].stringValue());

    if ((x + w) > cvImage.size().width) {
    	throwOutOfBoundException("FastImage::crop");
    }

    if ((y +h) > cvImage.size().height) {
         	throwOutOfBoundException("FastImage::crop");
    }

	cv::Rect roi(x, y, w, h);
	cv::Mat croppedImage = cvImage(roi);

	cvImage = croppedImage;
	croppedImage.release();

	return self;
}

void FastImage::throwMustBePositiveException(std::string argNo, std::string method, std::string given)
{
	std::string message = (std::string)
			"FastImage: Argument "+ argNo + " of method " + method + " must be positive integer, "+given+ " given";
	;

	throw Php::Exception(message);
}

void FastImage::throwMustBePositiveOrNilException(std::string argNo, std::string method, std::string given)
{
	std::string message = (std::string)
			"FastImage: Argument "+ argNo + " of method " + method + " must be positive integer or 0, "+given+ " given";
	;

	throw Php::Exception(message);
}

void FastImage::throwOutOfBoundException(std::string method)
{
	std::string message = (std::string)
			"FastImage: Can\'t execute  " + method + ". Reason: Out of bounds.";
	;

	throw Php::Exception(message);
}

void FastImage::throwToSmallException(std::string method)
{
	std::string message = (std::string)
			"FastImage: Target size of method "+ method + " to small.";
	;

	throw Php::Exception(message);
}
