#ifndef GIF_GIF_WRITER_H_
#define GIF_GIF_WRITER_H_

#include "gif/gif_struct.h"

namespace GIFServices {
  namespace GIF {
    class GIFWriter {
    public:
      GIFWriter(std::vector<std::uint8_t> gif_src, std::vector<std::uint8_t> gif_dly,
        std::uint32_t gif_lop, std::uint32_t gif_dis,
        std::uint32_t gif_red, std::uint32_t gif_grn,
        std::uint32_t gif_blu, std::vector<std::vector<std::uint8_t>> gif_ofs,
        std::string gif_mod
      );
      GIFWriter(std::vector<std::string> gif_src, std::vector<std::uint8_t> gif_dly,
        std::uint32_t gif_lop, std::uint32_t gif_dis,
        std::uint32_t gif_red, std::uint32_t gif_grn,
        std::uint32_t gif_blu, std::vector<std::vector<std::uint8_t>> gif_ofs,
        std::string gif_mod
      );
      ~GIFWriter();
      std::vector<std::uint8_t> gif() {
        return gif_;
      }
    private:
      void Init(std::vector<std::uint8_t> gif_dly,
        std::uint32_t gif_lop, std::uint32_t gif_dis,
        std::uint32_t gif_red, std::uint32_t gif_grn,
        std::uint32_t gif_blu, std::vector<std::vector<std::uint8_t>> gif_ofs,
        std::string gif_mod);
      void AddHeaders();
      void AddFrames(std::uint32_t i, std::uint32_t d);
      void AddFooter();
      bool GIFBlockCompare(std::vector<std::uint8_t>, std::vector<std::uint8_t>, std::uint32_t);
      void InitGIFStruct(GIFHeaders::GIFVersionTable gif_version);
      GIFHeaders gif_headers_;
      std::vector<std::uint8_t> gif_src_bytes_;
      std::vector<std::string> gif_src_url_;
      std::uint32_t gif_src_size_;
      std::uint32_t sig_;
      std::uint32_t lop_;
      std::uint32_t dis_;
      std::uint32_t col_;
      std::uint32_t img_;
      std::vector<std::uint8_t> buf_;
      std::vector<std::vector<std::uint8_t>> ofs_;
      std::vector<std::uint8_t> gif_;
    };
  }
}

#endif
