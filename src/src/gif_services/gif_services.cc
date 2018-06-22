#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include "gif_plugin/gif_plugin.h"
#include "gif/gif_writer.h"

void Test(gdImagePtr image, gdImagePtr image1, std::uint32_t vmargin, std::uint32_t ysize) {
  std::string label = "Y-Axis Label";
  std::uint32_t labelfont = 5;

  // pixel-width of label
  std::uint32_t txtsz = GIFServices::imagefontwidth(labelfont) * label.size();
  // pixel-height of label
  std::uint32_t txtht = GIFServices::imagefontheight(labelfont);

  // define temp image
  gdImagePtr labelimage = gdImageCreate(txtsz, txtsz);
  std::uint32_t white = gdImageColorAllocate(labelimage, 0xFF, 0xFF, 0xFF);
  std::uint32_t black = gdImageColorAllocate(labelimage, 0x00, 0x00, 0x00);

  // write to the temp image
  gdFontPtr font_ptr1 = GIFServices::GIF::php_find_gd_font(labelfont);
  gdImageString(labelimage, font_ptr1, 0, txtsz / 2 - txtht / 2, (unsigned char*)label.c_str(), black);

  // rotate the temp image
  gdImagePtr labelimage1 = gdImageRotateInterpolated(labelimage, 90, white);

  // copy the temp image back to the real image
  gdImageCopy(image, image1, 3, vmargin + ysize / 2 - txtsz / 2, txtsz / 2 - txtht / 2, 0, txtht, txtsz);

  // destroy temp images, clear memory
  gdImageDestroy(labelimage);
  gdImageDestroy(labelimage1);
}
std::string random_string(size_t length)
{
  static auto& chrs = "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  thread_local static std::mt19937 rg{ std::random_device{}() };
  thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

  std::string s;

  s.reserve(length);

  while (length--)
    s += chrs[pick(rg)];

  return s;
}
int RandomInt() {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(1, INT_MAX);
  return dist6(rng);
}
class PollutionStringGIF:private GIFServices::GIFPluginDK,GIFServices::GIFPluginOptions
{
public:
  PollutionStringGIF():GIFPluginDK(this){
    CreateFrames();
  }
  virtual std::string text() {
    return "adasda";
  }
  virtual std::uint32_t textSize() {
    return 5;
  }
  virtual GIFServices::GIF::GIFRGB* textColor(){
    srand(time(nullptr));
    static GIFServices::GIF::GIFRGB rgb1 = { 0xff, 0x00, 0x00 };
    return &rgb1;
  }
  virtual GIFServices::GIF::GIFRGB* backgroundColor() {
    srand(time(nullptr));
    static GIFServices::GIF::GIFRGB rgb2 = { 0x00, 0x00, 0x00 };
    return &rgb2;
  }
  virtual GIFServices::GIF::GIFRGB* foregroundColor() {
    srand(time(nullptr));
    static GIFServices::GIF::GIFRGB rgb3 = { 0xff, 0xff, 0xff };
    return &rgb3;
  }
  virtual std::uint32_t windowWidth() {
    return 0;
  }
  virtual std::uint32_t TextPosition() {
    return 10;
  }
  virtual std::uint32_t GetImageWidth() {
    return 100;
  }
  virtual std::uint32_t GetImageHeight() {
    return 40;
  }
  virtual void CreateFrames() {
    std::uint32_t imageWidth = GetImageWidth();
    std::uint32_t imageHeight = GetImageHeight();
    GIFPluginDK::Create(imageWidth, imageHeight);
    for (std::uint32_t i = 0; i < imageWidth; i++) {
      gdImagePtr image = GIFPluginDK::GetBaseImage();
      std::uint32_t black = GIFPluginDK::GetColor(image, foregroundColor());
      gdImageFilledRectangle(image, 0, 0, imageWidth, imageHeight, black);
      std::string text1 = random_string(text().size());
      gdFontPtr font_ptr1 = GIFServices::GIF::php_find_gd_font(textSize());
      std::uint32_t text_color = GetColor(GIFPluginDK::frame_image(), textColor());
      srand(time(nullptr));
      std::uint32_t first_string_hight_distance = (RandomInt() % 10);
      while (first_string_hight_distance==0){
        first_string_hight_distance = (RandomInt() % 10);
      }
      std::uint32_t first_string_hight_posation = floor(RandomInt() / (imageWidth * imageHeight));
      if (i>floor(imageWidth / 2)){
        static std::uint32_t index = 0;
        while (index>floor(imageWidth / 2)){
          index = RandomInt();
        }
        text1 = random_string(floor(text().size()));
        gdImageString(GIFPluginDK::frame_image(), font_ptr1, (index * 2), first_string_hight_posation, (unsigned char*)text1.c_str(), text_color);
        gdImageString(GIFPluginDK::frame_image(), font_ptr1, (index * 3), floor(RandomInt() / (imageWidth *imageHeight)) + (GetImageHeight() / 2), (unsigned char*)text1.c_str(), text_color);
        index++;
        text1 = random_string(floor(text().size()));
      }
      gdImageStringUp(GIFPluginDK::frame_image(), font_ptr1, i + windowWidth(), i, (unsigned char*)text1.c_str(), text_color);
      gdImageString(GIFPluginDK::frame_image(), font_ptr1, (i*3) + windowWidth(), first_string_hight_posation, (unsigned char*)text1.c_str(), text_color);
      gdImageString(GIFPluginDK::frame_image(), font_ptr1, (i*4) + windowWidth(), RandomInt() % imageHeight, (unsigned char*)text1.c_str(), text_color);
      gdImageString(GIFPluginDK::frame_image(), font_ptr1, i, TextPosition(), (unsigned char*)text().c_str(), text_color);
      gdImageStringUp(GIFPluginDK::frame_image(), font_ptr1, (i * 2) + windowWidth(), (i * 4), (unsigned char*)text1.c_str(), text_color);
      GIFPluginDK::AddImage();
    }
  }
  std::vector<std::uint8_t> frames() {
    return GIFPluginDK::frames();
  }
};
class DynamicStringGIF :private GIFServices::GIFPluginDK, GIFServices::GIFPluginOptions
{
public:
  DynamicStringGIF() :GIFPluginDK(this) {
    CreateFrames();
  }
  virtual std::string text() {
    return "adasda";
  }
  virtual std::uint32_t textSize() {
    return 5;
  }
  virtual GIFServices::GIF::GIFRGB* textColor() {
    srand(time(nullptr));
    static GIFServices::GIF::GIFRGB rgb1 = { 0xff, 0x00, 0x00 };
    return &rgb1;
  }
  virtual GIFServices::GIF::GIFRGB* backgroundColor() {
    srand(time(nullptr));
    static GIFServices::GIF::GIFRGB rgb2 = { 0x00, 0x00, 0x00 };
    return &rgb2;
  }
  virtual GIFServices::GIF::GIFRGB* foregroundColor() {
    srand(time(nullptr));
    static GIFServices::GIF::GIFRGB rgb3 = { 0xff, 0xff, 0xff };
    return &rgb3;
  }
  virtual std::uint32_t windowWidth() {
    return 0;
  }
  virtual std::uint32_t TextPosition() {
    return 10;
  }
  virtual std::uint32_t GetImageWidth() {
    return 100;
  }
  virtual std::uint32_t GetImageHeight() {
    return 40;
  }
  virtual void CreateFrames() {
    std::uint32_t imageWidth = GetImageWidth();
    std::uint32_t imageHeight = GetImageHeight();
    GIFPluginDK::Create(imageWidth, imageHeight);
    for (std::uint32_t i = 0; i < imageWidth; i++) {
      gdImagePtr image = GIFPluginDK::GetBaseImage();
      std::uint32_t black = GIFPluginDK::GetColor(image, foregroundColor());
      gdImageFilledRectangle(image, 0, 0, imageWidth, imageHeight, black);
      std::string text1 = random_string(text().size()*2);
      gdFontPtr font_ptr1 = GIFServices::GIF::php_find_gd_font(textSize());
      std::uint32_t text_color = GetColor(GIFPluginDK::frame_image(), textColor());
      srand(time(nullptr));
      for (int ii1 = 0; ii1 < 256; ii1++) {
        unsigned char ss[2] = { text1[ii1] ,0x0 };
        gdImageString(image, GIFServices::GIF::php_find_gd_font(textSize()), RandomInt() % imageWidth, RandomInt() % imageHeight, ss, text_color);
      }
      gdImageString(image, GIFServices::GIF::php_find_gd_font(textSize()), i, TextPosition(), (unsigned char*)text().c_str(), text_color);
      GIFPluginDK::AddImage();
    }
  }
  std::vector<std::uint8_t> frames() {
    return GIFPluginDK::frames();
  }
};
void PollutionStringGIFImage() {
  PollutionStringGIF* moving_rectangle = new PollutionStringGIF;
  std::vector<std::uint8_t> frames_bytes = moving_rectangle->frames();
  std::ofstream ofs;
  ofs.open("D:\\workspace\\gif_solution_demo\\bin\\Debug\\PollutionString.gif", std::ofstream::out | std::ofstream::binary);
  std::vector<std::uint8_t> ss = frames_bytes;
  ofs.write((const char*)&frames_bytes[0], frames_bytes.size());
  ofs.close();
  delete moving_rectangle;
}
void DynamicStringGIFImage() {
  DynamicStringGIF* moving_rectangle = new DynamicStringGIF;
  std::vector<std::uint8_t> frames_bytes = moving_rectangle->frames();
  std::ofstream ofs;
  ofs.open("D:\\workspace\\gif_solution_demo\\bin\\Debug\\DynamicStringGIF.gif", std::ofstream::out | std::ofstream::binary);
  std::vector<std::uint8_t> ss = frames_bytes;
  ofs.write((const char*)&frames_bytes[0], frames_bytes.size());
  ofs.close();
  delete moving_rectangle;
}
int main(){
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(1, UINT_MAX);
  std::cout << dist6(rng) << std::endl;
  PollutionStringGIFImage();
  DynamicStringGIFImage();
  return 0;
}

