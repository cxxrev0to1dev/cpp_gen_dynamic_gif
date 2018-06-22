#ifndef GIF_GIF_SERVICES_LIB_H_
#define GIF_GIF_SERVICES_LIB_H_

#include <cstdint>
#include <string>

void PollutionStringGIFImage(std::uint32_t length,std::uint32_t width,std::uint32_t height,const std::string& out_file,std::string& text_capcha);
void DynamicStringGIFImage(std::uint32_t length, std::uint32_t width, std::uint32_t height, const std::string& out_file, std::string& text_capcha);

#endif

