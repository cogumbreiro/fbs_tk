#ifndef _FBS_TK_HPP_
#define _FBS_TK_HPP_

#include <flatbuffers/idl.h>
#include <flatbuffers/util.h>
#include <iostream>
#include <cstring>

namespace fbs_tk {
// Loads all the contents of the input stream into a string
inline bool load_buffer(std::istream &in, std::string *buf) {
  *buf = std::string(std::istreambuf_iterator<char>(in),
                    std::istreambuf_iterator<char>());
  return !in.bad();
}
// Copies a buffer into a string
inline void buffer_copy(const void *data, size_t size, std::string &dst) {
	dst.resize(size);
	memcpy(&dst[0], data, size);
}
// Encodes a string into an output stream
bool store_string(std::ostream &out, const std::string &data);
// Reads a string from an input stream
bool load_string(std::istream &in, std::string &str);

// converts a json object into a binary FBS
bool json_to_bin(flatbuffers::Parser &parser, const char *js, std::string &bin);
// converts a binary FBS into a json object
std::string bin_to_json(flatbuffers::Parser &parser, const void *bin);
// converts a stream of binary FBS into a stream of JSON objects
bool fbs_stream_to_jsonl(const std::string &schema, std::istream &in, std::ostream &out);
// converts a stream of JSON objects into a stream of binary FBS
bool jsonl_to_fbs_stream(const std::string &schema, std::istream &in, std::ostream &out);

// GetRoot from a string
template<class T>
inline const T* GetRootFrom(const std::string &buff) {
	return flatbuffers::GetRoot<T>(buff.c_str());
}

// Create a root object and bundle it with its data
template <class T>
struct Root {
	const T* root;

	Root() : root(nullptr), data() {}
	Root(const std::string &buff) {
		data = buff;
		root = GetRootFrom<T>(data);
	}
	Root(const T& other, std::function<flatbuffers::Offset<T>(flatbuffers::FlatBufferBuilder &, const T&)> copier) : data() {
		flatbuffers::FlatBufferBuilder builder;
		auto subs = copier(builder, other);
		builder.Finish(subs);
		fbs_tk::buffer_copy(builder.GetBufferPointer(), builder.GetSize(), data);
		root = GetRootFrom<T>(data);
	}
private:
	std::string data;
};

} // namespace

#endif // _FBS_TK_HPP_
