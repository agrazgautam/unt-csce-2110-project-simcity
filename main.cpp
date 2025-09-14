#include "simulation.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

config read_config();



config read_config()
{   
    // Readig the config file.
    ifstream config_file("config1.txt");

    config configData;

    // If not found, prompting the user to enter the config file.
    if (!config_file.is_open()){

        string file_name;
        cout << "Enter the config file: ";
        getline(cin, file_name);

        ifstream config_file(file_name);

        if (!config_file.is_open()){
            cout << "Error: Config file could not be loaded: " << file_name << endl;
            return configData;
        }
    }

    cout << "Config file is loaded."<< endl;

    // Reading the config file line by line and taking values after :
    string line;

    getline(config_file, line, ':');

    getline(config_file, configData.layout);

    getline(config_file, line, ':');

    // Exception handling to ensure the int values are properly read from config file.
    
    try
    {
        getline(config_file, line);
        configData.limit = stoi(line);
    }

    catch(const exception& e)
    {
        std::cerr << "Error reading the Time Limit value. Cause: " << e.what() << '\n';
    };
    
    getline(config_file, line, ':');

    try
    {
        getline(config_file, line);
        configData.rate = stoi(line);
    }

    catch(const exception& e)
    {
        std::cerr << "Error reading the Refresh Rate value. Cause: " << e.what() << '\n';
    };

    

    return configData;


};





int main()
{
    config configData = read_config();

    if (configData.limit == -1 || configData.rate == -1){cout << "Error reading the config file." << endl; return -1;}




    return 0;
}