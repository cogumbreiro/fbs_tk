#include "fbs_tk.hpp"

#include <cstring>

using namespace std;
using namespace flatbuffers;

namespace fbs_tk {

bool json_to_bin(Parser &parser, const char *js, Buffer &bin) {
	if (!parser.Parse(js)) {
		return false;
	}
	copy_from(bin, parser.builder_);
	return true;
}

bool json_to_fbs(flatbuffers::Parser &parser, std::istream &in, std::ostream &out) {
	string js;
	if (!load_buffer(in, &js)) {
		return false;
	}
	Buffer bin;
	if (!json_to_bin(parser, js.c_str(), bin)) {
		return false;
	}
	bin.write_data(out);
	return true;
}


struct BinToJson {
	GeneratorOptions opts;
	BinToJson() {
		opts.strict_json = true;
		opts.indent_step = -1;
	}
};

string bin_to_json(Parser &parser, const Buffer &bin) {
	static BinToJson init;
	string buffer;
	GenerateText(parser, bin.get_data().data(), init.opts, &buffer);
	return buffer;
}

// converts a binary FBS into a json object
bool fbs_to_json(flatbuffers::Parser &parser, std::istream &in, std::ostream &out) {
	Buffer bin;
	bin.read_all_data(in);
	if (in.bad()) {
		return false;
	}
	out << bin_to_json(parser, bin);
	return out.good();
}

bool fbs_stream_to_jsonl(const string &schema, istream &in, ostream &out) {
	Parser parser;
	parser.Parse(schema.c_str());
	for (const auto & buff : range<Buffer>(in)) {
		out << bin_to_json(parser, buff) << endl;
	}
	return in.eof();
}

bool jsonl_to_fbs_stream(const string &schema, istream &in, ostream &out) {
	Parser parser;
	parser.Parse(schema.c_str());
	string js;
	Buffer bin;
	while(getline(in, js)) {
		if (!json_to_bin(parser, js.c_str(), bin)) {
			return false;
		}
		out << bin;
		if (out.bad()) {
			return false;
		}
	}
	return in.eof();
}

} // NAMESPACE


