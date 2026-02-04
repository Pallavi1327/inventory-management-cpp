#include "inventory.h"
#include <iostream>

// Constructor
InventoryService::InventoryService(Database& db)
    : database(db) {}


// ---------------- ADD ITEM ----------------
void InventoryService::addItem(const std::string& name,
                               int qty,
                               double price,
                               int threshold) {
    std::string sql =
        "INSERT INTO inventory "
        "(name, quantity, price, low_stock_threshold) "
        "VALUES (?, ?, ?, ?);";

    bool success = database.executePrepared(
        sql,
        {
            name,
            std::to_string(qty),
            std::to_string(price),
            std::to_string(threshold)
        }
    );

    if (success) {
        std::cout << "✅ Item added safely\n";
    } else {
        std::cout << "❌ Failed to add item\n";
    }
}


// ---------------- UPDATE QUANTITY ----------------
void InventoryService::updateQuantity(int id, int qty) {
    std::string sql =
        "UPDATE inventory SET quantity = ? WHERE id = ?;";

    bool success = database.executePrepared(
        sql,
        {
            std::to_string(qty),
            std::to_string(id)
        }
    );

    if (success) {
        std::cout << "✅ Quantity updated safely\n";
    } else {
        std::cout << "❌ Failed to update quantity\n";
    }
}


// ---------------- REMOVE ITEM ----------------
void InventoryService::removeItem(int id) {
    std::string sql =
        "DELETE FROM inventory WHERE id = ?;";

    bool success = database.executePrepared(
        sql,
        {
            std::to_string(id)
        }
    );

    if (success) {
        std::cout << "✅ Item removed safely\n";
    } else {
        std::cout << "❌ Failed to remove item\n";
    }
}


// ---------------- SHOW ALL ITEMS (CLI) ----------------
void InventoryService::showAllItems() {
    auto rows = database.query(
        "SELECT id, name, quantity, price, low_stock_threshold FROM inventory;"
    );

    std::cout << "\nID | Name | Qty | Price | Threshold\n";
    std::cout << "-----------------------------------\n";

    for (const auto& r : rows) {
        std::cout << r[0] << " | "
                  << r[1] << " | "
                  << r[2] << " | "
                  << r[3] << " | "
                  << r[4] << "\n";
    }
}


// ---------------- LOW STOCK ALERT (CLI) ----------------
void InventoryService::showLowStockItems() {
    auto rows = database.query(
        "SELECT name, quantity, low_stock_threshold "
        "FROM inventory WHERE quantity <= low_stock_threshold;"
    );

    if (rows.empty()) {
        std::cout << "✅ No low-stock items\n";
        return;
    }

    std::cout << "\n⚠ LOW STOCK ALERT ⚠\n";
    std::cout << "Name | Qty | Threshold\n";
    std::cout << "----------------------\n";

    for (const auto& r : rows) {
        std::cout << r[0] << " | "
                  << r[1] << " | "
                  << r[2] << "\n";
    }
}


// ---------------- REST API SUPPORT ----------------
std::vector<Item> InventoryService::getAllItems() {
    std::vector<Item> items;

    auto rows = database.query(
        "SELECT id, name, quantity, price, low_stock_threshold FROM inventory;"
    );

    for (const auto& r : rows) {
        Item item;
        item.id = std::stoi(r[0]);
        item.name = r[1];
        item.quantity = std::stoi(r[2]);
        item.price = std::stod(r[3]);
        item.threshold = std::stoi(r[4]);

        items.push_back(item);
    }

    return items;
}
