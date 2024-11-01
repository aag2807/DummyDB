#include <iostream>
#include "src/Database/Database.h"

int main()
{
    Database db;

    // Create a table
    std::vector<std::string> columns = {"id", "name", "age"};
    db.createTable("users", columns);

    // Insert some records
    auto users = db.getTable("users");
    if (users)
    {
        users->insert({"1", "John", "25"});
        users->insert({"2", "Alice", "30"});

        // Display the table contents
        std::cout << "\nContents of users table:\n";
        users->display();

        // Save to file
        db.saveToFile("database.cl");
    }

    // Create a new database instance and load from file
    Database db2;
    db2.loadFromFile("database.cl");

    // Display loaded data
    std::cout << "\nContents after loading from file:\n";
    auto loadedUsers = db2.getTable("users");
    if (loadedUsers)
    {
        loadedUsers->display();
    }

    return 0;
}
