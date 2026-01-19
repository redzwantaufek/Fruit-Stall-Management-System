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
#include <string>
#include <cstring>

using namespace std;

//declare struct for fruit item
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
//struct for cart item
struct CartItem {
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
void restockItem(Item [], int);
void checkExpiry(Item [], int);
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
    in.open("inventory.txt");
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
        cout << "6. Restock Item" << endl;
        cout << "7. Check Expiring Items" << endl;
        cout << "8. Management Reports" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter Menu (1-9): ";
        cin >> choice;
        cin.ignore(); 
  
        if (choice == 1)//display inventory
            displayInventory(inventory, fruitCount);
        else if (choice == 2){ //add new fruit
            addFruit(inventory, fruitCount);
            saveData(inventory, fruitCount);
            cout << ">> Auto-Save: File updated successfully.\n";
        }
        else if (choice == 3)//delete fruit
        {
            deleteFruit(inventory, fruitCount);
            saveData(inventory, fruitCount);
        }
        else if (choice == 4)//edit fruit details
        {
            editFruit(inventory, fruitCount);
        }
        else if (choice == 5)//point of sale system
            processSale(inventory, fruitCount, monthlySales);
        else if (choice == 6)
        {
            string searchName;
            cout << "Enter fruit name to restock: ";
            getline(cin, searchName);
            int index = searchFruit(inventory, fruitCount, searchName);
            if (index != -1)
                restockItem(inventory, index); 
            else cout << "Error: Fruit not found!\n";
        }
        else if (choice == 7)//check expiry
            checkExpiry(inventory, fruitCount);
        else if (choice == 8)//generate reports
            generateReports(monthlySales);
        
        else if (choice == 9) //exit program
        {
            saveData(inventory, fruitCount); 
            cout << "System Exiting... Data Saved.\n"; 
        }
        else//invalid input
            cout << "Invalid input. Please enter number between 1-9.!!!!!"<<endl;
            
    } while (choice != 9);

    in.close();
}

