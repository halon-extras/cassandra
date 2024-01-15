#include "configuration.hpp"
#include <syslog.h>
#include <string.h>
#include <vector>
#include <memory>
#include <map>
#include <cassandra.h>

bool parse_config(HalonConfig* cfg, ParsedConfig* parsed_cfg)
{
	cass_log_set_level(CASS_LOG_DISABLED);

	const char* default_profile = HalonMTA_config_string_get(HalonMTA_config_object_get(cfg, "default_profile"), nullptr);
	parsed_cfg->default_profile = default_profile ? default_profile : "__default";

	auto profiles = HalonMTA_config_object_get(cfg, "profiles");
	if (profiles)
	{
		size_t x = 0;
		HalonConfig* profile;
		while ((profile = HalonMTA_config_array_get(profiles, x++)))
		{
			CassError error;

			auto cassandra_profile = std::make_shared<CassandraProfile>();

			CassCluster* cluster = cass_cluster_new();
			cassandra_profile->cluster = cluster;

			const char* id = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "id"), nullptr);
			if (!id)
			{
				syslog(LOG_CRIT, "cassandra: Missing id for profile");
				return false;
			}

			std::string contact_points;
			auto hosts = HalonMTA_config_object_get(profile, "hosts");
			if (hosts)
			{
				size_t y = 0;
				HalonConfig* _host;
				while ((_host = HalonMTA_config_array_get(hosts, y++)))
				{
					const char* host = HalonMTA_config_string_get(_host, nullptr);
					if (!host)
					{
						syslog(LOG_CRIT, "cassandra: Invalid host");
						return false;
					}
					contact_points += host;
					contact_points += ",";
				}
				if (!contact_points.empty())
					contact_points.pop_back();
			}
			if (contact_points.empty())
				contact_points = "127.0.0.1";

			error = cass_cluster_set_contact_points(cluster, contact_points.c_str());
			if (error != CASS_OK)
			{
				syslog(LOG_CRIT, "cassandra: %s", cass_error_desc(error));
				return false;
			}

			const char* port = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "port"), nullptr);
			if (port)
			{
				error = cass_cluster_set_port(cluster, (int)strtoul(port, nullptr, 10));
				if (error != CASS_OK)
				{
					syslog(LOG_CRIT, "cassandra: %s", cass_error_desc(error));
					return false;
				}
			}

			const char* connect_timeout = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "connect_timeout"), nullptr);
			if (connect_timeout)
				cass_cluster_set_connect_timeout(cluster, (unsigned int)strtoul(connect_timeout, nullptr, 10));

			const char* request_timeout = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "request_timeout"), nullptr);
			if (request_timeout)
				cass_cluster_set_request_timeout(cluster, (unsigned int)strtoul(request_timeout, nullptr, 10));

			const char* resolve_timeout = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "resolve_timeout"), nullptr);
			if (resolve_timeout)
				cass_cluster_set_resolve_timeout(cluster, (unsigned int)strtoul(resolve_timeout, nullptr, 10));

			const char* connection_idle_timeout = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "connection_idle_timeout"), nullptr);
			if (connection_idle_timeout)
				cass_cluster_set_connection_idle_timeout(cluster, (unsigned int)strtoul(connection_idle_timeout, nullptr, 10));

			const char* connection_heartbeat_interval = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "connection_heartbeat_interval"), nullptr);
			if (connection_heartbeat_interval)
				cass_cluster_set_connection_heartbeat_interval(cluster, (unsigned int)strtoul(connection_heartbeat_interval, nullptr, 10));

			const char* tcp_keepalive = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "tcp_keepalive"), nullptr);
			if (tcp_keepalive)
				cass_cluster_set_tcp_keepalive(cluster, cass_bool_t::cass_true, (unsigned int)strtoul(tcp_keepalive, nullptr, 10));

			const char* core_connections_per_host = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "core_connections_per_host"), nullptr);
			if (core_connections_per_host)
			{
				error = cass_cluster_set_core_connections_per_host(cluster, (unsigned int)strtoul(core_connections_per_host, nullptr, 10));
				if (error != CASS_OK)
				{
					syslog(LOG_CRIT, "cassandra: %s", cass_error_desc(error));
					return false;
				}
			}

			const char* num_threads_io = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "num_threads_io"), nullptr);
			if (num_threads_io)
			{
				error = cass_cluster_set_num_threads_io(cluster, (unsigned int)strtoul(num_threads_io, nullptr, 10));
				if (error != CASS_OK)
				{
					syslog(LOG_CRIT, "cassandra: %s", cass_error_desc(error));
					return false;
				}
			}

			bool tls = false;
			bool tls_verify_peer = false;
			bool tls_verify_host = false;
			const char* _tls = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "tls"), nullptr);
			if (_tls && strcasecmp(_tls, "true") == 0)
				tls = true;
			const char* _tls_verify_peer = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "tls_verify_peer"), nullptr);
			if (_tls_verify_peer && strcasecmp(_tls_verify_peer, "true") == 0)
				tls_verify_peer = true;
			const char* _tls_verify_host = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "tls_verify_host"), nullptr);
			if (_tls_verify_host && strcasecmp(_tls_verify_host, "true") == 0)
				tls_verify_host = true;

			if (tls)
			{
				CassSsl* ssl = cass_ssl_new();
				if (tls_verify_peer && tls_verify_host)
					cass_ssl_set_verify_flags(ssl, CASS_SSL_VERIFY_PEER_CERT | CASS_SSL_VERIFY_PEER_IDENTITY);
				else if (tls_verify_peer)
					cass_ssl_set_verify_flags(ssl, CASS_SSL_VERIFY_PEER_CERT);
				else if (tls_verify_host)
					cass_ssl_set_verify_flags(ssl, CASS_SSL_VERIFY_PEER_IDENTITY);
				else
					cass_ssl_set_verify_flags(ssl, CASS_SSL_VERIFY_NONE);
				cass_cluster_set_ssl(cluster, ssl);
				cass_ssl_free(ssl);
			}

			const char* username = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "username"), nullptr);
			const char* password = HalonMTA_config_string_get(HalonMTA_config_object_get(profile, "password"), nullptr);
			if (username && password)
				cass_cluster_set_credentials(cluster, username, password);

			parsed_cfg->profiles[id] = cassandra_profile;
		}
	}
	if (parsed_cfg->profiles.empty())
	{
		auto cassandra_profile = std::make_shared<CassandraProfile>();

		CassCluster* cluster = cass_cluster_new();
		cassandra_profile->cluster = cluster;

		CassError error = cass_cluster_set_contact_points(cluster, "127.0.0.1");
		if (error != CASS_OK)
		{
			syslog(LOG_CRIT, "cassandra: %s", cass_error_desc(error));
			return false;
		}

		parsed_cfg->profiles[parsed_cfg->default_profile] = cassandra_profile;
	}

	return true;
}
