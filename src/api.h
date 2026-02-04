#ifndef API_H
#define API_H

#include "httplib.h"
#include "inventory.h"

void registerInventoryRoutes(httplib::Server& server,
                             InventoryService& inventory);

#endif
