#ifndef CONFIGURATION_HPP_
#define CONFIGURATION_HPP_

#include "cassandra/cassandra.hpp"
#include <string>
#include <HalonMTA.h>
#include <memory>
#include <map>

struct ParsedConfig
{
	std::string default_profile;
	std::map<std::string, std::shared_ptr<CassandraProfile>> profiles;
};

bool parse_config(HalonConfig* cfg, ParsedConfig* parsed_cfg);

#endif
