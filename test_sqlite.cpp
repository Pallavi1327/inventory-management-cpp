#include <iostream>
#include "sqlite3.h"

int main() {
    sqlite3* db = nullptr;

    int rc = sqlite3_open("test.db", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "❌ Can't open database: "
                  << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    std::cout << "✅ SQLite opened successfully!" << std::endl;

    sqlite3_close(db);
    return 0;
}
