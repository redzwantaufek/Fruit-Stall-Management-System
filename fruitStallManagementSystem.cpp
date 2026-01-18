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
const double MIN_WEIGHT_MEMBER = 3.0;

//function prototypes
void getData(Item [], int &, ifstream &);
void saveData(Item inv[], int size);  // Changed: no ofstream param
void addFruit(Item [], int &);
int searchFruit(Item [], int, string);
void deleteFruit(Item [], int &);
void displayInventory(Item [], int);
void processSale(Item [], int , double [][STATS]);
void restockItem(Item *fptr);
void checkExpiry(Item [], int);
void generateReports(double [][STATS]);

int main()
{
    Item inventory[MAX_FRUITS];// declare struct array
    double monthlySales[DAYS_IN_MONTH][STATS] = {0};//declare 2D array to store monthly sales
    int fruitCount = 0;//to store number of fruits
    int choice;// to store user choice

    //input file
    ifstream in;
    in.open("inventory.txt");
    //call funct get data
    getData(inventory, fruitCount, in);
    //main menu
    do {
        cout << "\n========================================="<<endl;
        cout << "Welcome to the Fruit Stall Management System!" << endl;
        cout << "=========================================" << endl;
        cout << "1. View Inventory" << endl;
        cout << "2. Add New Fruit" << endl;
        cout << "3. Delete Fruit" << endl;
        cout << "4. Point of Sale System (POS System)" << endl;
        cout << "5. Restock Item" << endl;
        cout << "6. Check Expiring Items" << endl;
        cout << "7. Management Reports" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter Menu (1-8): ";
        cin >> choice;
        cin.ignore(); 

        if (choice == 1)
            displayInventory(inventory, fruitCount);
        else if (choice == 2){
            addFruit(inventory, fruitCount);
            saveData(inventory, fruitCount);
            cout << ">> Auto-Save: File updated successfully.\n";
        }
        else if (choice == 3)
        {
            deleteFruit(inventory, fruitCount);
            saveData(inventory, fruitCount);
        }
        else if (choice == 4)
            processSale(inventory, fruitCount, monthlySales);
        else if (choice == 5)
        {
            string searchName;
            cout << "Enter fruit name to restock: ";
            getline(cin, searchName); // Allow spaces in search
            int index = searchFruit(inventory, fruitCount, searchName);
            if (index != -1)
                restockItem(&inventory[index]); 
            else cout << "Error: Fruit not found!\n";
        }
        else if (choice == 6)
            checkExpiry(inventory, fruitCount);
        else if (choice == 7)
            generateReports(monthlySales);
        else if (choice == 8)
        {
            saveData(inventory, fruitCount); 
            cout << "System Exiting... Data Saved.\n"; 
        }
        else
            cout << "Invalid input. Please enter number between 1-8.!!!!!"<<endl;
    } while (choice != 8);

    in.close();
}

//get data from file
void getData(Item inv[], int &size, ifstream &indata)
{
    
    if (!indata) return;//to check if file exists, if not it will return
    size = 0;//to make size = 0 everytime fuction is called
    // read file until eof
    while (!indata.eof()) 
    {
        //to read name from file
        getline(indata, inv[size].name, ';');
        //to read price from file
        indata >> inv[size].price; 
        indata.ignore(); // Skip the ;
        //to read weight from file
        indata >> inv[size].weight; 
        indata.ignore(); // Skip the ;
        // to read type from file
        getline(indata, inv[size].type, ';');
        //to read expiry day from file
        indata >> inv[size].expDay; 
        indata.ignore();
        //to read expiry month from file
        indata >> inv[size].expMonth; 
        indata.ignore();
        // to read expiry year from file
        indata >> inv[size].expYear; 
        indata.ignore(); // Skip newline at end of row
        
        size++;
    }
    indata.close();
}

