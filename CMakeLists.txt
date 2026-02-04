cmake_minimum_required(VERSION 3.10)

project(InventoryManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(inventory_app
    src/main.cpp
    src/api.cpp
    src/inventory.cpp
    src/database.cpp
    src/sqlite3.c
)

target_include_directories(inventory_app PRIVATE src)

# Platform-specific linking
if (WIN32)
    target_link_libraries(inventory_app ws2_32)
else()
    target_link_libraries(inventory_app pthread dl)
endif()
