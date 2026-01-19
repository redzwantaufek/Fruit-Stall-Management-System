/*
 * Project: Abah Fruit Hub Management System (Ultimate Version)
 * Course: CSC404 (Oct 2025 - Feb 2026)
 * Features: Delimiter Parsing (;), Spaces in Names, Weight Logic, Reports
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

// [REQ: Struct]
struct Fruit {
    string name;   
    double price;  
    double weight; 
    string type;   
    
    // Expiry Date
    int expDay;
    int expMonth;
    int expYear;
};

// [CONFIG: Capacity]
// 500 is large enough to feel "unlimited" for a student project.
const int MAX_FRUITS = 500; 
const int DAYS_IN_MONTH = 30; 
const int STATS = 2; // Col 0: Sales, Col 1: Profit
const double MEMBER_DISCOUNT = 0.05; 
const double BULK_THRESHOLD_KG = 3.0; 

// Function Prototypes
void loadData(Fruit inventory[], int &count);
void saveData(Fruit inventory[], int count);
void displayInventory(Fruit inventory[], int count);
void addFruit(Fruit inventory[], int &count);    
void deleteFruit(Fruit inventory[], int &count); 
void processSale(Fruit inventory[], int count, double monthlySales[DAYS_IN_MONTH][STATS]); 
void restockItem(Fruit *fptr); 
int searchFruit(Fruit inventory[], int count, string searchName);
void checkExpiry(Fruit inventory[], int count);
void generateReports(double monthlySales[DAYS_IN_MONTH][STATS]);
void printWeeklyReport(double monthlySales[DAYS_IN_MONTH][STATS], int weekNum);
void printMonthlyReport(double monthlySales[DAYS_IN_MONTH][STATS]);

int main() {
    Fruit inventory[MAX_FRUITS];
    double monthlySales[DAYS_IN_MONTH][STATS] = {0}; 
    int fruitCount = 0;
    int choice;

    // Load data at startup
    loadData(inventory, fruitCount);

    do {
        cout << "\n=========================================\n";
        cout << "   ABAH FRUIT HUB - SYSTEM DASHBOARD     \n";
        cout << "=========================================\n";
        cout << "1. View Inventory (kg)\n";
        cout << "2. Add New Fruit (Support Spaces)\n";
        cout << "3. Delete Fruit\n";
        cout << "4. Point of Sale (POS System)\n";
        cout << "5. Restock Item\n";
        cout << "6. Check Expiring Items\n";
        cout << "7. Management Reports\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        // Clear buffer after integer input to prevent getline skipping
        cin.ignore(1000, '\n'); 

        switch(choice) {
            case 1: displayInventory(inventory, fruitCount); break;
            case 2: addFruit(inventory, fruitCount); break;
            case 3: deleteFruit(inventory, fruitCount); break;
            case 4: processSale(inventory, fruitCount, monthlySales); break;
            case 5: {
                string searchName;
                cout << "Enter fruit name to restock: ";
                getline(cin, searchName); // Allow spaces in search
                int index = searchFruit(inventory, fruitCount, searchName);
                if (index != -1) restockItem(&inventory[index]); 
                else cout << "Error: Fruit not found!\n";
                break;
            }
            case 6: checkExpiry(inventory, fruitCount); break;
            case 7: generateReports(monthlySales); break; 
            case 8: 
                saveData(inventory, fruitCount); 
                cout << "System Exiting... Data Saved.\n"; 
                break;
            default: cout << "Invalid input.\n";
        }
    } while (choice != 8);

    return 0;
}

// [CRUD: Create with SPACES]
void addFruit(Fruit inventory[], int &count) {
    if (count >= MAX_FRUITS) {
        cout << "Error: Inventory Full (Limit 500)!\n";
        return;
    }
    cout << "\n--- ADD NEW FRUIT ---\n";
    
    cout << "Name (e.g. Green Apple): "; 
    getline(cin, inventory[count].name); // [FIX: getline for spaces]

    cout << "Price per KG: "; 
    cin >> inventory[count].price;

    cout << "Weight (KG): "; 
    cin >> inventory[count].weight;

    cout << "Type (Local/Imported): "; 
    cin >> inventory[count].type; // Type usually one word, cin is fine.

    cout << "Expiry Day: "; cin >> inventory[count].expDay;
    cout << "Expiry Month: "; cin >> inventory[count].expMonth;
    cout << "Expiry Year: "; cin >> inventory[count].expYear;
    
    count++; 
    saveData(inventory, count); 
    cout << ">> Success: Fruit Added & Saved!\n";
}

void deleteFruit(Fruit inventory[], int &count) {
    string name;
    cout << "\n--- DELETE FRUIT ---\n";
    cout << "Enter Name: "; 
    getline(cin, name); // [FIX: getline for spaces]

    int index = searchFruit(inventory, count, name);
    if (index == -1) { cout << "Not found.\n"; return; }

    char confirm;
    cout << "Delete " << inventory[index].name << "? (Y/N): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        for (int i = index; i < count - 1; i++) {
            inventory[i] = inventory[i + 1];
        }
        count--; 
        saveData(inventory, count);
        cout << ">> Item Deleted.\n";
    }
}

// [FILE I/O: Delimiter Parsing]
void loadData(Fruit inventory[], int &count) {
    ifstream inFile("inventory.txt");
    if (!inFile) return;
    
    count = 0;
    string tempStr;

    // Logic: Read Name until ';', then read numbers ignoring ';'
    while (getline(inFile, inventory[count].name, ';')) {
        
        inFile >> inventory[count].price; 
        inFile.ignore(1, ';'); // Skip the ;
        
        inFile >> inventory[count].weight; 
        inFile.ignore(1, ';'); // Skip the ;
        
        getline(inFile, inventory[count].type, ';'); // Read type string
        
        inFile >> inventory[count].expDay; 
        inFile.ignore(1, ';');
        
        inFile >> inventory[count].expMonth; 
        inFile.ignore(1, ';');
        
        inFile >> inventory[count].expYear; 
        inFile.ignore(100, '\n'); // Skip newline at end of row
        
        count++;
    }
    inFile.close();
}

void saveData(Fruit inventory[], int count) {
    ofstream outFile("inventory.txt");
    for (int i = 0; i < count; i++) {
        // [FIX: Write with ; delimiters]
        outFile << inventory[i].name << ";" 
                << inventory[i].price << ";" 
                << inventory[i].weight << ";" 
                << inventory[i].type << ";"
                << inventory[i].expDay << ";" 
                << inventory[i].expMonth << ";" 
                << inventory[i].expYear << endl;
    }
    outFile.close();
}

void displayInventory(Fruit inventory[], int count) {
    cout << "\n--- INVENTORY LIST ---\n";
    cout << left << setw(20) << "Name" << setw(10) << "Price/KG" 
         << setw(10) << "Stock(KG)" << setw(10) << "Type" << setw(12) << "Expiry" << endl;
    cout << "------------------------------------------------------------------\n";
    for (int i = 0; i < count; i++) {
        cout << left << setw(20) << inventory[i].name 
             << "RM" << fixed << setprecision(2) << setw(8) << inventory[i].price 
             << setw(10) << inventory[i].weight << setw(10) << inventory[i].type 
             << inventory[i].expDay << "/" << inventory[i].expMonth 
             << "/" << inventory[i].expYear << endl;
    }
}

void processSale(Fruit inventory[], int count, double monthlySales[DAYS_IN_MONTH][STATS]) {
    string name;
    double weightReq;
    int day;
    char isMember;

    cout << "\n================================\n";
    cout << "      POINT OF SALE (POS)       \n";
    cout << "================================\n";
    cout << "Enter Fruit: "; 
    getline(cin, name); // [FIX: getline]
    
    int index = searchFruit(inventory, count, name);
    if (index == -1) { cout << ">> Error: Item not found.\n"; return; }

    cout << "Stock Available: " << inventory[index].weight << " KG\n";
    cout << "Enter Weight needed (KG): ";
    cin >> weightReq;

    if (weightReq > inventory[index].weight) { 
        cout << ">> Error: Insufficient stock!\n"; 
        return; 
    }

    // Pointer variables for calculation
    double *grossTotal = new double;
    double *discountAmt = new double;
    double *netTotal = new double;

    *grossTotal = inventory[index].price * weightReq;
    *discountAmt = 0.0;

    cout << "Member? (Y/N): "; cin >> isMember;

    if ((isMember == 'Y' || isMember == 'y') && weightReq >= BULK_THRESHOLD_KG) {
        *discountAmt = *grossTotal * MEMBER_DISCOUNT;
        cout << ">> BULK MEMBER DISCOUNT APPLIED!\n";
    }

    *netTotal = *grossTotal - *discountAmt;
    double profit = *netTotal * 0.40; 
    
    inventory[index].weight -= weightReq;
    saveData(inventory, count); // Auto-Update

    cout << "Day of Month (1-30): "; cin >> day;
    if (day >= 1 && day <= 30) {
        monthlySales[day-1][0] += *netTotal;
        monthlySales[day-1][1] += profit;
    }

    cout << "\n--------------------------------\n";
    cout << "        SALES RECEIPT           \n";
    cout << "--------------------------------\n";
    cout << left << setw(20) << "Item:" << inventory[index].name << endl;
    cout << left << setw(20) << "Price/KG:" << "RM " << inventory[index].price << endl;
    cout << left << setw(20) << "Weight Sold:" << weightReq << " KG" << endl;
    cout << "--------------------------------\n";
    cout << left << setw(20) << "Gross Total:" << "RM " << *grossTotal << endl;
    cout << left << setw(20) << "Discount (-):" << "RM " << *discountAmt << endl;
    cout << "--------------------------------\n";
    cout << left << setw(20) << "NET TO PAY:" << "RM " << *netTotal << endl;
    cout << "--------------------------------\n";
    cout << "    Thank You Come Again!       \n";
    cout << "--------------------------------\n";

    delete grossTotal;
    delete discountAmt;
    delete netTotal;
}

void restockItem(Fruit *fptr) {
    double qty;
    cout << "Current: " << fptr->weight << " KG. Add (KG): ";
    cin >> qty;
    fptr->weight += qty;
    cout << "Updated Stock: " << fptr->weight << " KG\n";
}

int searchFruit(Fruit inventory[], int count, string searchName) {
    for (int i = 0; i < count; i++) {
        // [FIX: Compare strings directly]
        if (inventory[i].name == searchName) 
            return i;
    }
    return -1;
}

void checkExpiry(Fruit inventory[], int count) {
    int currMonth = 1, currYear = 2026;
    cout << "\n--- EXPIRY ALERT (Jan 2026) ---\n";
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (inventory[i].expYear < currYear || 
           (inventory[i].expYear == currYear && inventory[i].expMonth <= currMonth)) {
            cout << "ALERT: " << inventory[i].name << " expired on " 
                 << inventory[i].expDay << "/" << inventory[i].expMonth << "/" 
                 << inventory[i].expYear << endl;
            found = true;
        }
    }
    if (!found) cout << "No expired items found.\n";
}

void generateReports(double monthlySales[DAYS_IN_MONTH][STATS]) {
    int rptChoice;
    cout << "\n--- MANAGEMENT REPORT CENTER ---\n";
    cout << "1. Weekly Report (Specific Week)\n";
    cout << "2. Monthly Report (Full Overview)\n";
    cout << "Select Option: ";
    cin >> rptChoice;

    if (rptChoice == 1) {
        int week;
        cout << "Enter Week Number (1-4): ";
        cin >> week;
        if (week >= 1 && week <= 4) {
            printWeeklyReport(monthlySales, week);
        } else {
            cout << "Invalid Week.\n";
        }
    } else if (rptChoice == 2) {
        printMonthlyReport(monthlySales);
    } else {
        cout << "Invalid Selection.\n";
    }
}

void printWeeklyReport(double monthlySales[DAYS_IN_MONTH][STATS], int weekNum) {
    int startDay = (weekNum - 1) * 7;
    int endDay = startDay + 7;
    double total = 0, totalProfit = 0;

    cout << "\n--- WEEKLY REPORT (Week " << weekNum << ") ---\n";
    cout << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << setw(15) << "Profit(RM)" << endl;
    cout << "----------------------------------------\n";

    for (int i = startDay; i < endDay && i < DAYS_IN_MONTH; i++) {
        if (monthlySales[i][0] > 0) { 
            cout << "Day " << left << setw(6) << (i + 1) 
                 << "RM " << setw(12) << monthlySales[i][0] 
                 << "RM " << monthlySales[i][1] << endl;
            total += monthlySales[i][0];
            totalProfit += monthlySales[i][1];
        }
    }
    cout << "----------------------------------------\n";
    cout << "WEEK TOTAL SALES : RM " << total << endl;
    cout << "WEEK NET PROFIT  : RM " << totalProfit << endl;
}

void printMonthlyReport(double monthlySales[DAYS_IN_MONTH][STATS]) {
    double total = 0, maxS = -1;
    int hDay = 0;

    cout << "\n--- MONTHLY PERFORMANCE REPORT ---\n";
    cout << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << endl;
    
    for (int i = 0; i < DAYS_IN_MONTH; i++) {
        if (monthlySales[i][0] > 0) {
            cout << "Day " << left << setw(6) << (i + 1) 
                 << "RM " << monthlySales[i][0] << endl;
            total += monthlySales[i][0];
            if (monthlySales[i][0] > maxS) { maxS = monthlySales[i][0]; hDay = i + 1; }
        }
    }
    
    cout << "----------------------------------\n";
    cout << "TOTAL MONTH REVENUE : RM " << total << endl;
    cout << "BEST PERFORMING DAY : Day " << hDay << " (RM " << maxS << ")\n";
}