//function to save data to file
void saveData(Item inv[], int size)
{
    ofstream outdata("inventory.txt");  // Open here
    for (int i = 0; i < size; i++) 
    {
        //write data to file with ; delimiter
        outdata << inv[i].name << ";" ;
        outdata << inv[i].price << ";";
        outdata << inv[i].weight << ";"; 
        outdata << inv[i].type << ";";
        outdata << inv[i].expDay << ";"; 
        outdata << inv[i].expMonth << ";"; 
        outdata << inv[i].expYear << endl;
    }
    outdata.close();  // Close here
}

//function to add new fruit to file inventory
void addFruit(Item inv[], int &size)
{
    if (size >= MAX_FRUITS) 
    {
        cout << "Error: Inv Full (Limit 500)!\n";
    }
    else
    {
        cout << "\n--- ADD NEW FRUIT ---\n";
        // Name 
        cout << "Name (e.g. Green Apple): "; 
        getline(cin, inv[size].name);
        // Price
        cout << "Price per KG: "; 
        cin >> inv[size].price;
        // Weight
        cout << "Weight (KG): "; 
        cin >> inv[size].weight;
        // Type
        cout << "Type (Local/Imported): "; 
        cin >> inv[size].type; // Type usually one word, cin is fine.
        // Expiry Date
        cout << "Expiry Day: ";
        cin >> inv[size].expDay;
        cout << "Expiry Month: ";
        cin >> inv[size].expMonth;
        cout << "Expiry Year: ";
        cin >> inv[size].expYear;
        
        size++; 
        cout << ">> Success: Fruit Added & Saved!\n";
    }
}

//function to search fruit by name
int searchFruit(Item inv[], int size, string searchName)
{
    int index = -1;//give index value -1 if not found
    for (int i = 0; i < size; i++) {
        if (inv[i].name == searchName) 
            index = i;
    }
    return index;
}

//function to delete fruit from inventory
void deleteFruit(Item inv[], int &size) 
{
    string name;
    cout << "\n--- DELETE FRUIT ---\n";
    cout << "Enter Name: "; 
    getline(cin, name);

    //search for fruit index
    int index = -1;
    for (int i = 0; i < size; i++) 
    {
        if (inv[i].name == name) 
        {
            index = i;
            break;  // Stop at first match
        }
    }

    if (index != -1)   // Only proceed if found
    {
        char confirm;
        cout << "Delete " << inv[index].name << "? (Y/N): ";
        cin >> confirm;

        if (confirm == 'Y' || confirm == 'y') 
        {
            for (int i = index; i < size - 1; i++) 
            {
                inv[i] = inv[i + 1];
            }
            size--; 
            cout << ">> Item Deleted.\n";
        } else 
        {
            cout << ">> Deletion Cancelled.\n";
        }
    } 
    else 
    {
        cout << "Not found.\n"; 
    }
}

//function to display the inventory
void displayInventory(Item inv[], int size) 
{
    cout << "\n-------- INVENTORY LIST --------"<<endl;
    cout << left << setw(20) << "Name" << setw(10) << "Price/KG" << setw(10) << "Stock(KG)" << setw(10) << "Type" << setw(12) << "Expiry" << endl;
    cout << "------------------------------------------------------------------"<<endl;
    for (int i = 0; i < size; i++) 
    {
        cout << left << setw(20) << inv[i].name << "RM" << fixed << setprecision(2) << setw(8) << inv[i].price << setw(10) << inv[i].weight << setw(10) << inv[i].type << inv[i].expDay << "/" << inv[i].expMonth << "/" << inv[i].expYear << endl;
    }
}

