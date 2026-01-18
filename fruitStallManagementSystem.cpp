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

// Declared here globally so it looks like standard student code
struct CartItem {
    int index;      // To remember which fruit in inventory we are buying
    string name;
    double weight;
    double price;
    double subtotal;
};

//declare constant variables
const int MAX_FRUITS = 500;
const int DAYS_IN_MONTH = 31;
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
void restockItem(Item [], int);
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
        cout << "\n============================================="<<endl;
        cout << "Welcome to the Fruit Stall Management System!" << endl;
        cout << "=============================================" << endl;
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
            getline(cin, searchName);
            int index = searchFruit(inventory, fruitCount, searchName);
            if (index != -1)
                restockItem(inventory, index); 
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
        outdata << fixed << setprecision(2) << inv[i].price << ";";
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

// Function point of sale (Multi-Item / Shopping Cart Support)
void processSale(Item inv[], int size, double monthlyS[][STATS]) 
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
        double profit = *netTotal * 0.40; // 40% Margin

        // --- STEP 3: DATE & REPORTING ---
        int day, month, year;
        bool validDate = false;
        
        while (!validDate) {
            cout << "\nEnter Date (EXP: 10 1 2003): ";
            cin >> day >> month >> year;
            
            // Basic validation
            if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
                validDate = true;
                // Add to Monthly Report (Check bounds)
                if (day <= DAYS_IN_MONTH) {
                    monthlyS[day-1][0] += *netTotal;
                    monthlyS[day-1][1] += profit;
                }
            } else {
                cout << "Invalid Date. Try again.\n";
            }
        }

        // --- STEP 4: SAVE INVENTORY & LOG ---
        saveData(inv, size); // Save the stock deductions

        // Log transaction (Append to file)
        ofstream logFile;
        logFile.open("Transaction_Log.txt", ios::app);
        
        cout << "\n==========================================" << endl;
        cout << "              SALES RECEIPT               " << endl;
        cout << "==========================================" << endl;
        cout << "Date: " << day << "/" << month << "/" << year << endl;
        cout << "------------------------------------------" << endl;
        cout << left << setw(20) << "Item" << setw(10) << "Qty(KG)" << setw(10) << "Price" << endl;
        
        for (int i = 0; i < cartCount; i++) {
            // Print to Screen
            cout << left << setw(20) << cart[i].name 
                 << setw(10) << cart[i].weight 
                 << "RM " << fixed << setprecision(2) << cart[i].subtotal << endl;
            
            // Log to File
            logFile << day << "/" << month << "/" << year 
                    << " | " << cart[i].name 
                    << " | " << cart[i].weight << "kg | RM " << cart[i].subtotal << endl;
        }

        cout << "------------------------------------------" << endl;
        cout << left << setw(20) << "GROSS TOTAL:" << "RM " << *grossTotal << endl;
        cout << left << setw(20) << "DISCOUNT:" << "-RM " << *discountAmount << endl;
        cout << "------------------------------------------" << endl;
        cout << left << setw(20) << "NET TOTAL:" << "RM " << *netTotal << endl;
        cout << "==========================================" << endl;
        cout << "        Thank You Come Again!             " << endl;

        logFile.close();
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

// Function to generate advanced reports with Avg, Max, Min
void generateReports(double monthlyS[][STATS]) 
{
    int reportChoice;
    cout << "\n=============================================" << endl;
    cout << "          MANAGEMENT REPORT CENTER           " << endl;
    cout << "=============================================" << endl;
    cout << "1. Weekly Report (Specific Week)" << endl;
    cout << "2. Monthly Report (Full Month Overview)" << endl;
    cout << "Select Option: ";
    cin >> reportChoice;

    int startDay = 0, endDay = 0;
    string reportTitle, fileName;

    // --- SETUP RANGE BASED ON CHOICE ---
    if (reportChoice == 1) 
    {
        int week;
        cout << "Enter Week Number (1-4): ";
        cin >> week;
        if (week < 1 || week > 4) { cout << "Invalid Week.\n"; return; }

        startDay = (week - 1) * 7;
        endDay = startDay + 7;
        
        // Dynamic Filename
        if (week == 1) fileName = "Weekly_Report_Week_1.txt";
        else if (week == 2) fileName = "Weekly_Report_Week_2.txt";
        else if (week == 3) fileName = "Weekly_Report_Week_3.txt";
        else if (week == 4) fileName = "Weekly_Report_Week_4.txt";
        
        reportTitle = "WEEKLY REPORT (Week " + to_string(week) + ")";
    }
    else if (reportChoice == 2) 
    {
        startDay = 0;
        endDay = DAYS_IN_MONTH; // Full 30/31 days
        fileName = "Monthly_Report_Full.txt";
        reportTitle = "MONTHLY PERFORMANCE REPORT";
    }
    else 
    {
        cout << "Invalid Selection.\n"; 
        return;
    }

    ofstream outFile;
    outFile.open(fileName.c_str());

    // --- VARIABLES FOR STATISTICS ---
    double totalSales = 0, totalProfit = 0;
    double maxSale = -1.0;  // Start low
    double minSale = 999999.0; // Start high
    int countActiveDays = 0;
    int bestDay = 0, worstDay = 0;

    // --- HEADER OUTPUT (Screen & File) ---
    cout << "\n--- " << reportTitle << " ---" << endl;
    outFile << "--- " << reportTitle << " ---" << endl;

    cout << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << setw(15) << "Profit(RM)" << endl;
    outFile << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << setw(15) << "Profit(RM)" << endl;

    cout << "----------------------------------------" << endl;
    outFile << "----------------------------------------" << endl;

    // --- LOOP & CALCULATE ---
    for (int i = startDay; i < endDay && i < DAYS_IN_MONTH; i++) 
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
            totalProfit += monthlyS[i][1];
            countActiveDays++;

            // 3. Find MAX
            if (monthlyS[i][0] > maxSale) {
                maxSale = monthlyS[i][0];
                bestDay = i + 1;
            }

            // 4. Find MIN
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

    // --- FOOTER & STATS OUTPUT ---
    cout << "----------------------------------------" << endl;
    outFile << "----------------------------------------" << endl;

    // Helper Lambda or Macro isn't allowed in basic C++, so we just repeat code blocks for File & Screen
    // SCREEN OUTPUT
    cout << "TOTAL SALES      : RM " << fixed << setprecision(2) << totalSales << endl;
    cout << "TOTAL PROFIT     : RM " << totalProfit << endl;
    cout << "AVERAGE SALES    : RM " << averageSales << " (per active day)" << endl;
    cout << "HIGHEST SALE     : RM " << maxSale << " (Day " << bestDay << ")" << endl;
    cout << "LOWEST SALE      : RM " << minSale << " (Day " << worstDay << ")" << endl;

    // FILE OUTPUT
    outFile << "TOTAL SALES      : RM " << fixed << setprecision(2) << totalSales << endl;
    outFile << "TOTAL PROFIT     : RM " << totalProfit << endl;
    outFile << "AVERAGE SALES    : RM " << averageSales << " (per active day)" << endl;
    outFile << "HIGHEST SALE     : RM " << maxSale << " (Day " << bestDay << ")" << endl;
    outFile << "LOWEST SALE      : RM " << minSale << " (Day " << worstDay << ")" << endl;

    outFile << "\n[NOTE]: Detailed list of specific items sold is available in 'Transaction_Log.txt'" << endl;

    outFile.close();
    cout << "\n>> Report generated successfully: " << fileName << endl;
}