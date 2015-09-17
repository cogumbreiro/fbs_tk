#include "fbs_tk.hpp"
#include <flatbuffers/idl.h>
#include <flatbuffers/util.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace flatbuffers;
using namespace fbs_tk;

TEST_CASE("bin-to-json") {
	string schema =
		"table Atom {\n"
		"  a: int;\n"
		"  b: bool;\n"
		"}\n"
		"root_type Atom;\n";
	string js1 = "{\"a\":10,\"b\":0}";
	flatbuffers::Parser parser;
	REQUIRE(parser.Parse(schema.c_str()));
	Buffer bin1;
	REQUIRE(json_to_bin(parser, js1.c_str(), bin1));
	auto obtained_js1 = bin_to_json(parser, bin1);
	Buffer bin2;
	REQUIRE(json_to_bin(parser, obtained_js1.c_str(), bin2));
	REQUIRE(bin1 == bin2);
}

TEST_CASE("fbs-to-json-io") {
	string schema =
		"table Atom {\n"
		"  a: int;\n"
		"  b: bool;\n"
		"}\n"
		"root_type Atom;\n";
	flatbuffers::Parser parser1;
	REQUIRE(parser1.Parse(schema.c_str()));

	Buffer bin1;
	string js1 = "{\"a\": 10,\"b\": true}";
	json_to_bin(parser1, js1.c_str(), bin1);
	
	istringstream in(bin1.str());
	ostringstream out;
	REQUIRE(fbs_to_json(parser1, in, out));
	REQUIRE(out.str() == js1);
}

TEST_CASE("json-to-fbs-io") {
	string schema =
		"table Atom {\n"
		"  a: int;\n"
		"  b: bool;\n"
		"}\n"
		"root_type Atom;\n";
	string js1 = "{\"a\": 10,\"b\": 99}";
	flatbuffers::Parser parser1;
	parser1.Parse(schema.c_str());
	Buffer bin1;
	json_to_bin(parser1, js1.c_str(), bin1);
	
	istringstream in(js1);
	ostringstream out;
	REQUIRE(json_to_fbs(parser1, in, out));
	REQUIRE(out.str() == bin1.str());
}

TEST_CASE("parse-no-bug") {
	string schema =
		"table Bar {\n"
		"  a: int;\n"
		"  b: bool;\n"
		"}\n"
		"root_type Bar;\n";
	flatbuffers::Parser parser;
	REQUIRE(parser.Parse(schema.c_str()));
	REQUIRE(parser.Parse("{\"a\":10,\"b\":0}"));
	REQUIRE(parser.Parse("{\"b\":0,\"a\":10}"));
}

TEST_CASE("parse-bug") {
	string schema =
		"struct Bar {\n"
		"  a: int;\n"
		"  b: bool;\n"
		"}\n"
		"table Foo {\n"
		"  c: Bar;\n"
		"}\n"
		"root_type Foo;\n";
	flatbuffers::Parser parser;
	REQUIRE(parser.Parse(schema.c_str()));
	REQUIRE(parser.Parse("{\"c\":{\"a\":10,\"b\":0}}"));
	REQUIRE(parser.Parse("{\"c\":{\"b\":0,\"a\":10}}"));
}

TEST_CASE("bin-to-jsonl-to-bin") {
	string schema =
		"table Atom {\n"
		"  a: int;\n"
		"  b: bool;\n"
		"}\n"
		"root_type Atom;\n";
	flatbuffers::Parser parser;
	parser.Parse(schema.c_str());
	Buffer bin1;
	json_to_bin(parser, "{\"a\":99,\"b\":true}", bin1);
	Buffer bin2;
	json_to_bin(parser, "{\"a\":33,\"b\":false}", bin2);
	ostringstream out1;
	out1 << bin1;
	out1 << bin2;
	istringstream in1(out1.str());
	// end of bootstrap
	
	ostringstream out2;
	REQUIRE(fbs_stream_to_jsonl(schema, in1, out2));
	REQUIRE(in1.eof());
	
	istringstream in2(out2.str());
	ostringstream out3;
	REQUIRE(jsonl_to_fbs_stream(schema, in2, out3));
	REQUIRE(in2.eof());
	
	REQUIRE(out3.str().size() > 0);
	REQUIRE(Buffer(out3.str()).get_data() == Buffer(out1.str()).get_data());
}

