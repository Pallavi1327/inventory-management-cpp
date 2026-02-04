#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>
#include "database.h"

// ---------- Data Model ----------
struct Item {
    int id;
    std::string name;
    int quantity;
    double price;
    int threshold;
};

// ---------- Service Layer ----------
class InventoryService {
public:
    explicit InventoryService(Database& db);

    // CLI / Core operations
    void addItem(const std::string& name, int qty, double price, int threshold);
    void updateQuantity(int id, int qty);
    void removeItem(int id);
    void showAllItems();
    void showLowStockItems();

    // REST API support
    std::vector<Item> getAllItems();

private:
    Database& database;
};

#endif
