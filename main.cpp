#include "simulation.h"
#include "commercial.h"
#include "residential.h"
#include "industrial.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <thread>
#include <chrono>

using namespace std;

config read_config();

vector<vector<zone>> read_layout(const string region);

void print_layout(const vector<vector<zone>>& grid);


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

        config_file.open(file_name);

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

        regionLayout.open(file_name);

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
            cell.type = toupper(layout[0]);   // First character of cell
            if (isspace(cell.type) || cell.type == '\0') {cell.type = ' ';} // Converts all whitespace to space
            cell.population = 0;
            cell.pollution = 0;
            cell.goods = 0;
            cell.job = 0;
            cell.count = 1;
            row.push_back(cell);
        }
        grid.push_back(row);
    }

    regionLayout.close();

    return grid;

};


// Printing the layout. The zones are seperated by single space. Can be changed easily for better visualization.
void print_layout(const vector<vector<zone>>& grid)
{   
    string space = " "; //Change space value for adjusting the space between zone cells.


    for (vector<vector<zone>>::const_iterator row = grid.begin(); row != grid.end(); row++)
    {
        for (vector<zone>::const_iterator cell = row->begin(); cell != row->end(); cell++)
        {   
            // Show letter if population = 0 otherwise show the populatioin for R, I, and C. Everything else, show the type.

            if ((cell->type == 'R' || cell->type == 'I' || cell->type == 'C') && cell->population == 0) {
                cout << cell->type << space;
            }
            else if ((cell->type == 'R' || cell->type == 'I' || cell->type == 'C') && cell->population > 0){
                cout << cell->population << space;
            }
            else {

                cout << cell->type << space;
            }
        }

        cout << endl;


    }

    cout << endl;



    return;

};



