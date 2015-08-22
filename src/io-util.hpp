#pragma once

#include <iostream>
#include <cstring>

namespace fbs2jsonl {

inline bool load_buffer(std::istream &in, std::string *buf) {
  *buf = std::string(std::istreambuf_iterator<char>(in),
                    std::istreambuf_iterator<char>());
  return !in.bad();
}

inline void buffer_copy(const void *data, size_t size, std::string &dst) {
	dst.resize(size);
	memcpy(&dst[0], data, size);
}

bool store_string(std::ostream &out, const std::string &data);

bool load_string(std::istream &in, std::string &str);

} // namespace
