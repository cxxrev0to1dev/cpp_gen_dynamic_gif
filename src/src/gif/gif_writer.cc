#include "gif\gif_Writer.h"
#include <fstream>


namespace GIFServices {
  namespace GIF {
    template <class Type>
    Type substr(const char* str, std::uint32_t start, std::uint32_t size) {
      Type result;
      result.resize(size);
      memmove(&result[0], &str[start], size);
      return result;
    }
    template <class Type1, class Type2>
    Type1 append(const Type1& dst, const Type2& src) {
      Type1 result = dst;
      result.insert(result.end(), src.begin(), src.end());
      return result;
    }
    GIFWriter::GIFWriter(std::vector<std::uint8_t> gif_src, std::vector<std::uint8_t> gif_dly,
      std::uint32_t gif_lop, std::uint32_t gif_dis,
      std::uint32_t gif_red, std::uint32_t gif_grn,
      std::uint32_t gif_blu, std::vector<std::vector<std::uint8_t>> gif_ofs,
      std::string gif_mod) {
      std::string s = "GIF89a";
      gif_ = append<std::vector<std::uint8_t>, std::string>(gif_, s);
      img_ = -1;
      if (gif_mod == "bin") {
        gif_src_bytes_ = gif_src;
        gif_src_size_ = gif_src_bytes_.size();
      }
      Init(gif_dly, gif_lop, gif_dis, gif_red, gif_grn, gif_blu, gif_ofs, gif_mod);
    }
    GIFWriter::GIFWriter(std::vector<std::string> gif_src, std::vector<std::uint8_t> gif_dly,
      std::uint32_t gif_lop, std::uint32_t gif_dis,
      std::uint32_t gif_red, std::uint32_t gif_grn,
      std::uint32_t gif_blu, std::vector<std::vector<std::uint8_t>> gif_ofs,
      std::string gif_mod
    ) {
      std::string s = "GIF89a";
      gif_ = append<std::vector<std::uint8_t>, std::string>(gif_, s);
      img_ = -1;
      if (gif_mod == "url") {
        gif_src_url_ = gif_src;
        gif_src_size_ = gif_src_url_.size();
      }
      Init(gif_dly, gif_lop, gif_dis, gif_red, gif_grn, gif_blu, gif_ofs, gif_mod);
    }
    GIFWriter::~GIFWriter() {
    }
    void GIFWriter::Init(std::vector<std::uint8_t> gif_dly,
      std::uint32_t gif_lop, std::uint32_t gif_dis,
      std::uint32_t gif_red, std::uint32_t gif_grn,
      std::uint32_t gif_blu, std::vector<std::vector<std::uint8_t>> gif_ofs,
      std::string gif_mod) {
      if (!gif_dly.size()) {
        return;
      }
      if (gif_ofs.size()) {
        sig_ = 1;
        ofs_ = gif_ofs;
      }
      lop_ = (gif_lop > -1) ? gif_lop : 0;
      dis_ = (gif_dis > -1) ? ((gif_dis < 3) ? gif_dis : 3) : 2;
      col_ = (gif_red > -1 && gif_grn > -1 && gif_blu > -1) ?
        (gif_red | (gif_grn << 8) | (gif_blu << 16)) : -1;
      for (std::uint8_t i = 0; i < gif_src_size_; i++) {
        if (gif_mod == "url") {
          std::fstream fs(gif_src_url_[i], std::ios::in | std::ios::binary);
          if (!fs.is_open())
            return;
          fs.seekg(0, std::ios::end);
          std::streampos ps = fs.tellg();
          fs.seekg(0, std::ios::beg);
          buf_.resize((size_t)ps);
          fs.read((char *)&buf_[0], ps);
        }
        else if (gif_mod == "bin") {
          buf_.insert(buf_.end(),gif_src_bytes_.begin(), gif_src_bytes_.end());
        }
        else {
          return;
        }
        if (strncmp((const char*)&buf_[i], "GIF87a", 6) != 0 && strncmp((const char*)&buf_[i], "GIF89a", 6) != 0) {
          return;
        }
        bool k;
        for (std::uint32_t j = (13 + 3 * (2 << (buf_[i] & 0x07))), k = true; k==true; j++) {
          std::uint8_t* sss = &buf_[i];
          switch (sss[j]) {
          case '!':
            if (strncmp((const char*)buf_[i*(j + 3)], "NETSCAPE", 8) != 0) {
              return;
            }
            break;
          case ';':
            k = false;
            break;
          }
        }
        AddHeaders();
        for (std::uint32_t i = 0; i < buf_.size(); i++) {
          AddFrames(i, gif_dly[i]);
        }
        AddFooter();
        break;
      }
    }
    void GIFWriter::AddHeaders() {
      std::uint32_t cmap = 0;
      std::string s = "!\377\13NETSCAPE2.0\3\1";
      if (buf_[0] & 0x80) {
        cmap = 3 * (2 << (buf_[0] & 0x07));
        gif_.insert(gif_.end(), buf_.begin() + 6, buf_.begin() + 6 + 7);
        gif_.insert(gif_.end(), buf_.begin() + 13, buf_.begin() + 13 + cmap);
        gif_.insert(gif_.end(), s.begin(), s.end());
        gif_.insert(gif_.end(), 1, ((lop_ >> 0) & 0xFF));
        gif_.insert(gif_.end(), 1, ((lop_ >> 8) & 0xFF));
        gif_.insert(gif_.end(), 1, '\0');
      }
    }
    void GIFWriter::AddFrames(std::uint32_t i, std::uint32_t d) {
      std::uint32_t locals_str = 13 + 3 * (2 << (buf_[i] & 0x07));
      std::uint32_t locals_end = (buf_.size() - i) - locals_str - 1;
      if (locals_end<=locals_str||((locals_str + i)>=buf_.size())){
        return;
      }
      std::vector<std::uint8_t> Locals_tmp;
      Locals_tmp.resize(locals_end - locals_str);
      memmove(&Locals_tmp[0], &buf_[locals_str], locals_end - locals_str);
      std::uint32_t Global_len = 2 << (buf_[0] & 0x07);
      std::uint32_t Locals_len = 2 << (buf_[i] & 0x07);

      std::vector<std::uint8_t> Global_rgb;
      std::vector<std::uint8_t> Locals_rgb;
      Global_rgb.resize((3 * (2 << (buf_[0] & 0x07))));
      Locals_rgb.resize((3 * (2 << (buf_[i] & 0x07))));
      memmove(&Global_rgb[0], &buf_[13], (3 * (2 << (buf_[0] & 0x07))));
      memmove(&Locals_rgb[0], &buf_[i], (3 * (2 << (buf_[i] & 0x07))));
      std::string Locals_ext;
      Locals_ext.append("!\xF9\x04");
      Locals_ext.append(1, (dis_ << 2));
      Locals_ext.append(1, ((d >> 0) & 0xFF));
      Locals_ext.append(1, ((d >> 8) & 0xFF));
      Locals_ext.append("\x0\x0", 2);
      if (col_ > -1 && (buf_[i] & 0x80)) {
        for (std::uint32_t j = 0; j < (2 << (buf_[i] & 0x07)); j++) {
          if (Locals_rgb[3 * j + 0] == ((col_ >> 16) & 0xFF) &&
            (Locals_rgb[3 * j + 1] == ((col_ >> 8) & 0xFF)) &&
            (Locals_rgb[3 * j + 2] == ((col_ >> 0) & 0xFF))
            ) {
            Locals_ext.append("!\xF9\x04");
            Locals_ext.append(1, (dis_ << 2) + 1);
            Locals_ext.append(1, ((d >> 0) & 0xFF));
            Locals_ext.append(1, ((d >> 8) & 0xFF));
            Locals_ext.append("\x0", 1);
            break;
          }
        }
      }
      std::string Locals_img;
      switch (Locals_tmp[0]) {
      case '!':
        Locals_img = substr<std::string>((const char*)&Locals_tmp[0], 8, 10);
        Locals_tmp = substr<std::vector<std::uint8_t>>((const char*)&Locals_tmp[0], 18, Locals_tmp.size() - 18);
        break;
      case ',':
        Locals_img = substr<std::string>((const char*)&Locals_tmp[0], 0, 10);
        Locals_tmp = substr<std::vector<std::uint8_t>>((const char*)&Locals_tmp[0], 10, Locals_tmp.size() - 10);
        break;
      }
      if ((buf_[i] & 0x80) && img_ > -1) {
        if (Global_len == Locals_len) {
          if (GIFBlockCompare(Global_rgb, Locals_rgb, Global_len)) {
            gif_ = append<std::vector<std::uint8_t>, std::string>(gif_, Locals_ext);
            gif_ = append<std::vector<std::uint8_t>, std::string>(gif_, Locals_img);
            gif_ = append<std::vector<std::uint8_t>, std::vector<std::uint8_t>>(gif_, Locals_tmp);
          }
          else {
            if (sig_ == 1) {
              Locals_img[1] = (ofs_[i][0] & 0xFF);
              Locals_img[2] = ((ofs_[i][0] & 0xFF00) >> 8);
              Locals_img[3] = (ofs_[i][1] & 0xFF);
              Locals_img[4] = ((ofs_[i][1] & 0xFF00) >> 8);
            }
            std::uint8_t byte = Locals_img[9];
            byte |= 0x80;
            byte &= 0xF8;
            byte |= (buf_[0] & 0x07);
            Locals_img[9] = byte;
            gif_ = append<std::vector<std::uint8_t>, std::string>(gif_, Locals_ext);
            gif_ = append<std::vector<std::uint8_t>, std::string>(gif_, Locals_img);
            gif_ = append<std::vector<std::uint8_t>, std::vector<std::uint8_t>>(gif_, Locals_rgb);
            gif_ = append<std::vector<std::uint8_t>, std::vector<std::uint8_t>>(gif_, Locals_tmp);
          }
        }
        else {
          /*
          *
          * XY Padding...
          *
          */
          if (sig_ == 1) {
            Locals_img[1] = (ofs_[i][0] & 0xFF);
            Locals_img[2] = ((ofs_[i][0] & 0xFF00) >> 8);
            Locals_img[3] = (ofs_[i][1] & 0xFF);
            Locals_img[4] = ((ofs_[i][1] & 0xFF00) >> 8);
          }
          std::uint8_t byte = Locals_img[9];
          byte |= 0x80;
          byte &= 0xF8;
          byte |= (buf_[i] & 0x07);
          Locals_img[9] = byte;
          gif_ = append<std::vector<std::uint8_t>, std::string>(gif_, Locals_ext);
          gif_ = append<std::vector<std::uint8_t>, std::string>(gif_, Locals_img);
          gif_ = append<std::vector<std::uint8_t>, std::vector<std::uint8_t>>(gif_, Locals_rgb);
          gif_ = append<std::vector<std::uint8_t>, std::vector<std::uint8_t>>(gif_, Locals_tmp);
        }
      }
      else {
        gif_ = append<std::vector<std::uint8_t>, std::string>(gif_, Locals_ext);
        gif_ = append<std::vector<std::uint8_t>, std::string>(gif_, Locals_img);
        gif_ = append<std::vector<std::uint8_t>, std::vector<std::uint8_t>>(gif_, Locals_tmp);
      }
      img_ = 1;
    }
    void GIFWriter::AddFooter() {
      gif_.insert(gif_.end(), 1, ';');
    }
    bool GIFWriter::GIFBlockCompare(std::vector<std::uint8_t> GlobalBlock, std::vector<std::uint8_t> LocalBlock, std::uint32_t Len) {
      for (std::uint32_t i = 0; i < Len; i++) {
        if (
          GlobalBlock[3 * i + 0] != LocalBlock[3 * i + 0] ||
          GlobalBlock[3 * i + 1] != LocalBlock[3 * i + 1] ||
          GlobalBlock[3 * i + 2] != LocalBlock[3 * i + 2]
          ) {
          return false;
        }
      }

      return true;
    }
    void GIFWriter::InitGIFStruct(GIFHeaders::GIFVersionTable gif_version) {
      if (gif_version == GIFHeaders::GIFVersionTable::kGIF2)
        gif_headers_.gif_magic = "GIFEncoder V2.06";
      else if (gif_version == GIFHeaders::GIFVersionTable::kGIF3)
        gif_headers_.gif_magic = "GIFEncoder V3.00";
    }
  }
}