int main()
{
    config configData = read_config();

    if (configData.limit == -1 || configData.rate == -1){cout << "Error reading the config file." << endl; return -1;}

    vector<vector<zone>> grid = read_layout(configData.layout);

    if (grid.size() == 0) {cout <<"The Region Layout file could not be loaded properly." << endl; return -1;}


    // Initialize the time step 0.
    configData.timestep = 0;

    cout << "Time Step: 0" << endl;

    print_layout(grid);
    this_thread::sleep_for(chrono::seconds(configData.rate));

    bool state = false;

    int stateCount = 0;


    for (configData.timestep = 1; configData.timestep <= configData.limit; configData.timestep++)
    {
        state = false;

        for (int i = 0; i < 3; i++)
        {
            if (i == 0)
            {
                updateCommercial(grid, state);

                if (state == true) {break;}
            }

            else if (i == 1)
            {
                updateIndustrial(grid, state);
                if (state == true) {break;}
            }

            else if ( i == 2)
            {
                updateResidential(grid, state);
                if (state == true) {break;}
            }

        }

        if (state == false){stateCount = stateCount + 1;}
        else {stateCount = 0;}

        if (stateCount == 2) {break;}



        cout << "Time Step: " << configData.timestep << endl
        << "Available Workers: " << available(grid, 'R').job << endl
        << "Available Goods: " << available(grid, 'I').goods << endl;
        
        print_layout(grid);
        this_thread::sleep_for(chrono::seconds(configData.rate));
        
        
    }


    cout<< "Final State: " << endl
        << "Total Population: " << available(grid, 'C').population + available(grid, 'I').population + available(grid, 'R').population << endl
        << "Commercial Population: " << available(grid, 'C').population << endl
        << "Industrial Population: " << available(grid, 'I').population << endl
        << "Residential Population: " << available(grid, 'R').population << endl
        << "Total Pollution: " << available(grid, 'C').pollution + available(grid, 'I').pollution + available(grid, 'R').pollution + available(grid, '-').pollution +
        available(grid, 'T').pollution + available(grid, '#').pollution + available(grid, 'P').pollution + available(grid, ' ').pollution << endl
        << "Commercial Pollution: " << available(grid, 'C').pollution << endl
        << "Industrial Pollution: " << available(grid, 'I').pollution << endl
        << "Residential Pollution: " << available(grid, 'R').pollution << endl;
        
    
    print_layout(grid);

    char choice;
    position cell, cell2;

    int cell1Population, cell2Population, cell1Pollution, cell2Pollution;

    // Prompting user for looking at specific region

    while(true)
    {
        cout << "Do you want to take a closer look at a region?: [y/n] ";
        
        while (!(cin >> choice)) // error handling when invalid input is given
        {
            cout << "Invalid input. Please try again with: [y/n]" << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        }
        

        if (choice == 'y' || choice == 'Y')
        {   
            while (true)
            {
            
                cout << "Please enter the X-Coordinate (Column): ";

                while (!(cin >> cell.col)) // error handling when invalid input is given
                {
                    cout << "Invalid input. Please try again with a valid integer." << endl;
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                }

                cout << "Please enter the Y-Coordinate (Row): ";

                while (!(cin >> cell.row)) // error handling when invalid input is given
                {
                    cout << "Invalid input. Please try again with a valid integer." << endl;
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                }

                if (cell.row < 0 || cell.col < 0) {cout << "Invalid input. Please enter a positive number." << endl; continue;}

                if ( cell.row > (static_cast<int>(grid.size())-1) || cell.col > (static_cast<int>(grid.at(0).size())-1))
                {
                    cout << "Invalid input. Please enter a value X-Coordinate: 0-" << grid.at(0).size()-1 << " Y-Coordinate: 0-" << grid.size()-1 << endl;
                    continue;
                }


                cout << "Total Population of the Area: " << selectRegion(grid, cell, 'C').population + selectRegion(grid, cell, 'I').population + selectRegion(grid, cell, 'R').population << endl
                << "Commercial Population of the Area: " << selectRegion(grid, cell, 'C').population << endl
                << "Industrial Population of the Area: " << selectRegion(grid, cell, 'I').population << endl
                << "Residential Population of the Area: " << selectRegion(grid, cell, 'R').population << endl
                << "Total Pollution of the Area: " << selectRegion(grid, cell, 'C').pollution + selectRegion(grid, cell, 'I').pollution + selectRegion(grid, cell, 'R').pollution + selectRegion(grid, cell, '-').pollution +
                selectRegion(grid, cell, 'T').pollution + selectRegion(grid, cell, '#').pollution + selectRegion(grid, cell, 'P').pollution + selectRegion(grid, cell, ' ').pollution << endl
                << "Commercial Pollution of the Area: " << selectRegion(grid, cell, 'C').pollution << endl
                << "Industrial Pollution of the Area: " << selectRegion(grid, cell, 'I').pollution << endl
                << "Residential Pollution of the Area: " << selectRegion(grid, cell, 'R').pollution << endl;

                cout << endl;
                break;

            }

        }

        else if (choice == 'n' || choice == 'N'){break;}
        else {cout << "Invalid input. Please try again." << endl;}

        cout << endl;

        /************************************************************************************************************************************************/
        // Compare to another region

        cout << "Do you want to compare this to a different region?: [y/n] ";

        while (!(cin >> choice)) // error handling when invalid input is given
        {
            cout << "Invalid input. Please try again with: [y/n]" << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        }

        if (choice == 'y' || choice == 'Y')
        {   
            while (true)
            {
            
                cout << "Please enter the X-Coordinate (Column): ";

                while (!(cin >> cell2.col)) // error handling when invalid input is given
                {
                    cout << "Invalid input. Please try again with a valid integer." << endl;
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                }

                cout << "Please enter the Y-Coordinate (Row): ";

                while (!(cin >> cell2.row)) // error handling when invalid input is given
                {
                    cout << "Invalid input. Please try again with a valid integer." << endl;
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                }

                if (cell2.row < 0 || cell2.col < 0) {cout << "Invalid input. Please enter a positive number." << endl; continue;}

                if ( cell2.row > (static_cast<int>(grid.size())-1) || cell2.col > (static_cast<int>(grid.at(0).size())-1))
                {
                    cout << "Invalid input. Please enter a value X-Coordinate: 0-" << grid.at(0).size()-1 << " Y-Coordinate: 0-" << grid.size()-1 << endl;
                    continue;
                }


                cout << "Total Population of the Area: " << selectRegion(grid, cell2, 'C').population + selectRegion(grid, cell2, 'I').population + selectRegion(grid, cell2, 'R').population << endl
                << "Commercial Population of the Area: " << selectRegion(grid, cell2, 'C').population << endl
                << "Industrial Population of the Area: " << selectRegion(grid, cell2, 'I').population << endl
                << "Residential Population of the Area: " << selectRegion(grid, cell2, 'R').population << endl
                << "Total Pollution of the Area: " << selectRegion(grid, cell2, 'C').pollution + selectRegion(grid, cell2, 'I').pollution + selectRegion(grid, cell2, 'R').pollution + selectRegion(grid, cell2, '-').pollution +
                selectRegion(grid, cell2, 'T').pollution + selectRegion(grid, cell2, '#').pollution + selectRegion(grid, cell2, 'P').pollution + selectRegion(grid, cell2, ' ').pollution << endl
                << "Commercial Pollution of the Area: " << selectRegion(grid, cell2, 'C').pollution << endl
                << "Industrial Pollution of the Area: " << selectRegion(grid, cell2, 'I').pollution << endl
                << "Residential Pollution of the Area: " << selectRegion(grid, cell2, 'R').pollution << endl;

                cout << endl;

                cell1Population = (selectRegion(grid, cell, 'C').population + selectRegion(grid, cell, 'I').population + selectRegion(grid, cell, 'R').population);
                cell2Population = (selectRegion(grid, cell2, 'C').population + selectRegion(grid, cell2, 'I').population + selectRegion(grid, cell2, 'R').population);

                cell1Pollution = (selectRegion(grid, cell, 'C').pollution + selectRegion(grid, cell, 'I').pollution + selectRegion(grid, cell, 'R').pollution + selectRegion(grid, cell, '-').pollution +
                selectRegion(grid, cell, 'T').pollution + selectRegion(grid, cell, '#').pollution + selectRegion(grid, cell, 'P').pollution + selectRegion(grid, cell, ' ').pollution);

                cell2Pollution = (selectRegion(grid, cell2, 'C').pollution + selectRegion(grid, cell2, 'I').pollution + selectRegion(grid, cell2, 'R').pollution + selectRegion(grid, cell2, '-').pollution +
                selectRegion(grid, cell2, 'T').pollution + selectRegion(grid, cell2, '#').pollution + selectRegion(grid, cell2, 'P').pollution + selectRegion(grid, cell2, ' ').pollution);

                if (cell1Population > cell2Population)
                {
                    cout << "First Region (" << cell.col << "," << cell.row << ") has greater population density than Second Region (" << cell2.col << "," << cell2.row << "): " << cell1Population << " > " << cell2Population << endl;
                }
                else if (cell1Population < cell2Population)
                {
                    cout << "Second Region (" << cell2.col << "," << cell2.row << ") has greater population density than First Region (" << cell.col << "," << cell.row << "): " << cell2Population << " > " << cell1Population << endl;
                }
                else
                {
                    cout << "First Region and Second Region has same population density: " << cell1Population << endl;
                }



                if (cell1Pollution > cell2Pollution)
                {
                    cout << "First Region (" << cell.col << "," << cell.row << ") has greater population density than Second Region (" << cell2.col << "," << cell2.row << "): " << cell1Pollution << " > " << cell2Pollution << endl;
                }
                else if (cell1Pollution < cell2Pollution)
                {
                    cout << "Second Region (" << cell2.col << "," << cell2.row << ") has greater population density than First Region (" << cell.col << "," << cell.row << "): " << cell2Pollution << " > " << cell1Pollution << endl;
                }
                else
                {
                    cout << "First Region and Second Region has same population density: " << cell1Pollution << endl;
                }
                


                break;

            }

        }
        else if (choice == 'n' || choice == 'N'){break;}
        else {cout << "Invalid input. Please try again." << endl;}

        cout << endl;


    }


    return 0;
}