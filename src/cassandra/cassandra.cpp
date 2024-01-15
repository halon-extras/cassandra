#include "cassandra.hpp"
#include "statement/statement.hpp"
#include "batch/batch.hpp"
#include "configuration.hpp"
#include "argument.hpp"
#include <mutex>
#include <map>
#include <memory>

extern ParsedConfig parsed_cfg;
extern std::mutex mutex_cfg;

void Cassandra_constructor(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	std::string profile;
	if (!parse_hsl_argument_as_string(hhc, args, 0, false, profile))
		return;

	mutex_cfg.lock();

	if (profile.empty())
		profile = parsed_cfg.default_profile;

	std::shared_ptr<CassandraProfile> cassandra_profile;

	auto profile_iterator = parsed_cfg.profiles.find(profile);
	if (profile_iterator == parsed_cfg.profiles.end())
	{
		mutex_cfg.unlock();
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, "invalid profile", 0);
		return;
	}

	cassandra_profile = profile_iterator->second;
	mutex_cfg.unlock();

	std::unique_lock<std::mutex> lg(cassandra_profile->mutex);
	if (!cassandra_profile->session)
	{
		CassCluster* cluster = cassandra_profile->cluster;
		CassSession* session = cass_session_new();
		CassFuture* connect_future = cass_session_connect(session, cluster);
		if (cass_future_error_code(connect_future) == CASS_OK)
		{
			cassandra_profile->session = session;
			cass_future_free(connect_future);
		}
		else
		{
			const char* message;
			size_t message_length;
			cass_future_error_message(connect_future, &message, &message_length);
			HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
			HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, message, message_length);
			cass_future_free(connect_future);
			cass_session_free(session);
			return;
		}
	}

	Cassandra* ptr = new Cassandra();
	ptr->profile = cassandra_profile;

	HalonHSLObject* object = HalonMTA_hsl_object_new();
	HalonMTA_hsl_object_type_set(object, "Cassandra");
	HalonMTA_hsl_object_register_function(object, "statement", Cassandra_statement);
	HalonMTA_hsl_object_register_function(object, "batch", Cassandra_batch);
	HalonMTA_hsl_object_ptr_set(object, ptr, Cassandra_free);
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_OBJECT, object, 0);
	HalonMTA_hsl_object_delete(object);
}

void Cassandra_free(void* ptr)
{
	Cassandra* x = (Cassandra*)ptr;
	delete x;
}

CassandraProfile::~CassandraProfile()
{
	if (session)
	{
		CassFuture* close_future = cass_session_close(session);
		cass_future_wait(close_future);
		cass_future_free(close_future);
		cass_session_free(session);
	}
	cass_cluster_free(cluster);
}
