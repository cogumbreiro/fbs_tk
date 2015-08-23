#include "fbs-to-jsonl/fbs-to-jsonl.hpp"
#include "fbs-to-jsonl/io-util.hpp"

#include <cstring>

using namespace std;
using namespace flatbuffers;

namespace fbs2jsonl {

bool json_to_bin(Parser &parser, const char *js, string &bin) {
	if (!parser.Parse(js)) {
		return false;
	}
	buffer_copy(parser.builder_.GetBufferPointer(),  parser.builder_.GetSize(), bin);
	return true;
}

struct BinToJson {
	GeneratorOptions opts;
	BinToJson() {
		opts.strict_json = true;
		opts.indent_step = -1;
	}
};

string bin_to_json(Parser &parser, const void *bin) {
	static BinToJson init;
	string buffer;
	GenerateText(parser, bin, init.opts, &buffer);
	return buffer;
}

bool fbs_stream_to_jsonl(const string &schema, istream &in, ostream &out) {
	Parser parser;
	parser.Parse(schema.c_str());
	string data;
	while(load_string(in, data)) {
		out << bin_to_json(parser, data.c_str()) << endl;
	}
	return in.eof();
}

bool jsonl_to_fbs_stream(const string &schema, istream &in, ostream &out) {
	Parser parser;
	parser.Parse(schema.c_str());
	string js;
	string bin;
	while(getline(in, js)) {
		if (!json_to_bin(parser, js.c_str(), bin)) {
			return false;
		}
		if (!store_string(out, bin)) {
			return false;
		}
	}
	return in.eof();
}

} // NAMESPACE