//get data from file
void getData(Item inv[], int &size, ifstream &indata)
{

        size = 0; //to make size 0 everytime function is called
        
        // read file until loop fails
        while (getline(indata, inv[size].name, ';')) 
        {
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
}

//function to save data to file
void saveData(Item inv[], int size)
{
    ofstream outdata("inventory.txt");//outpu file stream 
    for (int i = 0; i < size; i++) 
    {
        //write data to file with ; delimiter
        outdata << inv[i].name << ";" ;
        outdata << fixed << setprecision(2) << inv[i].price << ";";
        outdata << inv[i].weight << ";"; 
        outdata << inv[i].type << ";";
        outdata << inv[i].expDay << ";"; 
        outdata << inv[i].expMonth << ";"; 
        outdata << inv[i].expYear << endl;
    }
    outdata.close(); 
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

// Function point of sale (Multi-Item / Shopping Cart Support)
void processSale(Item inv[], int size, double monthlyS[][TOTAL]) 
{
    // Use the global struct we defined at the top
    CartItem cart[50];  // Array to hold up to 50 items
    int cartCount = 0;
    
    char addMore = 'Y';
    char isMember;
    double totalWeight = 0; // To check for bulk discount

    // Pointer variables for calculation (Requirement)
    double *grossTotal = new double;
    double *discountAmount = new double;
    double *netTotal = new double;
    
    *grossTotal = 0.0;
    *discountAmount = 0.0;
    *netTotal = 0.0;

    cout << "\n==========================================" << endl;
    cout << "      POINT OF SALE (SHOPPING CART)       " << endl;
    cout << "==========================================" << endl;

    // --- STEP 1: ADD ITEMS TO CART ---
    do {
        string name;
        cout << "\nEnter Fruit Name: "; 
        getline(cin, name); 

        // Search for fruit
        int index = -1;
        for (int i = 0; i < size; i++) {
            if (inv[i].name == name) {
                index = i;
                break;
            }
        }

        if (index == -1) { 
            cout << ">> Error: Item not found." << endl; 
        } 
        else {
            double weightRequest;
            cout << "Stock Available: " << inv[index].weight << " KG" << endl;
            cout << "Enter Weight (KG): ";
            cin >> weightRequest;
            cin.ignore(); // Clear buffer

            if (weightRequest > inv[index].weight) { 
                cout << ">> Error: Insufficient stock!" << endl; 
            } 
            else {
                // Add to Cart Array
                cart[cartCount].index = index;
                cart[cartCount].name = inv[index].name;
                cart[cartCount].weight = weightRequest;
                cart[cartCount].price = inv[index].price;
                cart[cartCount].subtotal = inv[index].price * weightRequest;
                
                // Update running totals
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
        cin.ignore(); // Clear buffer

    } while ((addMore == 'Y' || addMore == 'y') && cartCount < 50);

    // --- STEP 2: CHECKOUT & CALCULATIONS ---
    if (cartCount > 0) {
        cout << "\n------------------------------------------" << endl;
        cout << "Proceeding to Checkout..." << endl;
        
        cout << "Member? (Y/N): "; 
        cin >> isMember;

        // Discount Logic
        if ((isMember == 'Y' || isMember == 'y') && totalWeight >= MIN_WEIGHT_MEMBER) {
            *discountAmount = *grossTotal * MEMBER_DISCOUNT;
            cout << ">> BULK MEMBER DISCOUNT APPLIED (5%)!" << endl;
        }

        *netTotal = *grossTotal - *discountAmount;

        // --- STEP 3: DATE & REPORTING ---
        int day, month, year;
        bool validDate = false;
        
        while (!validDate) {
            cout << "\nEnter Date (Day Month Year): "; // e.g., 10 1 2026
            cin >> day >> month >> year;
            
            // Basic validation
            if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
                validDate = true;
                // Add to Monthly Report (Check bounds)
                if (day <= DAYS_IN_MONTH) {
                    monthlyS[day-1][0] += *netTotal;
                }
            } else {
                cout << "Invalid Date. Try again.\n";
            }
        }

        // --- STEP 4: SAVE INVENTORY, LOG & RECEIPT ---
        saveData(inv, size); // Save the stock deductions

        // 2. Receipt File (Overwrites with latest receipt)
        ofstream receiptFile;
        receiptFile.open("Receipt.txt");

        // --- DISPLAY TO SCREEN ---
        cout << "\n==========================================" << endl;
        cout << "              SALES RECEIPT               " << endl;
        cout << "==========================================" << endl;
        cout << "Date: " << day << "/" << month << "/" << year << endl;
        cout << "------------------------------------------" << endl;
        cout << left << setw(20) << "Item" << setw(10) << "Qty(KG)" << setw(10) << "Price" << endl;

        // --- WRITE TO RECEIPT FILE ---
        receiptFile << "==========================================" << endl;
        receiptFile << "              SALES RECEIPT               " << endl;
        receiptFile << "==========================================" << endl;
        receiptFile << "Date: " << day << "/" << month << "/" << year << endl;
        receiptFile << "------------------------------------------" << endl;
        receiptFile << left << setw(20) << "Item" << setw(10) << "Qty(KG)" << setw(10) << "Price" << endl;

        // Loop through cart items
        for (int i = 0; i < cartCount; i++) {
            // Screen
            cout << left << setw(20) << cart[i].name 
                 << setw(10) << cart[i].weight 
                 << "RM " << fixed << setprecision(2) << cart[i].subtotal << endl;
            
            // Receipt File
            receiptFile << left << setw(20) << cart[i].name 
                        << setw(10) << cart[i].weight 
                        << "RM " << fixed << setprecision(2) << cart[i].subtotal << endl;

        }

        // --- FOOTER SCREEN ---
        cout << "------------------------------------------" << endl;
        cout << left << setw(20) << "GROSS TOTAL:" << "RM " << *grossTotal << endl;
        cout << left << setw(20) << "DISCOUNT:" << "-RM " << *discountAmount << endl;
        cout << "------------------------------------------" << endl;
        cout << left << setw(20) << "NET TOTAL:" << "RM " << *netTotal << endl;
        cout << "==========================================" << endl;
        cout << "        Thank You Come Again!             " << endl;

        // --- FOOTER RECEIPT FILE ---
        receiptFile << "------------------------------------------" << endl;
        receiptFile << left << setw(20) << "GROSS TOTAL:" << "RM " << *grossTotal << endl;
        receiptFile << left << setw(20) << "DISCOUNT:" << "-RM " << *discountAmount << endl;
        receiptFile << "------------------------------------------" << endl;
        receiptFile << left << setw(20) << "NET TOTAL:" << "RM " << *netTotal << endl;
        receiptFile << "==========================================" << endl;
        receiptFile << "        Thank You Come Again!             " << endl;

        receiptFile.close();

        cout << "\n>> Receipt saved to 'Receipt.txt'" << endl;
    } 
    else {
        cout << ">> Cart is empty. Transaction cancelled.\n";
    }

    // Free memory
    delete grossTotal;
    delete discountAmount;
    delete netTotal;
}

//function to restock item
void restockItem(Item inv[], int index) 
{
    double quantity;
    //access data and get the restock quantity
    cout << "\n--- RESTOCK ITEM ---" << endl;
    cout << "Item Name: " << inv[index].name << endl;
    cout << "Current Stock: " << inv[index].weight << " KG" << endl;
    cout << "Enter Quantity to Add (KG): ";
    cin >> quantity;
    // Validate input
    if (quantity > 0) {
        inv[index].weight += quantity;
        cout << ">> Success: Stock updated." << endl;
        cout<<">> New Total: " << inv[index].weight << " KG\n";
    } else {
        cout << ">> Error: Invalid quantity.\n";
    }
}

// Check expiry dates based on user input
void checkExpiry(Item inv[], int size) 
{
    int currDay, currMonth, currYear;

    cout << "\n========================================" << endl;
    cout << "          CHECK EXPIRY STATUS           " << endl;
    cout << "========================================" << endl;
    
    // 1. Get Today's Date
    cout << "Enter Today's Date (Day Month Year): "; 
    cin >> currDay >> currMonth >> currYear;

    cout << "\n--- Status Report (As of " << currDay << "/" << currMonth << "/" << currYear << ") ---\n" << endl;
    cout << left << setw(20) << "Item Name" << setw(20) << "Status" << setw(15) << "Expiry Date" << endl;
    cout << "-------------------------------------------------------" << endl;

    bool found = false;

    for (int i = 0; i < size; i++) {
        string status = "";
        
        // 1. Check EXPIRED (Past Date)
        if (inv[i].expYear < currYear || 
           (inv[i].expYear == currYear && inv[i].expMonth < currMonth) ||
           (inv[i].expYear == currYear && inv[i].expMonth == currMonth && inv[i].expDay < currDay)) 
        {
            status = "EXPIRED!";
        }
        // 2. Check TODAY
        else if (inv[i].expYear == currYear && inv[i].expMonth == currMonth && inv[i].expDay == currDay) 
        {
            status = "Expires TODAY";
        }
        // 3. Check EXPIRING SOON (Next 10 Days)
        else 
        {
            // Calculate the difference in days (Simplified logic)
            // This logic works well for same-month and next-month transitions
            int daysRemaining = 999; // Default high value

            if (inv[i].expYear == currYear) {
                if (inv[i].expMonth == currMonth) {
                    // Same month: just subtract days
                    daysRemaining = inv[i].expDay - currDay;
                }
                else if (inv[i].expMonth == currMonth + 1) {
                    // Next month: Add days remaining in current month + days in next month
                    // Assume 30 days per month for simplicity in this student project
                    int daysLeftInCurrMonth = 30 - currDay;
                    daysRemaining = daysLeftInCurrMonth + inv[i].expDay;
                }
            }
            
            // If the difference is between 1 and 10 days
            if (daysRemaining > 0 && daysRemaining <= 10) {
                status = "Expiring Soon";
            }
        }

        // Only print if we found a status (Expired, Today, or Soon)
        if (status != "") {
            cout << left << setw(20) << inv[i].name 
                 << setw(20) << status 
                 << inv[i].expDay << "/" << inv[i].expMonth << "/" << inv[i].expYear << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No items are expired or expiring soon." << endl;
    }
    cout << "-------------------------------------------------------" << endl;
}

// Function to generate Monthly Report with specific Month Name
void generateReports(double monthlyS[][TOTAL]) 
{
    string monthName;
    
    cout << "\n=============================================" << endl;
    cout << "          MONTHLY REPORT GENERATOR           " << endl;
    cout << "=============================================" << endl;
    
    // 1. Ask user for the Month Name to label the report
    cout << "Enter Month Name (e.g. January): ";
    cin >> monthName;

    // Create a specific filename (e.g., Monthly_Report_January.txt)
    string fileName = "Monthly_Report_" + monthName + ".txt";
    string reportTitle = "PERFORMANCE REPORT: " + monthName;

    ofstream outFile;
    outFile.open(fileName.c_str());

    // --- VARIABLES FOR STATISTICS ---
    double totalSales = 0;
    double maxSale = -1.0; 
    double minSale = 999999.0;
    int countActiveDays = 0;
    int bestDay = 0, worstDay = 0;

    // --- HEADER OUTPUT (Screen & File) ---
    cout << "\n--- " << reportTitle << " ---" << endl;
    outFile << "--- " << reportTitle << " ---" << endl;

    cout << left << setw(10) << "Day" << setw(15) << "Sales(RM)"<< endl;
    outFile << left << setw(10) << "Day" << setw(15) << "Sales(RM)"<< endl;

    cout << "----------------------------------------" << endl;
    outFile << "----------------------------------------" << endl;

    // --- LOOP THROUGH ALL DAYS (0 to 30) ---
    for (int i = 0; i < DAYS_IN_MONTH; i++) 
    {
        // Only process days that had sales > 0
        if (monthlyS[i][0] > 0) 
        {
            // 1. Print Row
            cout << "Day " << left << setw(6) << (i + 1) 
                 << "RM " << setw(12) << monthlyS[i][0] 
                 << "RM " << monthlyS[i][1] << endl;

            outFile << "Day " << left << setw(6) << (i + 1) 
                    << "RM " << setw(12) << monthlyS[i][0] 
                    << "RM " << monthlyS[i][1] << endl;

            // 2. Accumulate Totals
            totalSales += monthlyS[i][0];
            countActiveDays++;

            // 3. Find MAX (Highest Sale)
            if (monthlyS[i][0] > maxSale) {
                maxSale = monthlyS[i][0];
                bestDay = i + 1;
            }

            // 4. Find MIN (Lowest Sale)
            if (monthlyS[i][0] < minSale) {
                minSale = monthlyS[i][0];
                worstDay = i + 1;
            }
        }
    }

    // --- CALCULATE AVERAGE ---
    double averageSales = 0.0;
    if (countActiveDays > 0) {
        averageSales = totalSales / countActiveDays;
    } else {
        minSale = 0; // If no sales, min is 0
    }

    // --- FOOTER & TOTAL OUTPUT ---
    cout << "----------------------------------------" << endl;
    outFile << "----------------------------------------" << endl;

    // SCREEN OUTPUT
    cout << "TOTAL SALES      : RM " << fixed << setprecision(2) << totalSales << endl;
    cout << "AVERAGE SALES    : RM " << averageSales << " (per active day)" << endl;
    cout << "HIGHEST SALE     : RM " << maxSale << " (Day " << bestDay << ")" << endl;
    cout << "LOWEST SALE      : RM " << minSale << " (Day " << worstDay << ")" << endl;

    // FILE OUTPUT
    outFile << "TOTAL SALES      : RM " << fixed << setprecision(2) << totalSales << endl;
    outFile << "AVERAGE SALES    : RM " << averageSales << " (per active day)" << endl;
    outFile << "HIGHEST SALE     : RM " << maxSale << " (Day " << bestDay << ")" << endl;
    outFile << "LOWEST SALE      : RM " << minSale << " (Day " << worstDay << ")" << endl;

    outFile.close();
    cout << "\n>> Report generated successfully: " << fileName << endl;
}

// Function to edit existing fruit details
void editFruit(Item inv[], int size) 
{
    string name;
    cout << "\n===================================" << endl;
    cout << "           EDIT FRUIT              " << endl;
    cout << "===================================" << endl;
    cout << "Enter Name of Fruit to Edit: ";
    getline(cin, name);

    // Reuse your existing search function
    int index = searchFruit(inv, size, name);

    if (index == -1) 
    {
        cout << ">> Error: Fruit not found in inventory.\n";
        return;
    }

    // Show current details so user knows what they are editing
    cout << "\n--- Current Details for " << inv[index].name << " ---" << endl;
    cout << "1. Price  : RM " << inv[index].price << endl;
    cout << "2. Weight : " << inv[index].weight << " KG" << endl;
    cout << "3. Type   : " << inv[index].type << endl;
    cout << "4. Expiry : " << inv[index].expDay << "/" << inv[index].expMonth << "/" << inv[index].expYear << endl;
    cout << "5. Cancel Edit" << endl;
    
    int editChoice;
    cout << "\nSelect field to edit (1-5): ";
    cin >> editChoice;
    cin.ignore(); // Clear buffer

    if (editChoice == 1) {
        cout << "Enter New Price: ";
        cin >> inv[index].price;
        cout << ">> Price updated successfully.\n";
    } else if (editChoice == 2) {
        cout << "Enter New Weight: ";
        cin >> inv[index].weight;
        cout << ">> Weight updated successfully.\n";
    } else if (editChoice == 3) {
        cout << "Enter New Type: ";
        getline(cin, inv[index].type);
        cout << ">> Type updated successfully.\n";
    } else if (editChoice == 4) {
        cout << "Enter New Expiry Day: "; cin >> inv[index].expDay;
        cout << "Enter New Expiry Month: "; cin >> inv[index].expMonth;
        cout << "Enter New Expiry Year: "; cin >> inv[index].expYear;
        cout << ">> Expiry date updated successfully.\n";
    } else if (editChoice == 5) {
        cout << ">> Edit Cancelled.\n";
        return;
    } else {
        cout << ">> Invalid choice.\n";
        return;
    }

    // Auto-save the changes to the file immediately
    saveData(inv, size);
    cout << ">> File updated.\n";
}