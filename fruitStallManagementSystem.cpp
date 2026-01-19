/*
Title: Fruit Stall Management System
CSC404 M3CS2302A
Members:
1. Muhammad Redzwan Bin Md Taufek (2025152219)
2. Muhammad Naufal bin Abdul Rahim (2025147243)
3. Muhammad Izzat Azamuddin bin Azman ()
*/
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

//declare struct for fruit item
struct Item {
    string name;   
    double price;  
    double weight; 
    string type;
};
//struct for cart item
struct Cart {
    int index;
    string name;
    double weight;
    double price;
    double subtotal;
};

//declare constant variables
const int MAX_FRUITS = 100;//max fruit in inventory
const int DAYS_IN_MONTH = 31;//days in month
const int TOTAL = 2; //0 total sales, 1 total items sold for 2d array
const double MEMBER_DISCOUNT = 0.05;//5% member discount
const double MIN_WEIGHT_MEMBER = 3.0;//min weight for member discount

//function prototypes
void getData(Item [], int &, ifstream &);
void saveData(Item inv[], int);
void addFruit(Item [], int &);
int searchFruit(Item [], int, string);
void deleteFruit(Item [], int &);
void displayInventory(Item [], int);
void processSale(Item [], int , double [][TOTAL]);
void generateReports(double [][TOTAL]);
void editFruit(Item [], int);

int main()
{
    Item inventory[MAX_FRUITS];// declare struct array, with max 500 fruits array
    double monthlySales[DAYS_IN_MONTH][TOTAL] = {0};//declare 2D array to store monthly sales
    int fruitCount = 0;//to store number of fruits
    int choice;// to store user choice

    //input file
    ifstream in;
    in.open("Inventory.txt");
    //call funct get data
    getData(inventory, fruitCount, in);
    //main menu
    do {
        cout << "\n============================================="<<endl;
        cout << "Welcome to the Fruit Stall Management System!" << endl;
        cout << "=============================================" << endl;
        cout << "1. View Inventory" << endl;
        cout << "2. Add New Fruit" << endl;
        cout << "3. Delete Fruit" << endl;
        cout << "4. Edit Fruit Details" << endl;
        cout << "5. Point of Sale System (POS System)" << endl;
        cout << "6. Management Reports" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter Menu (1-7): ";
        cin >> choice;
        cin.ignore(); 
  
        if (choice == 1)//display inventory
            displayInventory(inventory, fruitCount);
        else if (choice == 2){ //add new fruit
            addFruit(inventory, fruitCount);//call add fruit function
            saveData(inventory, fruitCount);//auto save after adding fruit
            cout << ">> Auto-Save: File updated successfully.\n";
        }
        else if (choice == 3)//delete fruit
        {
            deleteFruit(inventory, fruitCount);//call delete fruit function
            saveData(inventory, fruitCount);//auto save after deleting fruit
        }
        else if (choice == 4)//edit fruit details
        {
            editFruit(inventory, fruitCount);
        }
        else if (choice == 5)//point of sale system
            processSale(inventory, fruitCount, monthlySales);
        else if (choice == 6)//generate reports
            generateReports(monthlySales);
        
        else if (choice == 7) //exit program
        {
            saveData(inventory, fruitCount); 
            cout << "System Exiting... Data Saved.\n"; 
        }
        else//invalid input
            cout << "Invalid input. Please enter number between 1-9.!!!!!"<<endl;
            
    } while (choice != 7);

    in.close();
}

//get data from file
void getData(Item inv[], int &size, ifstream &indata)
{

        size = 0; //to make size 0 everytime function is called
        
        // read file
        while (getline(indata, inv[size].name, ';')) {
            //to read price from file
            indata >> inv[size].price; 
            indata.ignore();
            //to read weight from file
            indata >> inv[size].weight; 
            indata.ignore();
            // to read type from file
            getline(indata, inv[size].type);
            
            size++;
        }
}

//function to save data to file
void saveData(Item inv[], int size)
{
    ofstream outdata("Inventory.txt");//outpu file stream 
    for (int i = 0; i < size; i++) {
        //write data to file with ; delimiter
        outdata << inv[i].name << ";" ;
        outdata << fixed << setprecision(2) << inv[i].price << ";";
        outdata << inv[i].weight << ";"; 
        outdata << inv[i].type << endl;
    }
    outdata.close(); 
}