//function point of sale
void processSale(Item inv[], int size, double monthlyS[][STATS]) 
{
    string name;
    double weightRequest;
    int day;
    char isMember;

    cout << "\n================================"<<endl;
    cout << "      POINT OF SALE (POS)       "<<endl;
    cout << "================================"<<endl;
    cout << "Enter Fruit: "; 
    getline(cin, name); // [FIX: getline]

    //search for fruit index
    int index = -1;
    for (int i = 0; i < size; i++) 
    {
        if (inv[i].name == name) 
        {
            index = i;
            break;  // Stop at first match
        }
    }

    if (index == -1) 
    { 
        cout << ">> Error: Item not found."<<endl; 
    } 
    else 
    {
        cout << "Stock Available: " << inv[index].weight << " KG"<<endl;
        cout << "Enter Weight needed (KG): ";
        cin >> weightRequest;

        if (weightRequest > inv[index].weight) 
        { 
            cout << ">> Error: Insufficient stock!"<<endl; 
        } 
        else 
        {
            // Pointer variables for calculation
            double *grossTotal = new double;
            double *discountAmount = new double;
            double *netTotal = new double;

            *grossTotal = inv[index].price * weightRequest;
            *discountAmount = 0.0;

            cout << "Member? (Y/N): "; cin >> isMember;

            if ((isMember == 'Y' || isMember == 'y') && weightRequest >= MIN_WEIGHT_MEMBER)
            {
                *discountAmount = *grossTotal * MEMBER_DISCOUNT;
                cout << ">> BULK MEMBER DISCOUNT APPLIED!"<<endl;
            }

            *netTotal = *grossTotal - *discountAmount;
            double profit = *netTotal * 0.40; 
            
            inv[index].weight -= weightRequest;
            // Inline save data
            ofstream outdata("inventory.txt");
            for (int i = 0; i < size; i++) 
            {
                outdata << inv[i].name << ";" ;
                outdata << inv[i].price << ";";
                outdata << inv[i].weight << ";"; 
                outdata << inv[i].type << ";";
                outdata << inv[i].expDay << ";"; 
                outdata << inv[i].expMonth << ";"; 
                outdata << inv[i].expYear << endl;
            }
            outdata.close();

            cout << "Day of Month (1-30): "; cin >> day;
            if (day >= 1 && day <= 30) 
            {
                monthlyS[day-1][0] += *netTotal;
                monthlyS[day-1][1] += profit;
            }

            cout << "\n--------------------------------"<<endl;
            cout << "        SALES RECEIPT           "<<endl;
            cout << "--------------------------------"<<endl;
            cout << left << setw(20) << "Item:" << inv[index].name << endl;
            cout << left << setw(20) << "Price/KG:" << "RM " << inv[index].price << endl;
            cout << left << setw(20) << "Weight Sold:" << weightRequest << " KG" << endl;
            cout << "--------------------------------"<<endl;
            cout << left << setw(20) << "Gross Total:" << "RM " << *grossTotal << endl;
            cout << left << setw(20) << "Discount (-):" << "RM " << *discountAmount << endl;
            cout << "--------------------------------"<<endl;
            cout << left << setw(20) << "NET TO PAY:" << "RM " << *netTotal << endl;
            cout << "--------------------------------"<<endl;
            cout << "    Thank You Come Again!       "<<endl;
            cout << "--------------------------------"<<endl;

            delete grossTotal;
            delete discountAmount;
            delete netTotal;
        }
    }
}

//function to restock item (nak kene ubah lagi)
void restockItem(Item *fptr) 
{
    double qty;
    cout << "Current: " << fptr->weight << " KG. Add (KG): ";
    cin >> qty;
    fptr->weight += qty;
    cout << "Updated Stock: " << fptr->weight << " KG\n";
}

//check expiry dates
void checkExpiry(Item inv[], int size) 
{
    int currMonth = 1, currYear = 2026;
    cout << "\n--- EXPIRY ALERT (Jan 2026) ---"<<endl;
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (inv[i].expYear < currYear || (inv[i].expYear == currYear && inv[i].expMonth <= currMonth)) 
        {
            cout << "ALERT: " << inv[i].name << " expired on " << inv[i].expDay << "/" << inv[i].expMonth << "/" << inv[i].expYear << endl;
            found = true;
        }
    }
    if (found == false) 
        cout << "No expired items found.\n";
}

