#include <iostream>
#include "src/Database/Database.h"

int main()
{
    Database db;
    std::vector<std::string> columns = {"id", "name", "age"};
    db.createTable("users", columns);
    auto users = db.getTable("users");
    if (users) {
        users->insert({"1", "John", "25"});
        users->insert({"2", "Alice", "30"});

        // Display the table contents
        std::cout << "\nContents of users table:\n";
        users->display();
    }

    return 0;
}
