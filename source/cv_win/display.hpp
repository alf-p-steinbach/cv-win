#pragma once    // Source encoding: UTF-8 with BOM (π is a lowercase Greek "pi").
#include <winapi-header-wrappers/windows-h.hpp>
#include <opencv2/opencv.hpp>

#include <assert.h>

#include <cv-win/conversions.hpp>

namespace cv_win {
    inline void display_aligned_bgr( const HDC dc, const cv::Mat& image )
    {
        constexpr int pixels_per_meter = 2835;  // 72 DPI × 39.3701 inch/meter. Is ignored.

        assert( image.channels() == 3 );                    // BGR format.
        assert( CV_MAT_DEPTH( image.type() ) == CV_8U );    // Of bytes.
        assert( image.step % 4 == 0 );                      // With 4 byte row alignment.

        const int logical_width     = image.cols;
        const int physical_width    = int( image.step / 3 );   assert( image.step % 3 == 0 );
        const int height            = image.rows;

        BITMAPINFO bmpinfo = {};
        bmpinfo.bmiHeader.biSize            = sizeof (BITMAPINFOHEADER );
        bmpinfo.bmiHeader.biBitCount        = 24;
        bmpinfo.bmiHeader.biClrImportant    = 0;
        bmpinfo.bmiHeader.biClrUsed         = 0;
        bmpinfo.bmiHeader.biCompression     = BI_RGB;
        bmpinfo.bmiHeader.biWidth           = physical_width;
        bmpinfo.bmiHeader.biHeight          = -height;
        bmpinfo.bmiHeader.biPlanes          = 1;
        bmpinfo.bmiHeader.biSizeImage       = 0;
        bmpinfo.bmiHeader.biXPelsPerMeter   = pixels_per_meter;
        bmpinfo.bmiHeader.biYPelsPerMeter   = pixels_per_meter;

        //const auto& h = bmpinfo.bmiHeader;
        //const int stride = ((((h.biWidth * h.biBitCount) + 31) & ~31) >> 3);
        //const int cvstep = image.step;
        //assert( stride == cvstep );

        const int n_scan_lines_set = SetDIBitsToDevice(
            dc,                 // hdc
            0,                  // xDest
            0,                  // yDest
            logical_width,      // w
            height,             // h
            0,                  // xSrc
            0,                  // ySrc
            0,                  // StartScan
            height,             // cLines
            image.data,         // lpvBits
            &bmpinfo,           // lpbmi
            DIB_RGB_COLORS      // ColorUse
        );
        (void) n_scan_lines_set;
    }

    inline void display_bgr( const HDC dc, const cv::Mat& image )
    {
        assert( image.channels() == 3 );                    // BGR format.
        assert( CV_MAT_DEPTH( image.type() ) == CV_8U );    // Of bytes.    // TODO: Handle 16-bit images.

        const int stepmod = image.step % 4;
        if( stepmod == 0 ) {
            display_aligned_bgr( dc, image );
        } else {
            display_aligned_bgr( dc, wincompatible_mat_from( image ) );
        }
    }
}  // namespace cv_win
