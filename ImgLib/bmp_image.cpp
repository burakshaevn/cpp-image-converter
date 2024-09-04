#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>
#include <iostream>
#include <vector>

using namespace std;

namespace img_lib {

    // функция вычисления отступа по ширине
    static int GetBMPStride(int w) {
        return 4 * ((w * 3 + 3) / 4);  // выравнивание по 4 байта
    }

    PACKED_STRUCT_BEGIN BitmapFileHeader{
     BitmapFileHeader(int width, int height, int stride)
         : bfType{'B', 'M'}
         , bfSize(static_cast<uint32_t>(14 + 40 + stride * height))  // общий размер файла
         , bfReserved(0)
         , bfOffBits(14 + 40)  // смещение от начала файла до начала данных изображения
     {}
     char bfType[2];
     uint32_t bfSize;
     uint32_t bfReserved;
     uint32_t bfOffBits;
    } PACKED_STRUCT_END;

    PACKED_STRUCT_BEGIN BitmapInfoHeader{
        BitmapInfoHeader(int width, int height, int stride)
            : biSize(40)
            , biWidth(width)
            , biHeight(height)
            , biPlanes(1)
            , biBitCount(24)
            , biCompression(0)
            , biSizeImage(static_cast<uint32_t>(stride * height))  // размер данных изображения
            , biXPelsPerMeter(11811)
            , biYPelsPerMeter(11811)
            , biClrUsed(0)
            , biClrImportant(0x1000000)
        {}
        uint32_t biSize;
        int32_t biWidth;
        int32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        int32_t biXPelsPerMeter;
        int32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    } PACKED_STRUCT_END;


    bool SaveBMP(const Path& file, const Image& image) {
        int width = image.GetWidth();
        int height = image.GetHeight();
        const int stride = GetBMPStride(width);

        BitmapFileHeader file_header{ width, height, stride };
        BitmapInfoHeader info_header{ width, height, stride };

        ofstream out(file, ios::binary);
        out.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));  
        out.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));  
        vector<char> buff(stride);
         
        for (int y = height - 1; y >= 0; --y) {
            const Color* line = image.GetLine(y);

            for (int x = 0; x < width; ++x) {
                buff[x * 3 + 0] = static_cast<char>(line[x].b);   
                buff[x * 3 + 1] = static_cast<char>(line[x].g);  
                buff[x * 3 + 2] = static_cast<char>(line[x].r);  
            }
            out.write(buff.data(), buff.size());
        }

        return out.good();
    }


    Image LoadBMP(const Path& file) {
        ifstream ifs(file, ios::binary);
        if (!ifs) {
            cerr << "Failed to open file for reading: " << file << endl;
            return {};
        }

        int32_t w, h;
        ifs.ignore(18); 

        ifs.read(reinterpret_cast<char*>(&w), sizeof(w));
        ifs.read(reinterpret_cast<char*>(&h), sizeof(h));
        if (!ifs.good() || w <= 0 || h == 0) {
            cerr << "Failed to read width/height or invalid values.\n";
            return {};
        }

        ifs.ignore(28); 

        int stride = GetBMPStride(w);
        Image result(w, abs(h), Color::Black());
        std::vector<char> buff(stride);

        for (int y = result.GetHeight() - 1; y >= 0; --y) {
            Color* line = result.GetLine(y);
            ifs.read(buff.data(), stride);

            for (int x = 0; x < w; ++x) {
                line[x].b = static_cast<byte>(buff[x * 3 + 0]);
                line[x].g = static_cast<byte>(buff[x * 3 + 1]);
                line[x].r = static_cast<byte>(buff[x * 3 + 2]);
            }
        }

        return result;
    }

}  // namespace img_lib
