/*
Title: Fruit Stall Management System
CSC404 M3CS2302A
Members:
1. Muhammad Redzwan Bin Md Taufek (2025152219)
2. Muhammad Naufal bin Abdul Rahim (2025147243)
3. Muhammad Izzat Azamuddin bin Azman (2025127367)
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <windows.h>

using namespace std;

//declare struct
struct Item {
    string name;   
    double price;  
    double weight; 
    string type;
};
struct Checkout {
    string name;
    double weight;
    double price;
    double subtotal;
};

//function prototypes
void getData(Item [], int &, ifstream &);
void saveData(Item inv[], int);
void addFruit(Item [], int &);
void deleteFruit(Item [], int &);
void displayInventory(Item [], int);
void processSale(Item [], int , double [][2]);
void generateReports(double [][2]);
void editFruit(Item [], int);
void splashScreen();
 
int main()
{
    Item inventory[100];// declare struct array, with max 100 fruits array
    double monthlySales[31][2] = {0};//declare 2D array to store monthly sales
    int fruitCount = 0;//to store number of fruits
    int choice;// to store user choice

    //call the splash screen function
    splashScreen();
    //input file
    ifstream in;
    in.open("inventory.txt");
    //call funct get data
    getData(inventory, fruitCount, in);
    //main menu
    
    do {
        cout << "\n============================================="<<endl;
        cout << "Welcome to the Fruit Stall Management System" << endl;
        cout << "=============================================" << endl;
        cout << "1. View Inventory" << endl;
        cout << "2. Add New Fruit" << endl;
        cout << "3. Delete Fruit" << endl;
        cout << "4. Edit Fruit Details" << endl;
        cout << "5. Check Out System" << endl;
        cout << "6. Report" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter Menu (1-7): ";
        cin >> choice;
        cin.ignore();

        // Error handling for invalid input
        if (cin.fail()) {
            cin.clear();  //reset
            cin.ignore(10000, '\n');//ignore input
            cout << "Invalid input. Please enter a number between 1-7!!!" << endl;
            continue;
        }
  
        if (choice == 1)//display inventory
            displayInventory(inventory, fruitCount);
        else if (choice == 2){ //add new fruit
            addFruit(inventory, fruitCount);
            saveData(inventory, fruitCount);
            cout << ">>File updated successfully." << endl;
        }
        else if (choice == 3) {//delete fruit
            deleteFruit(inventory, fruitCount);
            saveData(inventory, fruitCount);
        }
        else if (choice == 4) { //edit fruit details
            editFruit(inventory, fruitCount);
            saveData(inventory, fruitCount);
        }
        else if (choice == 5) { //check out system
            processSale(inventory, fruitCount, monthlySales);
            saveData(inventory, fruitCount);
        }
        else if (choice == 6) //generate report
            generateReports(monthlySales);
        
        else if (choice == 7) { //exit program
            saveData(inventory, fruitCount); 
            cout << "Exit System" << endl;
        }
        else//invalid input
            cout << "Invalid input. Please enter number between 1-7.!!!"<<endl;
            
    } while (choice != 7);

    in.close();
}

//get data from file
void getData(Item inv[], int &fCount, ifstream &indata)
{

        fCount = 0; //to make size 0 everytime function is called
        // read file
        while (getline(indata, inv[fCount].name, ';')) { //read name first, kalau takde nama break the loop, kalau buat eof ada blank space
            indata >> inv[fCount].price; 
            indata.ignore();
            indata >> inv[fCount].weight; 
            indata.ignore();
            getline(indata, inv[fCount].type);
            
            fCount++;
        }
}
//function to save data to file
void saveData(Item inv[], int fCount)
{
    ofstream outdata;
    outdata.open("inventory.txt"); 
    for (int i = 0; i < fCount; i++) {
        outdata << inv[i].name << ";" ;
        outdata << fixed << setprecision(2) << inv[i].price << ";";
        outdata << inv[i].weight << ";"; 
        outdata << inv[i].type << endl;
    }
    outdata.close();
}

//function to add new fruit to file inventory
void addFruit(Item inv[], int &fCount)
{
    //check if inventory full
    if (fCount < 100)
    {
        cout << "\n==========================================" << endl;
        cout << "             ADD NEW FRUIT             " << endl;
        cout << "==========================================" << endl;
        cout << "Name (Exp: Green Apple): "; 
        getline(cin, inv[fCount].name);
        cout << "Price per KG: "; 
        cin >> inv[fCount].price;
        cout << "Weight (KG): "; 
        cin >> inv[fCount].weight;
        cout << "Type (Local/Imported): "; 
        cin >> inv[fCount].type; 
        cout << ">> Fruit Added!!!" << endl;
        fCount++;
    }
    else {
        cout << "Inventory Full (Limit 100)!!!" << endl;
    }
}


//function to delete fruit from inventory
void deleteFruit(Item inv[], int &fCount) 
{
    string name;
    cout << "\n==========================================" << endl;
    cout << "             DELETE FRUIT              " << endl;
    cout << "==========================================" << endl;
    cout << "Enter Fruit Name: "; 
    getline(cin, name);

    //search for fruit index
    bool check = false;
    int index = 0;
    for (int i = 0; i < fCount; i++) {
        if (inv[i].name == name) {
            index = i;
            check = true;
        }
}
    //delete
    if (check == true){
        char confirm;
        cout << "Delete " << inv[index].name << "? (Y/N): ";
        cin >> confirm;

        if (confirm == 'Y' || confirm == 'y') {
            for (int i = index; i < fCount - 1; i++) {
                inv[i] = inv[i + 1];
            }
            fCount--; 
            cout << ">> Item Deleted." << endl;
        } else {
            cout << ">> Deletion Cancelled." << endl;
        }
    } else {
        cout << "Fruit Not found!!!" << endl; 
    }
}

//function to display the inventory
void displayInventory(Item inv[], int fCount) 
{
    cout << "\n------------------------- INVENTORY LIST -------------------------"<<endl;
    cout << left << setw(20) << "Name" << setw(10) << "Price/KG" << setw(10) << "Stock(KG)" << setw(10) << "Type" << endl;
    cout << "------------------------------------------------------------------"<<endl;
    for (int i = 0; i < fCount; i++) {
        cout << left << setw(20) << inv[i].name << "RM" << fixed << setprecision(2) << setw(8) << inv[i].price << setw(10) << inv[i].weight << setw(10) << inv[i].type << endl;
    }
}

// Function checkout system
void processSale(Item inv[], int fCount, double monthlyS[][2]) 
{
    Checkout cart[50]; // Array to hold up to 50 items
    int cartCount = 0;// Number of items in cart
    char addMore = 'Y';//add ore items to cart or not
    char isMember;//member status
    double totalWeight = 0;
    // Pointer variables for calculation
    double *grossTotal = new double;// Pointer for gross total
    double *discountAmount = new double;// Pointer for discount amount
    double *netTotal = new double;// Pointer for net total
    
    *grossTotal = 0.0;
    *discountAmount = 0.0;
    *netTotal = 0.0;

    cout << "\n==========================================" << endl;
    cout << "             Checkout System              " << endl;
    cout << "==========================================" << endl;

    //step 1 add item
    do {
        string name;
        cout << "\nEnter Fruit Name: "; 
        getline(cin, name); 

        //search for fruit index
        bool check = false;
        int index = 0;
        for (int i = 0; i < fCount; i++) {
            if (inv[i].name == name) {
                index = i;
                check = true;
            }
        }

        if (check == true) { //calculate the total price
            double weightRequest;
            cout << "Stock Available: " << inv[index].weight << " KG" << endl;
            cout << "Enter Weight (KG): ";
            cin >> weightRequest;
            cin.ignore();

            if (weightRequest <= inv[index].weight) {
                //cart item details
                cart[cartCount].name = inv[index].name;
                cart[cartCount].weight = weightRequest;
                cart[cartCount].price = inv[index].price;
                cart[cartCount].subtotal = inv[index].price * weightRequest;
                
                //update totals
                *grossTotal += cart[cartCount].subtotal;
                totalWeight += weightRequest;
                
                //reduce stock in memory
                inv[index].weight -= weightRequest; 
                
                cartCount++;
                cout << ">> Item added to cart." << endl;

            } else
                cout << ">>Insufficient stock!" << endl; 

        } 
        else {
            cout << ">>Item not found!!!" << endl; 
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
        if ((isMember == 'Y' || isMember == 'y') && totalWeight >= 3.0) {
            *discountAmount = *grossTotal * 0.05;
            cout << ">>MEMBER DISCOUNT APPLIED (5%)!" << endl;
        }
        //calculate net total
        *netTotal = *grossTotal - *discountAmount;

        int day, month, year;
        bool validDate = false;
        //get date from user
        while (validDate == false) {
            cout << "\nEnter Date (Day Month Year: 10 1 2003): ";
            cin >> day >> month >> year;
            if ((month >= 1 && month <= 12) && (day >= 1 && day <= 31)) {
                validDate = true;
            } else {
                cout << "Invalid Date. Try again." << endl;
            }
        }
        monthlyS[day-1][0] += *netTotal;//total sales
        monthlyS[day-1][1] += totalWeight;//total weight sold

        ofstream outData; // Receipt file
        outData.open("Receipt.txt");

        //display output
        cout << "\n==========================================" << endl;
        outData << "==========================================" << endl;
        cout << "              SALES RECEIPT               " << endl;
        outData << "              SALES RECEIPT               " << endl;
        cout << "==========================================" << endl;
        outData << "==========================================" << endl;
        cout << "Date: " << day << "/" << month << "/" << year << endl;
        outData << "Date: " << day << "/" << month << "/" << year << endl;
        cout << "------------------------------------------" << endl;
        outData << "------------------------------------------" << endl;
        cout << left << setw(20) << "Item" << setw(10) << "Qty(KG)" << setw(10) << "Price" << endl;
        outData << left << setw(20) << "Item" << setw(10) << "Qty(KG)" << setw(10) << "Price" << endl;

        //display cart items
        for (int i = 0; i < cartCount; i++) {
            cout << left << setw(20) << cart[i].name << setw(10) << cart[i].weight << "RM " << fixed << setprecision(2) << cart[i].subtotal << endl; 
            outData << left << setw(20) << cart[i].name << setw(10) << cart[i].weight << "RM " << fixed << setprecision(2) << cart[i].subtotal << endl;
        }

        //display ouput
        cout << "------------------------------------------" << endl;
        outData << "------------------------------------------" << endl;
        cout << left << setw(20) << "GROSS TOTAL:" << "RM " << *grossTotal << endl;
        outData << left << setw(20) << "GROSS TOTAL:" << "RM " << *grossTotal << endl;
        cout << left << setw(20) << "DISCOUNT:" << "-RM " << *discountAmount << endl;
        outData << left << setw(20) << "DISCOUNT:" << "-RM " << *discountAmount << endl;
        cout << "------------------------------------------" << endl;
        outData << "------------------------------------------" << endl;
        cout << left << setw(20) << "NET TOTAL:" << "RM " << *netTotal << endl;
        outData << left << setw(20) << "NET TOTAL:" << "RM " << *netTotal << endl;
        cout << "==========================================" << endl;
        outData << "==========================================" << endl;
        cout << "        Thank You Come Again!             " << endl;
        outData << "        Thank You Come Again!             " << endl;
        cout << "==========================================" << endl;
        outData << "==========================================" << endl;

        outData.close();
    } 
    else {
        cout << ">> Cart is empty. Transaction cancelled." << endl;
    }

    //free the memory
    delete grossTotal;
    delete discountAmount;
    delete netTotal;
}

// Function to write report
void generateReports(double monthlyS[][2]) 
{
    string monthName;//to store month name
    cout << "\n=============================================" << endl;
    cout << "          MONTHLY REPORT GENERATOR           " << endl;
    cout << "=============================================" << endl;
    cout << "Enter Month Name (Exp: January): ";
    cin >> monthName;

    string reportTitle = "PERFORMANCE REPORT: " + monthName;//report title
    //open file output
    ofstream outdata;
    outdata.open("Monthly Report.txt");
    //declare variables for calculations
    double totalSales = 0;
    double totalWeight = 0;
    double maxSale = 0; 
    double minSale = monthlyS[0][0];//bacause we want to compare for lowest number
    bool firstSaleFound = false;
    int countActiveDays = 0;// to count days with sales
    int bestDay = 0, worstDay = 0;
    //show output
    cout << "\n----- " << reportTitle << " -----" << endl;
    outdata << "----- " << reportTitle << " -----" << endl;
    cout << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << setw(15) << "Weight(KG)" << endl;
    outdata << left << setw(10) << "Day" << setw(15) << "Sales(RM)" << setw(15) << "Weight(KG)" << endl;
    cout << "----------------------------------------" << endl;
    outdata << "----------------------------------------" << endl;
    //to check each day sales
    for (int i = 0; i < 31; i++) 
    {
        // Only process days that had sales > 0
        if (monthlyS[i][0] > 0) 
        {
            cout << "Day " << left << setw(6) << (i + 1) << "RM " << fixed << setprecision(2) << setw(12) << monthlyS[i][0] << setw(10) << monthlyS[i][1] << endl;
            outdata << "Day " << left << setw(6) << (i + 1) << "RM " << fixed << setprecision(2) << setw(12) << monthlyS[i][0] << setw(10) << monthlyS[i][1] << endl;
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
            //if first sale found
            if (firstSaleFound == false) {
                minSale = monthlyS[i][0];
                worstDay = i + 1;
                firstSaleFound = true;
            }
            //if not first sale
            else if (monthlyS[i][0] < minSale) {
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
        minSale = 0;
    }
    //output totals
    cout << "----------------------------------------" << endl;
    outdata << "----------------------------------------" << endl;
    cout << "TOTAL SALES      : RM " << fixed << setprecision(2) << totalSales << endl;
    outdata << "TOTAL SALES      : RM " << fixed << setprecision(2) << totalSales << endl;
    cout << "TOTAL WEIGHT     : " << totalWeight << " KG" << endl;
    outdata << "TOTAL WEIGHT     : " << totalWeight << " KG" << endl;
    cout << "AVERAGE SALES    : RM " << averageSales << " (per active day)" << endl;
    outdata << "AVERAGE SALES    : RM " << averageSales << " (per active day)" << endl;
    cout << "HIGHEST SALE     : RM " << maxSale << " (Day " << bestDay << ")" << endl;
    outdata << "HIGHEST SALE     : RM " << maxSale << " (Day " << bestDay << ")" << endl;
    cout << "LOWEST SALE      : RM " << minSale << " (Day " << worstDay << ")" << endl;
    outdata << "LOWEST SALE      : RM " << minSale << " (Day " << worstDay << ")" << endl;

    outdata.close();
}

//Function to edit fruit details
void editFruit(Item inv[], int fCount) 
{
    string name;
    cout << "\n===================================" << endl;
    cout << "           EDIT FRUIT              " << endl;
    cout << "===================================" << endl;
    cout << "Enter Name of Fruit to Edit: ";
    getline(cin, name);

    //search
    int index = 0;
    bool check = false;
    for (int i = 0; i < fCount; i++) {
        if (inv[i].name == name)
        {
            index = i;
            check = true;
        }
    }
    if (check == true){
        //menu to edit
        cout << "\n--- Current Details for " << inv[index].name << " ---" << endl;
        cout << "1. Price  : RM " << inv[index].price << endl;
        cout << "2. Weight : " << inv[index].weight << " KG" << endl;
        cout << "3. Type   : " << inv[index].type << endl;
        cout << "4. Cancel Edit" << endl;

        int editChoice;
        cout << "\nSelect menu to edit (1-4): ";
        cin >> editChoice;
        cin.ignore();

        if (editChoice == 1) {
            cout << "Enter New Price: ";
            cin >> inv[index].price;
            cout << ">> Price updated successfully." << endl;
        } else if (editChoice == 2) {
            cout << "Enter New Weight: ";
            cin >> inv[index].weight;
            cout << ">> Weight updated successfully." << endl;
        } else if (editChoice == 3) {
            cout << "Enter New Type (Local/Imported): ";
            getline(cin, inv[index].type);
            cout << ">> Type updated successfully." << endl;
        } else if (editChoice == 4) {
            cout << ">> Edit Cancelled." << endl;
            return;
        } else {
            cout << ">> Invalid choice." << endl;
            return;
        }
    }
    else {
        cout << ">>Fruit not found in inventory." << endl;
    }
}

//function for splash screen
void splashScreen() 
{
    //clear screen
    system("cls"); 

    //Splash screen
    cout << "\n\n\n";
    cout << "\t\t  ===========================================" << endl;
    cout << "\t\t    ____  ____   ___   _   _  ____   _____  " << endl;
    cout << "\t\t   / ___||  _ \\ / _ \\ | | | ||  _ \\ |___  | " << endl;
    cout << "\t\t  | |  _ | |_) | | | || | | || |_) |   / /  " << endl;
    cout << "\t\t  | |_| ||  _ <| |_| || |_| ||  __/   / /   " << endl;
    cout << "\t\t   \\____||_| \\_\\\\___/  \\___/ |_|     /_/    " << endl;
    cout << "\t\t                                               " << endl;
    cout << "\t\t         FRUIT STALL MANAGEMENT SYSTEM         " << endl;
    cout << "\t\t  ===========================================" << endl;
    cout << "\n\n";

    //loading bar
    cout << "\t  Loading System: ";
    char x = 219; // This is a solid block character code in Windows Console
    
    for(int i = 0; i < 25; i++) {
        cout << x;      // Print the block
        Sleep(100);      // Wait 100 milliseconds
    }
    
    Sleep(500); //pause for 0.2 seconds
    
    //clear screen after loading
    system("cls");
}