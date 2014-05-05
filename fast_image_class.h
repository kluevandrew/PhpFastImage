//============================================================================
// Name        : fast_image_class.h
// Author      : Kluev Andrew
// Version     : 1.0
// License     : BSD
// Description : Fast image processing library depended on OpenCV
//============================================================================

#ifndef FAST_IMAGE_CLASS_H_
#define FAST_IMAGE_CLASS_H_

#include <phpcpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

const int FAST_IMAGE_JPEG_QUALITY    = CV_IMWRITE_JPEG_QUALITY;
const int FAST_IMAGE_PNG_COMPRESSION = CV_IMWRITE_PNG_COMPRESSION;
const int FAST_IMAGE_PXM_BINARY      = CV_IMWRITE_PXM_BINARY;

class FastImage : public Php::Base
{
private:
	cv::Mat cvImage;
	std::string inputFile;

	std::vector<int> calculateResizedImageSize(int targetWidth, int targetHeight);
	int round(float number);

	void throwToSmallException(std::string method);
	void throwOutOfBoundException(std::string method);
	void throwMustBePositiveException(std::string argNo, std::string method, std::string given);
	void throwMustBePositiveOrNilException(std::string argNo, std::string method, std::string given);

public:
    /**
     *  c++ constructor
     */
	FastImage() {}

    /**
     *  c++ destructor
     */
    virtual ~FastImage() {}

    void __construct(Php::Parameters &parameters);
    void __destruct();

    Php::Value getWidth();
    Php::Value getHeight();
    Php::Value save(Php::Parameters &parameters);

    Php::Value resize(Php::Parameters &parameters);
    Php::Value crop(Php::Parameters &parameter);
};

#endif /* FAST_IMAGE_CLASS_H_ */
