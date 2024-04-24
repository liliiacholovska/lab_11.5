#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;

struct Product {
    char name[50];
    double price;
    int deliveryTime;
};

struct Company {
    char name[50];
    Product products[100];  
    int productCount;
};

void AddCompany(const string& filename);
void ListCompanies(const string& filename);
void ListCompanyDirectly(const string& filename);
void DeleteCompany(const string& filename, int index);
void UpdateCompany(const string& filename, int index);
int ReadCompanies(const string& filename, Company companies[], int max_companies);
void WriteCompanies(const string& filename, Company companies[], int companyCount);
int CalculateTotalDeliveryTime(const Company& company);
void SortCompaniesByDeliveryTime(const string& filename);
void FindCompaniesWithShortDelivery(const string& filename, const char* productName, int maxDays = 2);
void FindAffordableProducts(const string& filename, const char* companyName, double maxPrice);

int main() {
    string filename;
    cout << "Enter filename: ";
    getline(cin, filename);

    int choice;
    bool running = true;
    while (running) {
        cout << "\n";
        cout << "1. Load and list companies from file\n";
        cout << "2. Add a company\n";
        cout << "3. Delete a company\n";
        cout << "4. Update a company\n";
        cout << "5. List all companies\n";
        cout << "6. Sort companies by delivery time\n";
        cout << "7. Find companies that deliver a product within two days\n";
        cout << "8. Find products under a specified price in a given company\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  

        switch (choice) {
        case 1:
            ListCompanies(filename);
            break;
        case 2:
            AddCompany(filename);
            break;
        case 3: {
            cout << "Enter the index of the company to delete: ";
            int index;
            cin >> index;
            DeleteCompany(filename, index);
            break;
        }
        case 4: {
            int index;
            cout << "Enter the index of the company to update: ";
            cin >> index;
            cin.ignore();  
            UpdateCompany(filename, index);
            break;
        }
        case 5:
            ListCompanies(filename);
            break;
        case 6:
            SortCompaniesByDeliveryTime(filename);
            cout << "Companies sorted by delivery time:\n";
            ListCompanies(filename);
            break;
        case 7: {
            char productName[50];
            cout << "Enter the product name: ";
            cin.getline(productName, 50);
            FindCompaniesWithShortDelivery(filename, productName, 2);  
            break;
        }
        case 8: {
            char companyName[50];
            double maxPrice;
            cout << "Enter the company name: ";
            cin.getline(companyName, 50);
            cout << "Enter the maximum price: ";
            cin >> maxPrice;
            cin.ignore();  
            FindAffordableProducts(filename, companyName, maxPrice);
            break;
        }
        case 9:
            running = false;
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
    return 0;
}

void AddCompany(const string& filename) {
    Company newCompany;
    cout << "Enter company name: ";
    cin.getline(newCompany.name, 50);
    cout << "How many products does this company have? ";
    cin >> newCompany.productCount;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    for (int i = 0; i < newCompany.productCount; ++i) {
        cout << "Enter product name: ";
        cin.getline(newCompany.products[i].name, 50);
        cout << "Enter product price: ";
        cin >> newCompany.products[i].price;
        cout << "Enter delivery time (in days): ";
        cin >> newCompany.products[i].deliveryTime;
        cin.ignore(); 
    }

    Company companies[100];  
    int companyCount = ReadCompanies(filename, companies, 100);
    if (companyCount < 100) {
        companies[companyCount] = newCompany;
        companyCount++;
        WriteCompanies(filename, companies, companyCount);
    }
    else {
        cout << "Error: Maximum number of companies reached. Cannot add more companies." << endl;
    }
}

void ListCompanies(const string& filename) {
    Company companies[100];
    int companyCount = ReadCompanies(filename, companies, 100);
    for (int i = 0; i < companyCount; ++i) {
        cout << "Company " << i << ": " << companies[i].name << ", Products count: " << companies[i].productCount << "\n";
        for (int j = 0; j < companies[i].productCount; ++j) {
            cout << "\tProduct " << j + 1 << ": " << companies[i].products[j].name
                << ", Price: " << companies[i].products[j].price
                << ", Delivery Time: " << companies[i].products[j].deliveryTime << " days\n";
        }
    }
}

void ListCompanyDirectly(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Error opening file for reading.\n";
        return;
    }

    Company company;
    while (file.read(reinterpret_cast<char*>(&company), sizeof(Company))) {
        cout << "Company: " << company.name << ", Products count: " << company.productCount << "\n";
        for (int j = 0; j < company.productCount; ++j) {
            cout << "\tProduct " << j + 1 << ": " << company.products[j].name
                << ", Price: " << company.products[j].price
                << ", Delivery Time: " << company.products[j].deliveryTime << " days\n";
        }
    }
    file.close();
}

void DeleteCompany(const string& filename, int index) {
    Company companies[100];
    int companyCount = ReadCompanies(filename, companies, 100);
    if (index < companyCount) {
        for (int i = index; i < companyCount - 1; ++i) {
            companies[i] = companies[i + 1];
        }
        companyCount--;
        WriteCompanies(filename, companies, companyCount);
        cout << "Company deleted successfully.\n";
    }
    else {
        cout << "Invalid index.\n";
    }
}

void UpdateCompany(const string& filename, int index) {
    Company companies[100];
    int companyCount = ReadCompanies(filename, companies, 100);
    if (index < companyCount) {
        Company& company = companies[index];
        cout << "Enter new company name: ";
        cout << "How many products does this company have now? ";
        cin >> company.productCount;
        cin.ignore(); 

        for (int i = 0; i < company.productCount; ++i) {
            cout << "Enter product name: ";
            cin.getline(company.products[i].name, 50);
            cout << "Enter product price: ";
            cin >> company.products[i].price;
            cout << "Enter delivery time (in days): ";
            cin >> company.products[i].deliveryTime;
            cin.ignore(); 
        }
        WriteCompanies(filename, companies, companyCount);
        cout << "Company updated successfully.\n";
    }
    else {
        cout << "Invalid index.\n";
    }
}

int ReadCompanies(const string& filename, Company companies[], int max_companies) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Error opening file for reading.\n";
        return 0;  
    }

    int companyCount = 0;
    file.read(reinterpret_cast<char*>(&companyCount), sizeof(companyCount));
    if (companyCount > max_companies) {
        file.close();
        return 0; 
    }
    for (int i = 0; i < companyCount; ++i) {
        file.read(reinterpret_cast<char*>(&companies[i]), sizeof(Company));
    }
    file.close();
    return companyCount;
}

