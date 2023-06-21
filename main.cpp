#include <iostream>
// for stringstream which reads data from a string
#include <sstream>
#include <vector>
// for functions like toUpper 
#include <cctype>
// for functions like transform
#include <algorithm>
// for the stoi function
#include <string.h>

#include "DatabaseConnector.h"
#include "Customer.h"

using namespace std;


DatabaseConnector connector("localhost", "root", "root@123", "ELECTRICITY_PAYMENT_APP");

// util functions
string getUserInput(string input) {
    cout << "Console > ";
    getline(cin, input);
    return input;
}

int checkIfIncludes(vector<string> data, string searchValue){
    int found = 0;
    for(const auto& token : data){
        if(token == searchValue){
            found = 1;
        }
    }
    return found;
}

void showHelpMenu(){
    cout << "========================================================================" << endl;
    cout << endl;
    cout << "                          H   E    L   P   M   E   N   U                *" <<endl;
    cout << endl;
    cout << "========================================================================" << endl;
    cout << endl; 

    cout << "add <Customer name> <Cash Power Number> <Category>            :Add a new customer" << endl;
    cout << endl;
    
    cout << "get <cash power number>                                       :Get customer by cash power number" << endl;
    cout << endl; 

    cout << "delete <cash power number>                                    :Get customer by cash power number" << endl;
    cout << endl; 

    cout << "buy <cash power number> <amount>                              :Buy electricity" << endl;
    cout << endl;
    cout << "help                                                          :Prints user manual" << endl;
    cout << endl;
    cout << "Exit                                                          :Exits the program" << endl;
    cout << endl;
}

string getCurrentTime(){
    time_t currentTime = time(nullptr);

    // Format the time using std::strftime
    char formattedTime[50];
    strftime(formattedTime, sizeof(formattedTime), "%a %b %d %H:%M:%S CAT %Y", localtime(&currentTime));
    return formattedTime ;
}

void showWelcome(){
    cout << "=========================================================" << endl;
    cout << endl;
    cout << "*      Welcome to ELECTRICITY BUYING APP v1.0!       *" << endl;
    cout << endl;
    cout << "*  *******************************************  *        " << endl;
    cout << endl;
    cout << "*                             *                          " << endl;
    cout << endl;
    cout << "* It is developed by 'Kunda Mugisha Sarah' as practical *" <<endl;
    cout << endl;
    cout << "* evaluation for the end of year 3." << endl;
    cout <<endl;
    cout << "=========================================================" << endl;
    cout<< endl;
    cout << "Starting Time: " << getCurrentTime() << endl;
    cout << endl;
    cout << "Need a help? Type 'help' then press Enter key." << endl;
    cout << endl;
}

vector<string> toUpperCase(vector<string> data){
    for(auto& item : data){
        transform(item.begin(), item.end(), item.begin(), [](char c) {
            return toupper(c);
        });
    }

    return data;
}

void printOutput(string output){
    cout << "\t" << output << endl;
}

float calculateResidentialPower(int amount) {
    float power;
    int currAmount;
    if(amount <= 1335){
            power = amount/89;
        return power;
    }
    else if(amount > 1335 && amount<=8755){
            power = 15;
        currAmount = amount - 1335;
        power+=currAmount/212;
        return power;
    }else{
            power = 50;
        currAmount = amount-8775;
        power+=currAmount/249;
        return power;
    }
}

const vector<string> categories = {"RESIDENTIAL", "NON_RESIDENTIAL", "TELECOM_TOWERS", "WATER_TREATMENT_PLANTS_AND_WATER_PUMPING_STATIONS", "HOTELS", "HEALTH_FACILITIES", "BROADCASTERS", "COMMERCIAL_DATA_CENTERS"};
int main() {
    bool exit = false;
    string featureRequest;
    string featureRequestToken;
    vector<string> featureRequestArray; 

    showWelcome();
    
    while (!exit)
    {
        // Get user input
        string featureRequest = "";
        featureRequest = getUserInput(featureRequest);
        // read featureRequest into an object to split it
        stringstream ss(featureRequest);
        while(ss >> featureRequestToken){
            featureRequestArray.push_back(featureRequestToken);
        }
        // convert every user data to uppercase 
        featureRequestArray = toUpperCase(featureRequestArray);
        string command = featureRequestArray[0];

        if(command == "ADD"){
            if(featureRequestArray.size() != 4){
                printOutput("Invalid number of arguments");
                continue;
            }

            // TODO: validate input data
            string customerName = featureRequestArray[1];
            string cashPowerNumber = featureRequestArray[2];
            string category = featureRequestArray[3];

            if(!checkIfIncludes(categories, category)){
                printOutput("Invalid category");
                continue;
            }

            connector.addCustomer(customerName, cashPowerNumber, category);
        }
        else if(command == "GET"){
            if(featureRequestArray.size() != 2){
                printOutput("Invalid number of arguments");
                continue;
            }

            string cashPowerNumber = featureRequestArray[1];
            Customer customer = connector.getCustomer(cashPowerNumber);
            printOutput("Customer Name: " + customer.getName());
            printOutput("Cash Power Number: " + customer.getCashPowerNumber());
            printOutput("Category: " + customer.getCategory());
        }
        else if(command == "DELETE"){
            if(featureRequestArray.size() != 2){
                printOutput("Invalid number of arguments");
                continue;
            }

            string cashPowerNumber = featureRequestArray[1];
            connector.deleteCustomer(cashPowerNumber);
        }
        else if(command == "BUY"){
            if(featureRequestArray.size() != 3){
                printOutput("Invalid number of arguments");
                continue;
            }

            string cashPowerNumber = featureRequestArray[1];
            int amount =  stoi(featureRequestArray[2]);
    
            Customer customerData = connector.getCustomer(cashPowerNumber);
            string category = customerData.getCategory();

            if(category == "RESIDENTIAL"){
                int power = calculateResidentialPower(amount);
                connector.buyElectricity(amount, cashPowerNumber, power);
            }
        }
        else if(command == "HELP"){
            showHelpMenu();
        }
        else if(command == "EXIT"){
            exit = true;
        }
        else{
            printOutput("Invalid command");
        }
        featureRequest = "" ;
        featureRequestArray.clear();
    }
    return 0;   
}