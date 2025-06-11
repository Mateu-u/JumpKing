#include "databasemanager.h"
#include "sqlite3.h"
#include <iostream>

static sqlite3* g_db = nullptr;

bool DatabaseManager::init(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &g_db) != SQLITE_OK) {
        std::cerr << "Nie udało się otworzyć DB: " << sqlite3_errmsg(g_db) << "\n";
        return false;
    }
    // Możesz tu automatycznie utworzyć tabelę, jeśli nie istnieje:
    const char* ddl =
        "CREATE TABLE IF NOT EXISTS users ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " username TEXT UNIQUE,"
        " password TEXT,"
        " levels_completed INTEGER DEFAULT 0);";
    char* err = nullptr;
    sqlite3_exec(g_db, ddl, nullptr, nullptr, &err);
    if (err) { sqlite3_free(err); }
    return true;
}

bool DatabaseManager::authenticate(const std::string& user, const std::string& pass) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT password FROM users WHERE username = ?;";
    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);

    bool ok = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string stored = reinterpret_cast<const char*>(sqlite3_column_text(stmt,0));
        ok = (stored == pass);
    }
    sqlite3_finalize(stmt);
    return ok;
}

bool DatabaseManager::createUser(const std::string& user, const std::string& pass) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "INSERT INTO users(username,password) VALUES(?,?);";
    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass.c_str(), -1, SQLITE_STATIC);
    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

std::vector<PlayerInfo> DatabaseManager::fetchAllPlayers() {
    std::vector<PlayerInfo> v;
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT id,username,levels_completed FROM users;";
    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            v.push_back({
                sqlite3_column_int(stmt,0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt,1)),
                sqlite3_column_int(stmt,2)
            });
        }
    }
    sqlite3_finalize(stmt);
    return v;
}

bool DatabaseManager::updateLevels(int playerId, int newCount) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "UPDATE users SET levels_completed = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_int(stmt, 1, newCount);
    sqlite3_bind_int(stmt, 2, playerId);
    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}