void WriteCompanies(const string& filename, Company companies[], int companyCount) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Error opening file for writing.\n";
        return;
    }

    file.write(reinterpret_cast<const char*>(&companyCount), sizeof(companyCount));
    for (int i = 0; i < companyCount; ++i) {
        file.write(reinterpret_cast<const char*>(&companies[i]), sizeof(Company));
    }
    file.close();
}

int CalculateTotalDeliveryTime(const Company& company) {
    int totalDeliveryTime = 0;
    for (int i = 0; i < company.productCount; ++i) {
        totalDeliveryTime += company.products[i].deliveryTime;
    }
    return totalDeliveryTime;
}

void SortCompaniesByDeliveryTime(const string& filename) {
    Company companies[100];
    int companyCount = ReadCompanies(filename, companies, 100);

    for (int i = 0; i < companyCount - 1; i++) {
        for (int j = i + 1; j < companyCount; j++) {
            int deliveryTimeI = CalculateTotalDeliveryTime(companies[i]);
            int deliveryTimeJ = CalculateTotalDeliveryTime(companies[j]);
            if (deliveryTimeI > deliveryTimeJ) {
                Company temp = companies[i];
                companies[i] = companies[j];
                companies[j] = temp;
            }
        }
    }

    WriteCompanies(filename, companies, companyCount);
}

void FindCompaniesWithShortDelivery(const string& filename, const char* productName, int maxDays) {
    Company companies[100];
    int companyCount = ReadCompanies(filename, companies, 100);
    bool found = false;
    for (int i = 0; i < companyCount; ++i) {
        for (int j = 0; j < companies[i].productCount; ++j) {
            if (strcmp(companies[i].products[j].name, productName) == 0 && companies[i].products[j].deliveryTime <= maxDays) {
                cout << "Company " << companies[i].name << " can deliver " << productName << " in " << maxDays << " days or less.\n";
                found = true;
            }
        }
    }
    if (!found) {
        cout << "No company found that can deliver " << productName << " in " << maxDays << " days or less.\n";
    }
}

void FindAffordableProducts(const string& filename, const char* companyName, double maxPrice) {
    Company companies[100];
    int companyCount = ReadCompanies(filename, companies, 100);
    bool found = false;
    for (int i = 0; i < companyCount; ++i) {
        if (strcmp(companies[i].name, companyName) == 0) {
            for (int j = 0; j < companies[i].productCount; ++j) {
                if (companies[i].products[j].price <= maxPrice) {
                    cout << "Product " << companies[i].products[j].name << " with price " << companies[i].products[j].price << " is under " << maxPrice << "\n";
                    found = true;
                }
            }
        }
    }
    if (!found) {
        cout << "No products found in " << companyName << " with price under " << maxPrice << ".\n";
    }
}