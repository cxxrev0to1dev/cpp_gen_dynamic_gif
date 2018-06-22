#ifndef GIF_GIF_STRUCT_H_
#define GIF_GIF_STRUCT_H_

#include <string>
#include <vector>
#include <cstdint>

namespace GIFServices{
  namespace GIF {
    class GIFHeaders {
    public:
      enum class GIFVersionTable { kGIF2, kGIF3 };
      std::string gif_magic;
      std::string gif_version;
      std::vector<std::uint8_t> gif_buf;
      std::uint32_t LOP = 0;
      std::uint32_t DIS = 2;
      std::uint32_t COL = -1;
      std::uint32_t IMG = -1;
    };
    typedef struct
    {
      std::uint32_t r;
      std::uint32_t g;
      std::uint32_t b;
      char* data;
    }GIFRGB;
    typedef struct
    {
      std::uint32_t nchars;
      std::uint32_t offset;
      std::uint32_t w;
      std::uint32_t h;
      char* data;
    }GIFFont;
    typedef GIFFont* GIFFontPtr;
  }
}

#endif