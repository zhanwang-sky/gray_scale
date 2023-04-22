//
//  main.cpp
//  gray_scale
//
//  Created by jichen on 2023/4/19.
//

#include <fstream>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

constexpr unsigned int bit_depth = 16;
constexpr unsigned int img_width = 2560;
constexpr unsigned int img_height = 255;

unsigned char img_buf[img_width * img_height * 6];

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./gray_scale <output.ppm>\n";
    exit(EXIT_FAILURE);
  }

  if (bit_depth < 1 || bit_depth > 16 || img_width < 2 || img_height < 1) {
    cerr << "Invalid parameters: bit_depth=" << bit_depth
         << ", img_width=" << img_width << ", img_height=" << img_height << endl;
    exit(EXIT_FAILURE);
  }

  unsigned int c_max = 1U << bit_depth;
  unsigned int scale = 65536 / (c_max - 1U);

  cout << c_max << endl;
  cout << scale << endl;

  unsigned int pos = 0;
  for (unsigned int i = 0; i != img_height; ++i) {
    for (unsigned int j = 0; j != img_width; ++j) {
      unsigned int c = j * c_max / (img_width - 1) * scale;
      c = c > UINT16_MAX ? UINT16_MAX : c;
//      if (i == 0) {
//        cout << '[' << j << ']' << c << endl;
//      }
      // sRGB color space
      img_buf[pos++] = c >> 8;
      img_buf[pos++] = c & 0xff;
      img_buf[pos++] = c >> 8;
      img_buf[pos++] = c & 0xff;
      img_buf[pos++] = c >> 8;
      img_buf[pos++] = c & 0xff;
    }
  }

  std::ofstream ofs(argv[1], std::ofstream::binary | std::ofstream::trunc);
  if (!ofs.is_open()) {
    cerr << "Fail to open \"" << argv[1] << "\" for writing\n";
    exit(EXIT_FAILURE);
  }

  ofs << "P6 " << img_width << ' ' << img_height << " 65535\n";
  ofs.write((const char*) img_buf, sizeof(img_buf));

  cout << "Done.\n";

  return 0;
}
