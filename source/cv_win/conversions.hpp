#pragma once    // Source encoding: UTF-8 with BOM (π is a lowercase Greek "pi").

#include <cppx-core-language/all.hpp>
#include <winapi-header-wrappers/windows-h.hpp>
#include <opencv2/opencv.hpp>

#include <assert.h>

namespace cv_win {
    $use_cppx( Array_span_, Byte );

    inline auto wincompatible_mat_from( const cv::Mat image )
        -> cv::Mat
    {
        const int stepmod = image.step % 4;
        if( stepmod == 0 ) {
            return image;
        } else {
            const int colsmod = image.cols % 4;
            cv::Mat aligned_image_data( image.rows, image.cols + (4 - colsmod), image.type() );
            cv::Mat aligned_image = aligned_image_data( cv::Rect( 0, 0, image.cols, image.rows ) );
            image.copyTo( aligned_image );
            return aligned_image;
        }
    }

    inline auto mat_from( const Array_span_<const Byte>& image_data )
        -> cv::Mat
    {
        const int n_bytes = int( image_data.size() );
        const int arity = 1;
        const auto cv_image_data = cv::Mat(
            arity,
            &n_bytes,
            CV_8UC1,
            const_cast<void*>( static_cast<const void*>( image_data.first() ) )
        );
        return cv::imdecode( cv_image_data, cv::IMREAD_COLOR | cv::IMREAD_ANYDEPTH );
    }
}  // namespace cv_win