//function to generate reports
void generateReports(double monthlyS[][STATS]) 
{
    int reportChoice;
    cout << "\n--- MANAGEMENT REPORT CENTER ---\n";
    cout << "1. Weekly Report (Specific Week)\n";
    cout << "2. Monthly Report (Full Overview)\n";
    cout << "Select Option: ";
    cin >> reportChoice;

    if (reportChoice == 1) 
    {
        int week;
        cout << "Enter Week Number (1-4): ";
        cin >> week;
        if (week >= 1 && week <= 4) 
        {
            ofstream outWeekly;
            outWeekly.open("Weekly Report.txt");
            // Inline printWeeklyReport
            int startDay = (week - 1) * 7;
            int endDay = startDay + 7;
            double total = 0, totalProfit = 0;

            cout << "\n--- WEEKLY REPORT (Week " << week << ") ---\n";
            outWeekly << "\n--- WEEKLY REPORT (Week " << week << ") ---\n";
            cout << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << setw(15) << "Profit(RM)" << endl;
            outWeekly << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << setw(15) << "Profit(RM)" << endl;
            cout << "----------------------------------------\n";
            outWeekly << "----------------------------------------\n";

            for (int i = startDay; i < endDay && i < DAYS_IN_MONTH; i++) 
            {
                if (monthlyS[i][0] > 0) { 
                    cout << "Day " << left << setw(6) << (i + 1) 
                         << "RM " << setw(12) << monthlyS[i][0] 
                         << "RM " << monthlyS[i][1] << endl;
                    outWeekly << "Day " << left << setw(6) << (i + 1) 
                         << "RM " << setw(12) << monthlyS[i][0] 
                         << "RM " << monthlyS[i][1] << endl;
                    total += monthlyS[i][0];
                    totalProfit += monthlyS[i][1];
                }
            }
            cout << "----------------------------------------"<<endl;
            outWeekly << "----------------------------------------"<<endl;
            cout << "WEEK TOTAL SALES : RM " << total << endl;
            outWeekly << "WEEK TOTAL SALES : RM " << total << endl;
            cout << "WEEK NET PROFIT  : RM " << totalProfit << endl;
            outWeekly << "WEEK NET PROFIT  : RM " << totalProfit << endl;
            outWeekly.close();
        } 
        else 
        {
            cout << "Invalid Week.\n";
        }
    } 
    else if (reportChoice == 2) 
    {
        ofstream outMonthly;
        outMonthly.open("Monthly Report.txt");
        // Inline printMonthlyReport
        double total = 0, maxSales = -1;
        int highestDay = 0;

        cout << "\n--- MONTHLY PERFORMANCE REPORT ---"<<endl;
        outMonthly << "\n--- MONTHLY PERFORMANCE REPORT ---"<<endl;
        cout << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << endl;
        outMonthly << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << endl;
        
        for (int i = 0; i < DAYS_IN_MONTH; i++) 
        {
            if (monthlyS[i][0] > 0) 
            {
                cout << "Day " << left << setw(6) << (i + 1) 
                     << "RM " << monthlyS[i][0] << endl;
                outMonthly << "Day " << left << setw(6) << (i + 1) 
                     << "RM " << monthlyS[i][0] << endl;
                total += monthlyS[i][0];
                if (monthlyS[i][0] > maxSales) { maxSales = monthlyS[i][0]; highestDay = i + 1; }
            }
        }
        
        cout << "----------------------------------"<<endl;
        outMonthly << "----------------------------------"<<endl;
        cout << "TOTAL MONTH REVENUE : RM " << total << endl;
        outMonthly << "TOTAL MONTH REVENUE : RM " << total << endl;
        cout << "BEST PERFORMING DAY : Day " << highestDay << " (RM " << maxSales << ")"<<endl;
        outMonthly << "BEST PERFORMING DAY : Day " << highestDay << " (RM " << maxSales << ")"<<endl;
        outMonthly.close();
    } 
    else 
    {
        cout << "Invalid Selection.\n";
    }
}