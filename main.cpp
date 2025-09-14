#include "simulation.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

config read_config();

vector<vector<zone>> read_layout(const string region);


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

    config_file.close();
    

    return configData;


};

// Reading the layout file in csv format.

vector<vector<zone>> read_layout(const string region)
{   
    vector<vector<zone>> grid;

    ifstream regionLayout (region);

    if (!regionLayout.is_open())
    {
        string file_name;
        cout << "Enter the Region Layout file: ";
        getline(cin, file_name);

        ifstream regionLayout(file_name);

        if (!regionLayout.is_open()){
            cout << "Error: The Region Layout file could not be loaded: " << file_name << endl;
            return grid;
        }
    }

    string line;

    while (getline(regionLayout, line)) {
        vector<zone> row;
        string layout;
        stringstream ss(line);

        while (getline(ss, layout, ',')) {
            zone cell;
            cell.type = layout[0];   // First character of cell
            cell.population = 0;
            cell.pollution = 0;
            row.push_back(cell);
        }
        grid.push_back(row);
    }

    regionLayout.close();

    return grid;

};




int main()
{
    config configData = read_config();

    if (configData.limit == -1 || configData.rate == -1){cout << "Error reading the config file." << endl; return -1;}

    vector<vector<zone>> grid = read_layout(configData.layout);

    if (grid.size() == 0) {cout <<"The Region Layout file could not be loaded properly." << endl; return -1;}
    






    return 0;
}