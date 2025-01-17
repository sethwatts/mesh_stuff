#include "zlib.h"

#include <string>
#include <vector>
#include <iostream>

static std::vector<uint8_t> compress(const std::vector<uint8_t>& uncompressed_data) {
  unsigned long uncompressed_bytes = uncompressed_data.size();
  unsigned long compressed_bytes = compressBound(uncompressed_bytes);
  std::vector<uint8_t> compressed_data(compressed_bytes); // allocate enough size for output buffer
  int error = compress((Bytef *)&compressed_data[0], &compressed_bytes, 
                       (Bytef *)&uncompressed_data[0], uncompressed_bytes);

  compressed_data.resize(compressed_bytes);
  return compressed_data;
}

int main()
{

  std::vector<uint8_t> bytes = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,63,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,63,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,63,0,0,0,0,0,0,240,63,0,0,0,0,0,0,240,63,0,0,0,0,0,0,240,63,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,64,0,0,0,0,0,0,240,63,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,0,0,0,240,63,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,64,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,64,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,64,0,0,0,0,0,0,240,63,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,64,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,64,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,64,0,0,0,0,0,0,0,64};
  auto compressed = compress(bytes);

  std::cout << compressed.size() << " bytes" << std::endl;
  int count = 0;
  for (auto c : compressed) {
    std::cout << std::dec << count++ << ": " << std::hex << uint16_t(c) << std::endl;
  }

}