#include <iostream>
#include "src/Database/Database.h"

int main()
{
    Database db("mydb.cl");

    // Create a table
    std::vector<std::string> columns = {"id", "name", "age"};
    db.createTable("users", columns);

    // Insert some records
    db.insertRecord("users", {"1", "John", "25"});
    db.insertRecord("users", {"2", "Alice", "30"});
    db.insertRecord("users", {"3", "Bob", "35"});

    // Select and display all records
    std::cout << "\nAll records in users table:\n";
    db.selectAll("users");

    return 0;
}
