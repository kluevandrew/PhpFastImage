//============================================================================
// Name        : php_fimage.cpp
// Author      : Kluev Andrew
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <phpcpp.h>
#include "fast_image_class.h"

extern "C" {

    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module()
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension PhpFastImage("FastImage", "1.0");

        // description of the class so that PHP knows which methods are accessible
        Php::Class<FastImage> fastImage("FastImage");

        // register the methods
        fastImage.method("__construct", &FastImage::__construct, {
        		Php::ByVal("string", Php::Type::String),
        });

        fastImage.method("__destruct", &FastImage::__destruct);

        fastImage.method("save", &FastImage::save, {
        		Php::ByVal("string", Php::Type::String),
        });

        fastImage.method("resize", &FastImage::resize, {
                Php::ByVal("width", Php::Type::Numeric),
                Php::ByVal("height", Php::Type::Numeric),
                Php::ByVal("save_aspect_ratio", Php::Type::Numeric, false),
         });

        fastImage.method("crop", &FastImage::crop, {
                Php::ByVal("x", Php::Type::Numeric),
                Php::ByVal("y", Php::Type::Numeric),
                Php::ByVal("w", Php::Type::Numeric),
                Php::ByVal("h", Php::Type::Numeric),
         });

        fastImage.method("getWidth", &FastImage::getWidth, {});
        fastImage.method("getHeight", &FastImage::getHeight, {});

        fastImage.property("FAST_IMAGE_JPEG_QUALITY", FAST_IMAGE_JPEG_QUALITY, Php::Const);
        fastImage.property("FAST_IMAGE_PNG_COMPRESSION", FAST_IMAGE_PNG_COMPRESSION, Php::Const);
		fastImage.property("FAST_IMAGE_PXM_BINARY", FAST_IMAGE_PXM_BINARY, Php::Const);

        PhpFastImage.add(std::move(fastImage));

        // return the extension
        return PhpFastImage;
    }
}

