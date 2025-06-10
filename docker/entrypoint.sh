#!/bin/bash
set -e

# Start Cassandra in the background
echo "Starting Cassandra..."
/docker-entrypoint.sh cassandra -R &
CASSANDRA_PID=$!

# Wait for Cassandra to be ready
echo "Waiting for Cassandra to be ready on port 9042..."
for i in {1..60}; do
    if nc -z localhost 9042 && nodetool status >/dev/null 2>&1; then
        echo "Cassandra is up and running!"
        sleep 10  # Give extra time for full initialization
        break
    fi
    echo "Attempt $i: Cassandra not ready yet... ($(date))"
    sleep 5
done

# Load schema if available
if [ -f /data/init_schema.cql ]; then
    echo "Loading schema..."
    until cqlsh localhost -e "DESCRIBE KEYSPACES" >/dev/null 2>&1; do
        echo "Waiting for CQL to be fully ready..."
        sleep 5
    done
    cqlsh localhost -f /data/init_schema.cql || echo "Schema load failed"
fi

# Wait for Cassandra to exit
wait $CASSANDRA_PID
