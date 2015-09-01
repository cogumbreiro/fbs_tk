#include "fbs_tk.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <flatbuffers/flatbuffers.h>

using namespace flatbuffers;
using namespace std;

namespace fbs_tk {

bool write_buffer(ostream &out, const vector<int8_t> &buff) {
	uint32_t size = buff.size();
	char size_buff[sizeof(uint32_t)];
	WriteScalar(size_buff, size);
	out.write(size_buff, sizeof(uint32_t));
	out.write(reinterpret_cast<const char*>(buff.data()), size);
	return !out.bad();
}

bool read_buffer(istream &in, vector<int8_t> &buff) {
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
	int8_t *data = buff.data();
	in.read(reinterpret_cast<char *>(data), size);
  	return !in.bad();
}

bool store_string(ostream &out, const string &data) {
	uint32_t size = data.size();
	char buff[sizeof(uint32_t)];
	WriteScalar(buff, size);
	out.write(buff, sizeof(buff));
	out.write(data.c_str(), size);
	return !out.bad();
}

bool load_string(istream &in, std::string &str) {
	if (in.peek() == EOF) {
		return false;
	}
	char buff[sizeof(uint32_t)];
	in.read(buff, sizeof(uint32_t));
	if (in.bad()) {
		return false;
	}
	auto size = ReadScalar<uint32_t>(buff);
	str.resize(size);
	in.read(&str[0], size);
  	return !in.bad();
}

}

