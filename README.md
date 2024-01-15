# Cassandra client plugin

This plugin is a wrapper around [cpp-driver](https://github.com/datastax/cpp-driver).

## Installation

Follow the [instructions](https://docs.halon.io/manual/comp_install.html#installation) in our manual to add our package repository and then run the below command.

### Ubuntu

```
apt-get install halon-extras-cassandra
```

### RHEL

```
yum install halon-extras-cassandra
```

## Configuration

For the configuration schema, see [cassandra-app.schema.json](cassandra-app.schema.json).

### smtpd-app.yaml

```
plugins:
  - id: cassandra
    config:
      default_profile: cluster-1
      profiles:
        - id: cluster-1
          hosts:
            - cassandra-1
            - cassandra-2
          port: 9042
          connect_timeout: 5000
          request_timeout: 12000
          resolve_timeout: 2000
          connection_idle_timeout: 60
          connection_heartbeat_interval: 30
          tcp_keepalive: 60
          core_connections_per_host: 32
          num_threads_io: 1
          # username: username
          # password: password
          tls: false
          tls_verify_peer: false
          tls_verify_host: false
```

## Exported classes

These classes needs to be [imported](https://docs.halon.io/hsl/structures.html#import) from the `extras://cassandra` module path.

### Cassandra([string $profile])

The Cassandra class is a [cpp-driver](https://github.com/datastax/cpp-driver) wrapper class. On error an exception is thrown.

#### statement(string $query [, number $parameter_count]): CassandraStatement

Creates a new query statement. On error an exception is thrown.

#### batch(string $type): CassandraBatch

Creates a new batch statement with batch type `"LOGGED"`, `"UNLOGGED"` or `"COUNTER"`. On error an exception is thrown.

### CassandraStatement()

A query statement. This class cannot be instantiated and can only be returned from other functions.

#### paging_size(number $paging_size): this

Set the paging size. On error an exception is thrown.

#### has_more_pages(): boolean

Check if there are more pages. On error an exception is thrown.

#### execute(): array

Execute a query statement. On error an exception is thrown. See the table below for the data types mapping.

| Cassandra   | HSL       | Comment                                              |
|-------------|-----------|------------------------------------------------------|
| `ascii`     | `string`  |                                                      |
| `text`      | `string`  |                                                      |
| `varchar`   | `string`  |                                                      |
| `bigint`    | `string`  |                                                      |
| `counter`   | `string`  |                                                      |
| `int`       | `number`  |                                                      |
| `smallint`  | `number`  |                                                      |
| `tinyint`   | `number`  |                                                      |
| `float`     | `number`  |                                                      |
| `double`    | `number`  |                                                      |
| `inet`      | `string`  |                                                      |
| `uuid`      | `string`  |                                                      |
| `timeuuid`  | `string`  |                                                      |
| `boolean`   | `boolean` |                                                      |
| `timestamp` | `string`  |                                                      |
| `date`      | `number`  |                                                      |
| `time`      | `string`  |                                                      |
| `duration`  | `array`   | months (`number`), days (`number`), nanos (`string`) |
| `blob`      | `string`  |                                                      |
| `varint`    | `string`  |                                                      |
| `decimal`   | `array`   | varint (`string`), scale (`number`)                  |
| `map`       | `array`   |                                                      |
| `list`      | `array`   |                                                      |
| `set`       | `array`   |                                                      |
| `tuple`     | `array`   |                                                      |
| `custom`    | `none`    | **Not supported**                                    |
| `udt`       | `none`    | **Not supported**                                    |

#### bind_bool(number $index, boolean $value): this

Binds a `boolean` to a query at the specified index. On error an exception is thrown.

#### bind_bytes(number $index, string $value): this

Binds a `blob`, `varint` or `custom` to a query at the specified index. On error an exception is thrown.

#### bind_double(number $index, number $value): this

Binds a `double` to a query at the specified index. On error an exception is thrown.

#### bind_float(number $index, number $value): this

Binds a `float` to a query at the specified index. On error an exception is thrown.

#### bind_int8(number $index, number $value): this

Binds a `int8` to a query at the specified index. On error an exception is thrown.

#### bind_int16(number $index, number $value): this

Binds a `int16` to a query at the specified index. On error an exception is thrown.

#### bind_int32(number $index, number $value): this

Binds a `int32` to a query at the specified index. On error an exception is thrown.

#### bind_int64(number $index, string $value): this

Binds a `int64` to a query at the specified index. On error an exception is thrown.

#### bind_uint32(number $index, number $value): this

Binds a `uint32` to a query at the specified index. On error an exception is thrown.

#### bind_decimal(number $index, string $varint, number $scale): this

Binds a `decimal` to a query at the specified index. On error an exception is thrown.

#### bind_inet(number $index, string $value): this

Binds a `inet` to a query at the specified index. On error an exception is thrown.

#### bind_null(number $index): this

Binds a `null` to a query at the specified index. On error an exception is thrown.

#### bind_string(number $index, string $value): this

Binds a `string` to a query at the specified index. On error an exception is thrown.

#### bind_uuid(number $index, string $value): this

Binds a `uuid` to a query at the specified index. On error an exception is thrown.

#### bind_duration(number $index, number $months, number $days, string $nanos): this

Binds a `duration` to a query at the specified index. On error an exception is thrown.

### CassandraBatch()

A batch statement. This class cannot be instantiated and can only be returned from other functions.

#### statement(string $query [, number $parameter_count]): CassandraStatement

Creates a new query statement. On error an exception is thrown.

#### execute(): none

Execute a batch statement. On error an exception is thrown.

## Examples

```
import { Cassandra } from "extras://cassandra";

// Create "Cassandra" instance with an (optional) config profile
$cassandra = Cassandra("cluster-1");

// "CREATE" query statement
$cassandra->statement("CREATE KEYSPACE IF NOT EXISTS foo WITH REPLICATION = {'class':'NetworkTopologyStrategy','datacenter1':2};")->execute();
$cassandra->statement("CREATE TABLE IF NOT EXISTS foo.bar (id UUID PRIMARY KEY);")->execute();

// "INSERT" query statement with parameter binding
$statement = $cassandra->statement("INSERT INTO foo.bar (id) VALUES (?);", 1);
$statement->bind_uuid(0, uuid());
$statement->execute();

// "SELECT" query statement
echo $cassandra->statement("SELECT * FROM foo.bar;")->execute();

// "INSERT" batch statement
$batch = $cassandra->batch("LOGGED");
$statement = $batch->statement("INSERT INTO foo.bar (id) VALUES (?);", 1);
$statement->bind_uuid(0, uuid());
$statement = $batch->statement("INSERT INTO foo.bar (id) VALUES (?);", 1);
$statement->bind_uuid(0, uuid());
$statement = $batch->statement("INSERT INTO foo.bar (id) VALUES (?);", 1);
$statement->bind_uuid(0, uuid());
$batch->execute();

// "SELECT" query statement with pagination
$rows = [];
$statement = $cassandra->statement("SELECT * FROM foo.bar");
$statement->paging_size(2);
while ($statement->has_more_pages())
    $rows += $statement->execute();
echo $rows;
```
