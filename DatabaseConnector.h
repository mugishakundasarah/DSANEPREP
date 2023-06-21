#ifndef DATABASE_CONNECTOR_H
#define DATABASE_CONNECTOR_H

#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
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
            
            Customer customer(res->getString("name"), res->getString("cash_power_number"), res->getString("category"));
            
            delete res;
            delete stmt;

            return customer;
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
   
    void buyElectricity(const int amount, const std::string& cashPowerNumber, const float power) {
    try {
        
    }
    catch (sql::SQLException &e) {
        std::cout << "SQL Exception occurred: " << e.what() << std::endl;
    }
}



};

#endif  // DATABASE_CONNECTOR_H