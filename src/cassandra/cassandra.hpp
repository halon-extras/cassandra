#ifndef CASSANDRA_HPP_
#define CASSANDRA_HPP_

#include <HalonMTA.h>
#include <cassandra.h>
#include <mutex>
#include <memory>

struct CassandraProfile
{
	CassCluster* cluster = nullptr;
	CassSession* session = nullptr;
	std::mutex mutex;
	~CassandraProfile();
};

class Cassandra
{
  public:
	std::shared_ptr<CassandraProfile> profile;
	CassStatement* statement = nullptr;
	CassBatch* batch = nullptr;
	bool has_more_pages = true;
};

void Cassandra_constructor(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_free(void* ptr);

void throw_cass_error(HalonHSLContext* hhc, CassError error);
void throw_cass_future_error(HalonHSLContext* hhc, CassFuture* future);

#endif
