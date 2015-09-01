#include "fbs_tk.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <flatbuffers/flatbuffers.h>

using namespace flatbuffers;
using namespace std;

namespace fbs_tk {

bool write_buffer(ostream &out, const vector<uint8_t> &buff) {
	uint32_t size = buff.size();
	char size_buff[sizeof(uint32_t)];
	WriteScalar(size_buff, size);
	out.write(size_buff, sizeof(uint32_t));
	out.write(reinterpret_cast<const char*>(buff.data()), size);
	return !out.bad();
}

bool read_buffer(istream &in, vector<uint8_t> &buff) {
	if (in.peek() == EOF) {
		return false;
	}
	char size_buff[sizeof(uint32_t)];
	in.read(size_buff, sizeof(uint32_t));
	if (in.bad()) {
		return false;
	}
	auto size = ReadScalar<uint32_t>(size_buff);
	buff.resize(size);
	auto data = buff.data();
	in.read(reinterpret_cast<char *>(data), size);
  	return !in.bad();
}

}

