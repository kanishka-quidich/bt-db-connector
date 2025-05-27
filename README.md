# bt-db-connector

A connector for the Ball_tracking ingest pipeline DB.

## Database Schema

The connector expects the following tables in the `quidich` keyspace:

### Table: camera

| Column  | Type    | Description         |
|---------|---------|---------------------|
| Camera  | text    | Primary key         |
| SSN     | int     | Serial number       |
| IP      | text    | Camera IP address   |
| System  | text    | System identifier   |

### Table: system

| Column  | Type    | Description         |
|---------|---------|---------------------|
| System  | text    | Primary key         |
| IP      | text    | System IP address   |

### Table: ball

| Column   | Type    | Description         |
|----------|---------|---------------------|
| BallID   | text    | Primary key         |
| Timestamp| text    | Timestamp           |
| Disk     | text    | Disk identifier     |
| Camera   | text    | Camera identifier   |

## Usage

Include the header and link against the Cassandra C/C++ driver.

```cpp
#include "DBApi.hpp"
#include <cassandra.h>

using namespace quidich;

int main() {
    // Initialize Cassandra cluster/session (not shown)
    CassSession* session = /* ... */;

    // Camera table usage
    CameraTable cameraTable(session);
    cameraTable.insert("CAM_01", 123, "192.168.1.10", "SYS_A");
    std::string cameraInfo = cameraTable.get("CAM_01");

    // System table usage
    SystemTable systemTable(session);
    systemTable.insert("SYS_A", "192.168.1.1");
    std::string systemInfo = systemTable.get("SYS_A");

    // Ball table usage
    BallTable ballTable(session);
    ballTable.insert("BALL_001", "2024-06-01T12:00:00Z", "DISK_1", "CAM_01");
    std::string ballInfo = ballTable.get("BALL_001");

    // ...cleanup session...
}
```

- All table classes require a valid `CassSession*` on construction.
- Each table class provides `insert` and `get` methods for basic operations.

> **Note:** Proper error handling and Cassandra session management are required in production code.
