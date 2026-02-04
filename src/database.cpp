#include "database.h"
#include <iostream>

Database::Database(const std::string& dbPath) : db(nullptr) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "❌ Failed to open database\n";
        db = nullptr;
    }
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

bool Database::execute(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "❌ SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

std::vector<std::vector<std::string>> Database::query(const std::string& sql) {
    std::vector<std::vector<std::string>> result;
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return result;
    }

    int cols = sqlite3_column_count(stmt);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;
        for (int i = 0; i < cols; i++) {
            const unsigned char* text = sqlite3_column_text(stmt, i);
            row.push_back(text ? reinterpret_cast<const char*>(text) : "");
        }
        result.push_back(row);
    }

    sqlite3_finalize(stmt);
    return result;
}

// 🔐 PREPARED STATEMENT SUPPORT (THIS WAS MISSING AT LINK TIME)
bool Database::executePrepared(const std::string& sql,
                               const std::vector<std::string>& params) {
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(
            stmt,
            static_cast<int>(i + 1),
            params[i].c_str(),
            -1,
            SQLITE_TRANSIENT
        );
    }

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}
