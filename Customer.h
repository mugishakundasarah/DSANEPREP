#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
using namespace std;

class Customer {
private:
    std::string name;
    std::string cashPowerNumber;
    std::string category;

public:
    Customer(const std::string& customerName, const std::string& powerNumber, const std::string& customerCategory)
        : name(customerName), cashPowerNumber(powerNumber), category(customerCategory) {
    }

    // Getter methods
    std::string getName() const {
        return name;
    }

    std::string getCashPowerNumber() const {
        return cashPowerNumber;
    }

    std::string getCategory() const {
        return category;
    }
};

#endif