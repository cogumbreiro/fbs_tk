#include "io-util.hpp"

#include <flatbuffers/idl.h>
#include <flatbuffers/util.h>

namespace fbs2jsonl {

// converts a json object into a binary FBS
bool json_to_bin(flatbuffers::Parser &parser, const char *js, std::string &bin);
// converts a binary FBS into a json object
std::string bin_to_json(flatbuffers::Parser &parser, const void *bin);
// converts a stream of binary FBS into a stream of JSON objects
bool fbs_stream_to_jsonl(const std::string &schema, std::istream &in, std::ostream &out);
// converts a stream of JSON objects into a stream of binary FBS
bool jsonl_to_fbs_stream(const std::string &schema, std::istream &in, std::ostream &out);

} // namespace
