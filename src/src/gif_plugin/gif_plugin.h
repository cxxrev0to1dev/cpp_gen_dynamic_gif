#ifndef GIF_GIF_PLUGIN_H_
#define GIF_GIF_PLUGIN_H_

#include <ctime>
#include <cstdint>
#include <string>
#include <vector>
#include "gif/gif_font.h"
#include "gif/gif_struct.h"
#include "third_party/libgd/src/gd.h"

namespace GIFServices {
  class GIFPluginOptions
  {
  public:
    virtual std::string text() = 0;
    virtual std::uint32_t textSize() = 0;
    virtual GIF::GIFRGB* textColor() = 0;
    virtual GIF::GIFRGB* backgroundColor() = 0;
    virtual GIF::GIFRGB* foregroundColor() = 0;
    virtual std::uint32_t windowWidth() = 0;
    virtual std::uint32_t TextPosition() = 0;
    virtual std::uint32_t GetImageWidth() = 0;
    virtual std::uint32_t GetImageHeight() = 0;
    virtual void CreateFrames() = 0;
  };
  class GIFPluginDK
  {
  public:
    explicit GIFPluginDK(GIFPluginOptions* options){
      options_ = options;
      frames_out_ = gdNewDynamicCtx(2048, NULL);
      frame_prev_ = nullptr;
    }
    ~GIFPluginDK(){
      if (frames_out_){
        gdImageGifAnimEndCtx(frames_out_);
        frames_out_->gd_free(frames_out_);
        frames_out_ = NULL;
      }
      if (frame_prev_){
        gdImageDestroy(frame_prev_);
        frame_prev_ = NULL;
      }
    }
  protected:
    void Create(std::uint32_t image_width,std::uint32_t image_height) {
      frame_image_ = gdImageCreateTrueColor(image_width, image_height);
      gdImageGifAnimBeginCtx(frame_image_, frames_out_, 1, 100000);
      //black background
      std::uint32_t sss = GetColor(frame_image_, options_->backgroundColor());
      gdImageFilledRectangle(frame_image_, 0, 0, image_width, image_height, sss);
    }
    std::uint32_t GetColor(gdImagePtr image, GIFServices::GIF::GIFRGB* color){
      return gdImageColorAllocate(image, color->r, color->g, color->b);
    }
    gdImagePtr GetBaseImage()
    {
      srand(time(nullptr));
      std::uint32_t imageWidth = options_->GetImageWidth();
      std::uint32_t imageHeight = options_->GetImageHeight();
      frame_image_ = gdImageCreate(imageWidth, imageHeight);
      GetColor(frame_image_, options_->foregroundColor());
      return frame_image_;
    }
    void AddImage() {
      if (frame_prev_) {
        gdImageGifAnimAddCtx(frame_image_, frames_out_, 1, 0, 0, 30, 1, frame_prev_);
        gdImageDestroy(frame_prev_);
        frame_image_ = nullptr;
      }
      frame_prev_ = frame_image_;
    }
    virtual std::vector<std::uint8_t> frames() {
      std::uint32_t size = 0;
      std::uint8_t* cv = (std::uint8_t*)gdDPExtractData(frames_out_, (int*)&size);
      if (size){
        frames_.resize(size);
        memmove(&frames_[0], cv, size);
      }
      return frames_;
    }
    gdImagePtr frame_image() const {
      return frame_image_;
    }
  private:
    GIFPluginOptions* options_;
    std::vector<std::uint8_t> frames_;
    gdIOCtx* frames_out_;
    gdImagePtr frame_prev_;
    gdImagePtr frame_image_;

  };
}

#endif

