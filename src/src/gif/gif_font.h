#ifndef GIF_GIF_FONT_H_
#define GIF_GIF_FONT_H_

#include "gif/gif_struct.h"
// #include "gif/gif_font_giant.h"
// #include "gif/gif_font_large.h"
// #include "gif/gif_font_medium_bold.h"
// #include "gif/gif_font_small.h"
// #include "gif/gif_font_tiny.h"
#include <third_party/libgd/src/gdfontt.h>  /* 1 Tiny font */
#include <third_party/libgd/src/gdfonts.h>  /* 2 Small font */
#include <third_party/libgd/src/gdfontmb.h> /* 3 Medium bold font */
#include <third_party/libgd/src/gdfontl.h>  /* 4 Large font */
#include <third_party/libgd/src/gdfontg.h>  /* 5 Giant font */

namespace GIFServices {
  namespace GIF {
    enum class ImageFontTable {
      ImageFontWidth = 0, 
      ImageFontHeight
    };
    static gdFontPtr php_find_gd_font(int size)
    {
      gdFontPtr font;
      switch (size) {
      case 1:
        font = gdFontTiny;
        break;
      case 2:
        font = gdFontSmall;
        break;
      case 3:
        font = gdFontMediumBold;
        break;
      case 4:
        font = gdFontLarge;
        break;
      case 5:
        font = gdFontGiant;
        break;
      default:
        break;
      }
      return font;
    }
  }
  static std::uint32_t php_imagefontsize(std::uint32_t size,GIF::ImageFontTable image_font){
    gdFontPtr font = GIF::php_find_gd_font(size);
    return (image_font== GIF::ImageFontTable::ImageFontHeight ? font->h : font->w);
  }
  static std::uint32_t imagefontheight(std::uint32_t size){
    return php_imagefontsize(size, GIF::ImageFontTable::ImageFontHeight);
  }
  static std::uint32_t imagefontwidth(std::uint32_t size){
    return php_imagefontsize(size, GIF::ImageFontTable::ImageFontWidth);
  }
}

#endif
