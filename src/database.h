#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include "sqlite3.h"

class Database {
private:
    sqlite3* db;

public:
    explicit Database(const std::string& dbPath);
    ~Database();

    bool execute(const std::string& sql);
    std::vector<std::vector<std::string>> query(const std::string& sql);

    // MUST MATCH database.cpp EXACTLY
    bool executePrepared(const std::string& sql,
                         const std::vector<std::string>& params);
};

#endif