//function to add new fruit to file inventory
void addFruit(Item inv[], int &size)
{
    //check if inventory full
    if (size >= MAX_FRUITS) 
    {
        cout << "Error: Inv Full (Limit 100)!\n";
    }
    else
    {
        cout << "\n---------- ADD NEW FRUIT ----------\n";
        // Name
        cout << "Name (Exp: Green Apple): "; 
        getline(cin, inv[size].name);
        // Price
        cout << "Price per KG: "; 
        cin >> inv[size].price;
        // Weight
        cout << "Weight (KG): "; 
        cin >> inv[size].weight;
        // Type
        cout << "Type (Local/Imported): "; 
        cin >> inv[size].type;
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
    cout << "\n------------- DELETE FRUIT -------------\n";
    cout << "Enter Fruit Name: "; 
    getline(cin, name);

    //search for fruit index
    int index = -1;
    for (int i = 0; i < size; i++) {
        if(index == -1){ // Only search if not found yet
            if (inv[i].name == name) {
                index = i;
            }
        }
    }
    //if found, confirm deletion
    if (index != -1){
        char confirm;
        cout << "Delete " << inv[index].name << "? (Y/N): ";
        cin >> confirm;

        if (confirm == 'Y' || confirm == 'y') {
            for (int i = index; i < size - 1; i++) {
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
        cout << "Fruit Not found.\n"; 
    }
}

//function to display the inventory
void displayInventory(Item inv[], int size) 
{
    cout << "\n------------------------- INVENTORY LIST -------------------------"<<endl;
    cout << left << setw(20) << "Name" << setw(10) << "Price/KG" << setw(10) << "Stock(KG)" << setw(10) << "Type" << endl;
    cout << "------------------------------------------------------------------"<<endl;
    for (int i = 0; i < size; i++) 
    {
        cout << left << setw(20) << inv[i].name << "RM" << fixed << setprecision(2) << setw(8) << inv[i].price << setw(10) << inv[i].weight << setw(10) << inv[i].type << endl;
    }
}

// Function point of sale
void processSale(Item inv[], int size, double monthlyS[][TOTAL]) 
{
    Cart cart[50]; // Array to hold up to 50 items
    int cartCount = 0;// Number of items in cart
    char addMore = 'Y';//add ore items to cart or not
    char isMember;//member status
    double totalWeight = 0; // To check for discount
    // Pointer variables for calculation
    double *grossTotal = new double;// Pointer for gross total
    double *discountAmount = new double;// Pointer for discount amount
    double *netTotal = new double;// Pointer for net total
    
    *grossTotal = 0.0;
    *discountAmount = 0.0;
    *netTotal = 0.0;

    cout << "\n==========================================" << endl;
    cout << "      POINT OF SALE       " << endl;
    cout << "==========================================" << endl;

    //step 1 add item
    do {
        string name;
        cout << "\nEnter Fruit Name: "; 
        getline(cin, name); 

        //search for fruit index
        int index = -1;
        for (int i = 0; i < size; i++) {
            if(index == -1){ // Only search if not found yet

                if (inv[i].name == name) {
                    index = i;
                }
            }
        }

        if (index == -1) { 
            cout << ">> Error: Item not found." << endl; 
        } 
        else {//calculate the total price
            double weightRequest;
            cout << "Stock Available: " << inv[index].weight << " KG" << endl;
            cout << "Enter Weight (KG): ";
            cin >> weightRequest;
            cin.ignore();

            if (weightRequest > inv[index].weight) { 
                cout << ">> Error: Insufficient stock!" << endl; 
            } else {
                //cart item details
                cart[cartCount].index = index;
                cart[cartCount].name = inv[index].name;
                cart[cartCount].weight = weightRequest;
                cart[cartCount].price = inv[index].price;
                cart[cartCount].subtotal = inv[index].price * weightRequest;
                
                //update totals
                *grossTotal += cart[cartCount].subtotal;
                totalWeight += weightRequest;
                
                // Temporarily reduce stock in memory
                inv[index].weight -= weightRequest; 
                
                cartCount++;
                cout << ">> Item added to cart." << endl;
            }
        }

        cout << "Add another item? (Y/N): ";
        cin >> addMore;
        cin.ignore();

    } while ((addMore == 'Y' || addMore == 'y') && cartCount < 50);

    // step 2 checkout
    if (cartCount > 0) {
        cout << "\n------------------------------------------" << endl;
        cout << "Proceeding to Checkout..." << endl;
        
        cout << "Member? (Y/N): "; 
        cin >> isMember;

        //check discount eligibility
        if ((isMember == 'Y' || isMember == 'y') && totalWeight >= MIN_WEIGHT_MEMBER) {
            *discountAmount = *grossTotal * MEMBER_DISCOUNT;
            cout << ">>MEMBER DISCOUNT APPLIED (5%)!" << endl;
        }

        *netTotal = *grossTotal - *discountAmount;

        //step 2 report
        int day, month, year;
        bool validDate = false;
        //get date from user
        while (!validDate) {
            cout << "\nEnter Date (Day Month Year): ";
            cin >> day >> month >> year;
            if ((month >= 1 && month <= 12) && (day >= 1 && day <= 31)) {
                validDate = true;
                if (day <= DAYS_IN_MONTH) {
                    monthlyS[day-1][0] += *netTotal;//total sales
                    monthlyS[day-1][1] += totalWeight;//total weight sold
                }
            } else {
                cout << "Invalid Date. Try again.\n";
            }
        }

        //step 3 print receipt
        saveData(inv, size); // Save the stock deductions
        ofstream outReceipt; // Receipt file
        outReceipt.open("Receipt.txt");

        //display receipt on screen
        cout << "\n==========================================" << endl;
        cout << "              SALES RECEIPT               " << endl;
        cout << "==========================================" << endl;
        cout << "Date: " << day << "/" << month << "/" << year << endl;
        cout << "------------------------------------------" << endl;
        cout << left << setw(20) << "Item" << setw(10) << "Qty(KG)" << setw(10) << "Price" << endl;
        //write receipt to file
        outReceipt << "==========================================" << endl;
        outReceipt << "              SALES RECEIPT               " << endl;
        outReceipt << "==========================================" << endl;
        outReceipt << "Date: " << day << "/" << month << "/" << year << endl;
        outReceipt << "------------------------------------------" << endl;
        outReceipt << left << setw(20) << "Item" << setw(10) << "Qty(KG)" << setw(10) << "Price" << endl;

        //display cart items
        for (int i = 0; i < cartCount; i++) {
            //screen
            cout << left << setw(20) << cart[i].name << setw(10) << cart[i].weight << "RM " << fixed << setprecision(2) << cart[i].subtotal << endl;
            
            //file
            outReceipt << left << setw(20) << cart[i].name << setw(10) << cart[i].weight << "RM " << fixed << setprecision(2) << cart[i].subtotal << endl;

        }
        //display on screen
        cout << "------------------------------------------" << endl;
        cout << left << setw(20) << "GROSS TOTAL:" << "RM " << *grossTotal << endl;
        cout << left << setw(20) << "DISCOUNT:" << "-RM " << *discountAmount << endl;
        cout << "------------------------------------------" << endl;
        cout << left << setw(20) << "NET TOTAL:" << "RM " << *netTotal << endl;
        cout << "==========================================" << endl;
        cout << "        Thank You Come Again!             " << endl;
        //write to file
        outReceipt << "------------------------------------------" << endl;
        outReceipt << left << setw(20) << "GROSS TOTAL:" << "RM " << *grossTotal << endl;
        outReceipt << left << setw(20) << "DISCOUNT:" << "-RM " << *discountAmount << endl;
        outReceipt << "------------------------------------------" << endl;
        outReceipt << left << setw(20) << "NET TOTAL:" << "RM " << *netTotal << endl;
        outReceipt << "==========================================" << endl;
        outReceipt << "        Thank You Come Again!             " << endl;

        outReceipt.close();
        cout << "\n>> Receipt saved to 'Receipt.txt'" << endl;
    } 
    else {
        cout << ">> Cart is empty. Transaction cancelled.\n";
    }

    //free the memory
    delete grossTotal;
    delete discountAmount;
    delete netTotal;
}

// Function to write report
void generateReports(double monthlyS[][TOTAL]) 
{
    string monthName;//to store month name
    cout << "\n=============================================" << endl;
    cout << "          MONTHLY REPORT GENERATOR           " << endl;
    cout << "=============================================" << endl;
    cout << "Enter Month Name (Exp: January): ";
    cin >> monthName;
    //create file name and report title
    string fileName = "Monthly_Report_" + monthName + ".txt";
    string reportTitle = "PERFORMANCE REPORT: " + monthName;
    //open file output
    ofstream out;
    out.open(fileName);
    //declare variables for calculations
    double totalSales = 0;
    double totalWeight = 0;
    double maxSale = 0; 
    double minSale = 999999.0;//bacause we want to compare for lowest number
    int countActiveDays = 0;// to count days with sales
    int bestDay = 0, worstDay = 0;
    //show output
    cout << "\n--- " << reportTitle << " ---" << endl;
    out << "--- " << reportTitle << " ---" << endl;
    cout << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << setw(15) << "Weight(KG)" << endl;
    out << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << setw(15) << "Weight(KG)" << endl;
    cout << "----------------------------------------" << endl;
    out << "----------------------------------------" << endl;
    //to check each day sales
    for (int i = 0; i < DAYS_IN_MONTH; i++) 
    {
        // Only process days that had sales > 0
        if (monthlyS[i][0] > 0) 
        {
            cout << "Day " << left << setw(6) << (i + 1) << "RM " << setw(12) << monthlyS[i][0] << setw(10) << monthlyS[i][1] << endl;
            out << "Day " << left << setw(6) << (i + 1) << "RM " << setw(12) << monthlyS[i][0] << setw(10) << monthlyS[i][1] << endl;
            //find total
            totalSales += monthlyS[i][0];
            totalWeight += monthlyS[i][1];
            countActiveDays++;
            //find max
            if (monthlyS[i][0] > maxSale) {
                maxSale = monthlyS[i][0];
                bestDay = i + 1;
            }
            //find min
            if (monthlyS[i][0] < minSale) {
                minSale = monthlyS[i][0];
                worstDay = i + 1;
            }
        }
    }
    //calculate average sales
    double averageSales = 0.0;
    if (countActiveDays > 0) {
        averageSales = totalSales / countActiveDays;
    } else {
        minSale = 0;//assign min sale to 0 if no sales
    }
    //output totals
    cout << "----------------------------------------" << endl;
    out << "----------------------------------------" << endl;
    //screen
    cout << "TOTAL SALES      : RM " << fixed << setprecision(2) << totalSales << endl;
    cout << "TOTAL WEIGHT     : " << totalWeight << " KG" << endl;
    cout << "AVERAGE SALES    : RM " << averageSales << " (per active day)" << endl;
    cout << "HIGHEST SALE     : RM " << maxSale << " (Day " << bestDay << ")" << endl;
    cout << "LOWEST SALE      : RM " << minSale << " (Day " << worstDay << ")" << endl;
    //file
    out << "TOTAL SALES      : RM " << fixed << setprecision(2) << totalSales << endl;
    out << "TOTAL WEIGHT     : " << totalWeight << " KG" << endl;
    out << "AVERAGE SALES    : RM " << averageSales << " (per active day)" << endl;
    out << "HIGHEST SALE     : RM " << maxSale << " (Day " << bestDay << ")" << endl;
    out << "LOWEST SALE      : RM " << minSale << " (Day " << worstDay << ")" << endl;

    out.close();
    cout << "\n>> Report generated successfully: " << fileName << endl;
}

//Function to edit fruit details
void editFruit(Item inv[], int size) 
{
    string name;
    cout << "\n===================================" << endl;
    cout << "           EDIT FRUIT              " << endl;
    cout << "===================================" << endl;
    cout << "Enter Name of Fruit to Edit: ";
    getline(cin, name);

    //search
    int index = -1;//give index value -1 if not found
    for (int i = 0; i < size; i++) {
        if (inv[i].name == name) 
            index = i;
    }
    if (index == -1) 
        cout << ">> Error: Fruit not found in inventory.\n";
    else
    {
        //show current details so user knows what they are editing
        cout << "\n--- Current Details for " << inv[index].name << " ---" << endl;
        cout << "1. Price  : RM " << inv[index].price << endl;
        cout << "2. Weight : " << inv[index].weight << " KG" << endl;
        cout << "3. Type   : " << inv[index].type << endl;
        cout << "4. Cancel Edit" << endl;

        int editChoice;
        cout << "\nSelect field to edit (1-4): ";
        cin >> editChoice;
        cin.ignore();

        if (editChoice == 1) {
            cout << "Enter New Price: ";
            cin >> inv[index].price;
            cout << ">> Price updated successfully.\n";
        } else if (editChoice == 2) {
            cout << "Enter New Weight: ";
            cin >> inv[index].weight;
            cout << ">> Weight updated successfully.\n";
        } else if (editChoice == 3) {
            cout << "Enter New Type (Local/Imported): ";
            getline(cin, inv[index].type);
            cout << ">> Type updated successfully.\n";
        } else if (editChoice == 4) {
            cout << ">> Edit Cancelled.\n";
            return;
        } else {
            cout << ">> Invalid choice.\n";
            return;
        }
        //save the file
        ofstream outdata("Inventory.txt");
        for (int i = 0; i < size; i++) {
            outdata << inv[i].name << ";" ;
            outdata << fixed << setprecision(2) << inv[i].price << ";";
            outdata << inv[i].weight << ";"; 
            outdata << inv[i].type << ";";
        }
        outdata.close(); 
        cout << ">> File updated.\n";
    }
}