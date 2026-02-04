#include "httplib.h"
#include "inventory.h"
#include <sstream>

// ---------------- REGISTER ROUTES ----------------
void registerInventoryRoutes(httplib::Server& server,
                             InventoryService& inventory) {

    // ---------------- HEALTH CHECK ----------------
    server.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Inventory API is running", "text/plain");
    });

    // ---------------- GET /items ----------------
    server.Get("/items", [&](const httplib::Request&, httplib::Response& res) {
        auto items = inventory.getAllItems();

        std::ostringstream json;
        json << "[";

        for (size_t i = 0; i < items.size(); ++i) {
            json << "{"
                 << "\"id\":" << items[i].id << ","
                 << "\"name\":\"" << items[i].name << "\","
                 << "\"quantity\":" << items[i].quantity << ","
                 << "\"price\":" << items[i].price << ","
                 << "\"threshold\":" << items[i].threshold
                 << "}";

            if (i != items.size() - 1)
                json << ",";
        }

        json << "]";
        res.set_content(json.str(), "application/json");
    });

    // ---------------- POST /items ----------------
    // Body (x-www-form-urlencoded):
    // name=Pen&quantity=10&price=5.5&threshold=3
    server.Post("/items", [&](const httplib::Request& req,
                              httplib::Response& res) {
        if (!req.has_param("name") ||
            !req.has_param("quantity") ||
            !req.has_param("price") ||
            !req.has_param("threshold")) {

            res.status = 400;
            res.set_content("Missing parameters", "text/plain");
            return;
        }

        std::string name = req.get_param_value("name");
        int quantity = std::stoi(req.get_param_value("quantity"));
        double price = std::stod(req.get_param_value("price"));
        int threshold = std::stoi(req.get_param_value("threshold"));

        inventory.addItem(name, quantity, price, threshold);

        res.set_content("Item added successfully", "text/plain");
    });

    // ---------------- PUT /items/{id} ----------------
    // Body:
    // quantity=25
    server.Put(R"(/items/(\d+))", [&](const httplib::Request& req,
                                      httplib::Response& res) {
        int id = std::stoi(req.matches[1]);

        if (!req.has_param("quantity")) {
            res.status = 400;
            res.set_content("Missing quantity parameter", "text/plain");
            return;
        }

        int quantity = std::stoi(req.get_param_value("quantity"));
        inventory.updateQuantity(id, quantity);

        res.set_content("Quantity updated", "text/plain");
    });

    // ---------------- DELETE /items/{id} ----------------
    server.Delete(R"(/items/(\d+))", [&](const httplib::Request& req,
                                         httplib::Response& res) {
        int id = std::stoi(req.matches[1]);
        inventory.removeItem(id);

        res.set_content("Item removed", "text/plain");
    });
}
