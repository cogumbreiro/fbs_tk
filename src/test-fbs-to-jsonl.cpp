#include "fbs-to-jsonl/fbs-to-jsonl.hpp"
#include <flatbuffers/idl.h>
#include <flatbuffers/util.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace flatbuffers;
using namespace fbs2jsonl;

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
	string bin1;
	REQUIRE(json_to_bin(parser, js1.c_str(), bin1));
	auto obtained_js1 = bin_to_json(parser, bin1.c_str());
	string bin2;
	REQUIRE(json_to_bin(parser, obtained_js1.c_str(), bin2));
	REQUIRE(bin1 == bin2);
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
	string bin1;
	json_to_bin(parser, "{\"a\":10,\"b\":0}", bin1);
	string bin2;
	json_to_bin(parser, "{\"a\":20,\"b\":1}", bin2);
	ostringstream out1;
	store_string(out1, bin1);
	store_string(out1, bin2);
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
	REQUIRE(out3.str() == out1.str());
}

