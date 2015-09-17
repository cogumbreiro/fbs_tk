#include "fbs_tk.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <flatbuffers/flatbuffers.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace flatbuffers;
using namespace fbs_tk;

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
	out << Buffer(foo);
	REQUIRE(out.good());
	auto data = out.str();
	REQUIRE(ReadScalar<uint32_t>(data.c_str()) == 3);
	REQUIRE(data[sizeof(uint32_t) + 0] == 'f');
	REQUIRE(data[sizeof(uint32_t) + 1] == 'o');
	REQUIRE(data[sizeof(uint32_t) + 2] == 'o');
	REQUIRE(data.size() == (foo.size() + sizeof(uint32_t)));
}


TEST_CASE("write-buffer") {
	ostringstream out;
	Buffer foo({1, 2, 3});
	out << foo;
	REQUIRE(out.good());
	auto str = out.str();
	auto data = str.c_str();
	REQUIRE(ReadScalar<uint32_t>(data) == 3);
	REQUIRE(data[sizeof(uint32_t) + 0] == 1);
	REQUIRE(data[sizeof(uint32_t) + 1] == 2);
	REQUIRE(data[sizeof(uint32_t) + 2] == 3);
	REQUIRE(str.size() == (foo.size() + sizeof(uint32_t)));
}

TEST_CASE("read-buffer") {
	ostringstream out;
	char size_buff[sizeof(uint32_t)];
	WriteScalar(size_buff, 3);
	out.write(size_buff, sizeof(uint32_t));
	uint8_t tmp[3] = {1, 2, 3};
	out.write(reinterpret_cast<const char*>(tmp), 3);

	Buffer obtained;
	istringstream in(out.str());
	in >> obtained;
	vector<uint8_t> expected = {1, 2, 3};
	REQUIRE(expected == obtained.get_data());
}

TEST_CASE("write-str-2") {
	ostringstream out;
	string foo = string("f\0o", 3);
	REQUIRE(foo.size() == 3);
	out << Buffer(foo);
	REQUIRE(out.good());
	auto data = out.str();
	REQUIRE(ReadScalar<uint32_t>(data.c_str()) == 3);
	REQUIRE(data[sizeof(uint32_t) + 0] == 'f');
	REQUIRE(data[sizeof(uint32_t) + 1] == '\0');
	REQUIRE(data[sizeof(uint32_t) + 2] == 'o');
	REQUIRE(data.size() == (foo.size() + sizeof(uint32_t)));
}

TEST_CASE("while-load-str") {
	ostringstream out;
	out << Buffer("foo");
	out << Buffer("bar");
	istringstream in(out.str());
	vector<string> obtained;
	Buffer elem;
	in >> elem;
	REQUIRE(in.good());
	obtained.push_back(elem.str());
	in >> elem;
	REQUIRE(in.good());
	obtained.push_back(elem.str());
	vector<string> expected = {"foo", "bar"};
	REQUIRE(obtained == expected);
}

TEST_CASE("irange") {
	ostringstream out;
	out << 1;
	out << " ";
	out << 2;
	out << " ";
	out << 3;
	istringstream in(out.str());
	vector<int> obtained;
	for (int elem : range<int>(in)) {
		obtained.push_back(elem);
	}
	vector<int> expected = {1, 2, 3};
	REQUIRE(obtained == expected);
}

TEST_CASE("buff-ser") {
	ostringstream out;
	vector<uint8_t> v1 = {1, 2};
	Buffer expected(v1);
	out << expected;
	istringstream in(out.str());
	Buffer obtained;
	in >> obtained;
	REQUIRE(in.good());
	REQUIRE(in.peek() == EOF);
}

TEST_CASE("range-buffer-empty") {
	istringstream in("");
	for (const auto & buffer : range<Buffer>(in)) {
		REQUIRE(false);
	}
	REQUIRE(in.eof());
}

TEST_CASE("range-buffer-garbage") {
	istringstream in("lololol");
	for (const auto & buffer : range<Buffer>(in)) {
		REQUIRE(false);
	}
	REQUIRE(in.fail());
}

TEST_CASE("read-str") {
	ostringstream out;
	Buffer expected("foo");
	out << expected;
	istringstream in(out.str());
	Buffer obtained;
	in >> obtained;
	REQUIRE(in.good());
	REQUIRE(expected == obtained);
}


