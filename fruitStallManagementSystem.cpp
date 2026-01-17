/*
Title: Fruit Stall Management System
CSC404 M3CS2302A
Members:
1. Muhammad Redzwan Bin Md Taufek (2025152219)
2. Muhammad Naufal bin Abdul Rahim ()
3. Muhammad Izzat Azamuddin bin Azman ()
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

//declare struct
struct Item {
    string name;   
    double price;  
    double weight; 
    string type;      
    // Expiry Date
    int expDay;
    int expMonth;
    int expYear;
};

//declare constant variables
const int MAX_FRUITS = 500;
const int DAYS_IN_MONTH = 30;
const int STATS = 2; //SIZE OF COLUM
const double MEMBER_DISCOUNT = 0.05;
const double BULK_THRESHOLD_KG = 3.0;

//function prototypes
void getData(Item [], int &, ifstream &);

int main()
{
    Item inventory[MAX_FRUITS];// declare struct array
    double monthlySales[DAYS_IN_MONTH][STATS] = {0};//declare 2D array to store monthly sales
    int fruitCount = 0;//to store number of fruits
    int choice;// to store user choice

    //input file
    ifstream in;
    in.open("inventory.txt");

    getData(inventory, fruitCount, in);//call funct get data

    cout << "Welcome to the Fruit Stall Management System!" << endl;
}

//get data from file
void getData(Item i[], int &counter, ifstream &indata)
{
    
    if (!indata) return;//to check if file exists, if not it will return
    counter = 0;//to make count = 0 everytime fuction is called
    // read file until eof
    while (indata.eof()) {
        //to read name from file
        getline(indata, i[counter].name, ';');
        indata.ignore(); // Skip the ;
        //to read price from file
        indata >> i[counter].price; 
        indata.ignore(); // Skip the ;
        //to read weight from file
        indata >> i[counter].weight; 
        indata.ignore(); // Skip the ;
        // to read type from file
        getline(indata, i[counter].type, ';');
        //to read expiry day from file
        indata >> i[counter].expDay; 
        indata.ignore();
        //to read expiry month from file
        indata >> i[counter].expMonth; 
        indata.ignore();
        // to read expiry year from file
        indata >> i[counter].expYear; 
        indata.ignore(); // Skip newline at end of row
        
        counter++;
    }
    indata.close();
}

void writeData(Item i[], int count, ofstream &outdata)
{
    for (int j = 0; j < count; j++) {
        //write data to file with ; delimiter
        outdata << i[j].name << ";" 
                << i[j].price << ";" 
                << i[j].weight << ";" 
                << i[j].type << ";"
                << i[j].expDay << ";" 
                << i[j].expMonth << ";" 
                << i[j].expYear << endl;
    }
    outdata.close();
}