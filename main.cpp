// main.cpp
#include <iostream>
#include <vector>
#include "src/Database/Database.h"

void testDatabaseOperations();

void testSQLParser();

void printSeparator();

int main()
{
    testDatabaseOperations();
    printSeparator();
    testSQLParser();

    return 0;
}

void printSeparator()
{
    std::cout << "\n----------------------------------------\n\n";
}

void testDatabaseOperations()
{
    // Initialize database
    Database db("test_database.cl");

    std::cout << "Database Demo\n";
    printSeparator();

    // 1. Create Tables
    std::cout << "1. Creating Tables\n";
    {
        // Create users table
        std::vector<std::string> userColumns = {"id", "name", "age", "city"};
        db.createTable("users", userColumns);

        // Create products table
        std::vector<std::string> productColumns = {"id", "name", "price", "stock"};
        db.createTable("products", productColumns);

        // Show all tables
        std::cout << "\nCurrent tables in database:\n";
        db.showTables();
    }
    printSeparator();

    // 2. Insert Records
    std::cout << "2. Inserting Records\n";
    {
        // Insert users
        db.insertRecord("users", {"1", "John Doe", "25", "New York"});
        db.insertRecord("users", {"2", "Jane Smith", "30", "Los Angeles"});
        db.insertRecord("users", {"3", "Bob Johnson", "35", "Chicago"});
        db.insertRecord("users", {"4", "Alice Brown", "28", "New York"});

        // Insert products
        db.insertRecord("products", {"1", "Laptop", "999.99", "50"});
        db.insertRecord("products", {"2", "Smartphone", "599.99", "100"});
        db.insertRecord("products", {"3", "Headphones", "99.99", "200"});
    }
    printSeparator();

    // 3. Create Indexes
    std::cout << "3. Creating Indexes\n";
    {
        db.createIndex("users", "city");
        db.createIndex("products", "price");
    }
    printSeparator();

    // 4. Select Records
    std::cout << "4. Selecting Records\n";
    {
        std::cout << "All users:\n";
        db.selectAll("users");

        std::cout << "\nAll products:\n";
        db.selectAll("products");

        std::cout << "\nUsers from New York:\n";
        WhereClause whereCity("city", WhereClause::Operator::EQUALS, "New York");
        db.selectAll("users", &whereCity);

        std::cout << "\nProducts with price > 500:\n";
        WhereClause wherePrice("price", WhereClause::Operator::GREATER_THAN, "500");
        db.selectAll("products", &wherePrice);
    }
    printSeparator();

    // 5. Update Records
    std::cout << "5. Updating Records\n";
    {
        // Update all users from New York to Boston
        WhereClause whereNY("city", WhereClause::Operator::EQUALS, "New York");
        db.updateRecords("users", "city", "Boston", &whereNY);

        std::cout << "\nUsers after update:\n";
        db.selectAll("users");
    }
    printSeparator();

    // 6. Delete Records
    std::cout << "6. Deleting Records\n";
    {
        // Delete products with price less than 100
        WhereClause whereCheap("price", WhereClause::Operator::LESS_THAN, "100");
        db.deleteRecords("products", &whereCheap);

        std::cout << "\nProducts after deletion:\n";
        db.selectAll("products");
    }
    printSeparator();

    // 7. Error Handling Examples
    std::cout << "7. Error Handling Examples\n";
    {
        // Try to create existing table
        std::cout << "\nTrying to create existing table:\n";
        std::vector<std::string> columns = {"id", "name"};
        db.createTable("users", columns);

        // Try to insert with wrong number of columns
        std::cout << "\nTrying to insert with wrong number of columns:\n";
        db.insertRecord("users", {"1", "John"}); // Should fail (4 columns required)

        // Try to update non-existent column
        std::cout << "\nTrying to update non-existent column:\n";
        db.updateRecords("users", "non_existent", "value", nullptr);

        // Try to query non-existent table
        std::cout << "\nTrying to query non-existent table:\n";
        db.selectAll("non_existent_table");
    }
    printSeparator();

    // 8. Transaction Example
    std::cout << "8. Transaction Example\n";
    {
        // This should work as one atomic operation
        std::cout << "\nInserting new user:\n";
        db.insertRecord("users", {"5", "Eve Wilson", "32", "Miami"});

        std::cout << "\nFinal users table:\n";
        db.selectAll("users");
    }
    printSeparator();

    // 9. Complex Where Clause Examples
    std::cout << "9. Complex Where Clauses\n";
    {
        std::cout << "\nUsers aged over 30:\n";
        WhereClause whereAge("age", WhereClause::Operator::GREATER_THAN, "30");
        db.selectAll("users", &whereAge);

        std::cout << "\nProducts with names containing 'phone':\n";
        WhereClause whereName("name", WhereClause::Operator::LIKE, "phone");
        db.selectAll("products", &whereName);
    }

    std::cout << "\nDatabase demo completed!\n";
}

void testSQLParser()
{
    Database db("sql_test.cl");

    // Create a test table
    std::vector<std::string> columns = {"id", "name", "price"};
    db.createTable("products", columns);

    // Test various SQL queries
    std::cout << "\nTesting SQL queries:\n";

    // Insert
    db.executeSQL("INSERT INTO products VALUES (1, 'Smartphone', 599.99)");
    db.executeSQL("INSERT INTO products VALUES (2, 'Laptop', 999.99)");
    db.executeSQL("INSERT INTO products VALUES (3, 'Headphones', 99.99)");

    // Select
    std::cout << "\nAll products:\n";
    db.executeSQL("SELECT * FROM products");

    std::cout << "\nProducts with 'phone' in name:\n";
    db.executeSQL("SELECT * FROM products WHERE name LIKE 'phone'");

    std::cout << "\nProducts with price > 500:\n";
    db.executeSQL("SELECT * FROM products WHERE price > 500");

    // Update
    db.executeSQL("UPDATE products SET price = 549.99 WHERE id = 1");

    std::cout << "\nAfter update:\n";
    db.executeSQL("SELECT * FROM products");

    // Delete
    db.executeSQL("DELETE FROM products WHERE price < 100");

    std::cout << "\nAfter delete:\n";
    db.executeSQL("SELECT * FROM products");

    // Drop
    db.executeSQL("DROP TABLE products");
}