#pragma once    // Source encoding: UTF-8 with BOM (π is a lowercase Greek "pi").
#include <winapi-integration/Abstract_image_displayer.hpp>
#include <cv-win/display.hpp>

namespace cv_win {
    $use_cppx( Size );
    using winapi::integration::Abstract_image_displayer;

    class Image_displayer:
        public Abstract_image_displayer
    {
        mutable cv::Mat     m_image;

    public:
        auto width() const
            -> Size override
        { return m_image.cols; }

        auto height() const
            -> Size override
        { return m_image.rows; }

        void display_on( const HDC dc ) const
            override
        {
            display_bgr( dc, m_image );
        }

        void stretch_display_on( const HDC dc, const RECT& rect ) const
            override
        {
            $is_unused( dc );  $is_unused( rect );
            $fail( "Not implemented yet" );
        }
    };

}  // namespace cv_win
