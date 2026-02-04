#define CPPHTTPLIB_NO_MMAP
#define CPPHTTPLIB_USE_SELECT

#include "httplib.h"
#include "database.h"
#include "inventory.h"
#include "api.h"
#include <iostream>

int main() {
    // Initialize database
    Database db("db/inventory.db");
    InventoryService inventory(db);

    // HTTP server
    httplib::Server server;

    // Serve frontend (GUI)
    // IMPORTANT: path is relative to runtime working directory
    server.set_mount_point("/", "./public");

    // Register REST APIs
    registerInventoryRoutes(server, inventory);

    std::cout << "🚀 Inventory App running at http://localhost:8080\n";

    // Start server
    server.listen("0.0.0.0", 8080);

    return 0;
}
