#ifndef CASSANDRA_HPP_
#define CASSANDRA_HPP_

#include <HalonMTA.h>
#include <cassandra.h>
#include <mutex>
#include <memory>

struct CassandraProfile
{
	CassCluster* cluster;
	CassSession* session;
	std::mutex mutex;
	~CassandraProfile();
};

class Cassandra
{
  public:
	std::shared_ptr<CassandraProfile> profile;
	CassStatement* statement;
	CassBatch* batch;
	bool has_more_pages = true;
};

void Cassandra_constructor(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_free(void* ptr);

#endif
