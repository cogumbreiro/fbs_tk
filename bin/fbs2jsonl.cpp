#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>
#include <flatbuffers/util.h>
#include <fbs_tk.hpp>

int main(int argc, const char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <SCHEMA>" << std::endl;
		std::cerr << "\tConverts a stream of binary Flatbuffers into JSONL." << std::endl;
		std::cerr << "\tSCHEMA\tThe flatbuffers schema used to parse the data." << std::endl;
		return -1;
	}
	flatbuffers::Parser parser;
	std::string schemafile;
  	if (!flatbuffers::LoadFile(argv[1], false, &schemafile)) {
  		std::cerr << "Could not open: " << argv[1] <<  std::endl;
  		return -2;
  	}
  	if (!fbs_tk::fbs_stream_to_jsonl(schemafile.c_str(), std::cin, std::cout)) {
		std::cerr << "Error parsing data!" << std::endl;
		return -3;
  	}
	return 0;
}

