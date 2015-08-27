#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>
#include <flatbuffers/util.h>
#include <fbs_tk.hpp>

int main(int argc, const char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <SCHEMA>" << std::endl;
		std::cerr << "\tConverts a binary Flatbuffer into JSON." << std::endl;
		std::cerr << "\tSCHEMA\tThe flatbuffers schema used to parse the data." << std::endl;
		return -1;
	}
	std::string schemafile;
  	if (!flatbuffers::LoadFile(argv[1], false, &schemafile)) {
  		std::cerr << "Could not open: " << argv[1] <<  std::endl;
  		return -2;
  	}
	flatbuffers::Parser parser;
	if (!parser.Parse(schemafile.c_str())) {
		std::cerr << "Could not parse schema: " << parser.error_ << std::endl;
		return -3;
	}
	if (!fbs_tk::fbs_to_json(parser, std::cin, std::cout)) {
		std::cerr << "Error parsing binary FBS: " << parser.error_ << std::endl;
		return -4;
	}
	return 0;
}
