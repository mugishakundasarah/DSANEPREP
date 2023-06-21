#ifndef DATABASE_CONNECTOR_H
#define DATABASE_CONNECTOR_H

#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "Customer.h"

class DatabaseConnector {
private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

public:
    DatabaseConnector(const std::string& host, const std::string& user, const std::string& password, const std::string& database) {
        try {
            driver = sql::mysql::get_mysql_driver_instance();
            con = driver->connect(host, user, password);
            con->setSchema(database);

            std::cout << "Connection established successfully!" << std::endl;
        }
        catch (sql::SQLException &e) {
            std::cout << "SQL Exception occurred: " << e.what() << std::endl;
        }
    }

    ~DatabaseConnector() {
        if (con) {
            con->close();
            delete con;
        }
    }

    // Include additional functions for database operations as needed
    void addCustomer(const std::string& name, const std::string& cashPowerNumber, const std::string& category) {
        try {
            sql::Statement *stmt = con->createStatement();
            stmt->execute("INSERT INTO customers (name, cash_power_number, category) VALUES ('" + name + "', '" + cashPowerNumber + "', '" + category + "')");
            delete stmt;
        }
        catch (sql::SQLException &e) {
            std::cout << "SQL Exception occurred: " << e.what() << std::endl;
        }
    }
    
    Customer getCustomer(const std::string& cashPowerNumber) {
        try {
            sql::Statement *stmt = con->createStatement();
            sql::ResultSet *res = stmt->executeQuery("SELECT * FROM customers WHERE cash_power_number = '" + cashPowerNumber + "'");

            if (res->next()) {
                // Cursor is positioned on a valid row, fetch the data
                Customer customer(res->getString("name"), res->getString("cash_power_number"), res->getString("category"));
                return customer;
            } else {
                // No matching row found
                // Handle the case when no customer is found with the given cash_power_number
                return Customer("", "", "");
            }

            // Don't forget to clean up the result set and statement
            delete res;
            delete stmt;
        }
        catch (sql::SQLException &e) {
            std::cout << "SQL Exception occurred: " << e.what() << std::endl;
        }
         // Add a return statement here
        return Customer("", "", "");  // Return a default-constructed Customer object
    }

    void deleteCustomer(const std::string& cashPowerNumber) {
        try {
            sql::Statement *stmt = con->createStatement();
            stmt->execute("DELETE FROM customers WHERE cash_power_number = '" + cashPowerNumber + "'");
            delete stmt;
        }
        catch (sql::SQLException &e) {
            std::cout << "SQL Exception occurred: " << e.what() << std::endl;
        }
    }
   
    void buyElectricity(const std::string& cashPowerNumber, const int amount, const float power) {
    try {
        sql::PreparedStatement *stmt = con->prepareStatement("SELECT cash_power_number FROM customers WHERE cash_power_number = ?");
        stmt->setString(1, cashPowerNumber);
        sql::ResultSet *res = stmt->executeQuery();
        
        if (res->next()) {
            // Cash power number exists, proceed with inserting the electricity record
            delete stmt;
            delete res;
            
            stmt = con->prepareStatement("INSERT INTO electricity (amount, power, cash_power_number) VALUES (?, ?, ?)");
            stmt->setInt(1, amount);
            stmt->setDouble(2, power);
            stmt->setString(3, cashPowerNumber);
            stmt->executeUpdate();
            
            std::cout << "Electricity recorded successfully!" << std::endl;
        } else {
            std::cout << "Customer not found" << std::endl;
        }
        
        delete stmt;
    }
    catch (sql::SQLException &e) {
        std::cout << "SQL Exception occurred: " << e.what() << std::endl;
    }
}

    
   
};

#endif  // DATABASE_CONNECTOR_H