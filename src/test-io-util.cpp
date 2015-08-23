#include "fbs-to-jsonl/io-util.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <flatbuffers/flatbuffers.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace flatbuffers;
using namespace fbs2jsonl;

TEST_CASE("load_buffer") {
	auto txt = "{\"access\": \"w\", \"function\":\"foo\", \"subscripts\": [{\"atoms\":[{\"constant\": 1, \"level\": 2, \"variable\": true}]}]}";
	istringstream in(txt);
	string buff;
	REQUIRE(load_buffer(in, &buff)); // test load buffer
	REQUIRE(buff == txt);
}

TEST_CASE("buffer_copy") {
	string data = "foo";
	string obtained;
	buffer_copy(data.c_str(), 2, obtained);
	REQUIRE(obtained == "fo");
}

TEST_CASE("write-str") {
	ostringstream out;
	string foo = "foo";
	REQUIRE(store_string(out, foo));
	auto data = out.str();
	REQUIRE(ReadScalar<uint32_t>(data.c_str()) == 3);
	REQUIRE(data[sizeof(uint32_t) + 0] == 'f');
	REQUIRE(data[sizeof(uint32_t) + 1] == 'o');
	REQUIRE(data[sizeof(uint32_t) + 2] == 'o');
	REQUIRE(data.size() == (foo.size() + sizeof(uint32_t)));
}

TEST_CASE("while-load-str") {
	ostringstream out;
	store_string(out, "foo");
	store_string(out, "bar");
	istringstream in(out.str());
	vector<string> obtained;
	string elem;
	while(load_string(in, elem)) {
		obtained.push_back(elem);
	}
	vector<string> expected = {"foo", "bar"};
	REQUIRE(obtained == expected);
}

TEST_CASE("read-str") {
	ostringstream out;
	string expected = "foo";
	store_string(out, expected);
	istringstream in(out.str());
	string obtained;
	REQUIRE(load_string(in, obtained));
	REQUIRE(expected == obtained);
}


