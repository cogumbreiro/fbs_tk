#include "fbs_tk.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <flatbuffers/flatbuffers.h>

using namespace flatbuffers;
using namespace std;

namespace fbs_tk {

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

