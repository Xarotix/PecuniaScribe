#include "PecuniaScribe.h"

// --- Global Variable Definitions ---
std::vector<Bill> bills;
int nextBillId = 1; // Initialize nextBillId

// --- System Utilities Definition ---
void clearScreen() {
    // Cross-platform screen clearing
#ifdef _WIN32
    system("cls"); // For Windows
#else
    // ANSI escape code for clearing screen on Linux/Mac
    // system("clear"); // This also works on most Unix-like systems
    std::cout << "\033[2J\033[1;1H"; // More direct ANSI sequence
#endif
}

// --- Encryption/Decryption Function Definition ---
std::string encryptDecrypt(const std::string& data, char key) {
    std::string result = data;
    for (size_t i = 0; i < data.length(); ++i) {
        result[i] = data[i] ^ key;
    }
    return result;
}

// --- File Operation Definitions ---
void saveBillsToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << RED_TEXT << "Error: Could not open file " << filename << " for saving." << RESET_COLOR << std::endl;
        return;
    }

    // Save the nextBillId first
    outFile << encryptDecrypt(std::to_string(nextBillId), ENCRYPTION_KEY) << std::endl;

    // Save each bill
    for (const auto& bill : bills) {
        std::stringstream ss;
        ss << bill.id << "|"
            << bill.title << "|"
            << bill.dueDate << "|"
            << std::fixed << std::setprecision(2) << bill.amount << "|"
            << (bill.isCrucial ? "1" : "0");
        outFile << encryptDecrypt(ss.str(), ENCRYPTION_KEY) << std::endl;
    }
    outFile.close();
}

void loadBillsFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        // If file doesn't exist, it's not an error on first run.
        return;
    }

    bills.clear(); // Clear existing in-memory bills before loading

    std::string line;

    // Load nextBillId
    if (std::getline(inFile, line)) {
        try {
            if (!line.empty()) { // Ensure line is not empty before processing
                nextBillId = std::stoi(encryptDecrypt(line, ENCRYPTION_KEY));
            }
            else {
                // Handle empty line for nextBillId if necessary, or assume default
                nextBillId = 1;
            }
        }
        catch (const std::exception& e) {
            std::cerr << RED_TEXT << "Error reading nextBillId: " << e.what() << ". Using default." << RESET_COLOR << std::endl;
            nextBillId = 1; // Reset to default if error
        }
    }
    else {
        // File might be empty or only had nextBillId which failed to read
        inFile.close();
        return;
    }

    // Load bills
    while (std::getline(inFile, line)) {
        if (line.empty()) continue; // Skip empty lines

        std::string decryptedLine = encryptDecrypt(line, ENCRYPTION_KEY);
        std::stringstream ss(decryptedLine);
        std::string token;
        Bill bill;

        try {
            // ID
            if (!std::getline(ss, token, '|')) throw std::runtime_error("Missing ID");
            bill.id = std::stoi(token);

            // Title
            if (!std::getline(ss, token, '|')) throw std::runtime_error("Missing Title");
            bill.title = token;

            // Due Date
            if (!std::getline(ss, token, '|')) throw std::runtime_error("Missing Due Date");
            bill.dueDate = token;

            // Amount
            if (!std::getline(ss, token, '|')) throw std::runtime_error("Missing Amount");
            bill.amount = std::stod(token);

            // Is Crucial
            if (!std::getline(ss, token, '|')) throw std::runtime_error("Missing Crucial Status");
            bill.isCrucial = (token == "1");

            bills.push_back(bill);
        }
        catch (const std::exception& e) {
            std::cerr << RED_TEXT << "Error parsing bill data: " << e.what() << " on line: " << decryptedLine << RESET_COLOR << std::endl;
        }
    }
    inFile.close();
}

// --- UI and Input Handling Definitions ---
void displayMenu() {
    // clearScreen(); // Clear screen before displaying the menu (This is handled in main.cpp)
    std::cout << "\n" << MAGENTA_TEXT << "====== PecuniaScribe - Bill Manager (" << APP_VERSION << ") ======" << RESET_COLOR << "\n";
    std::cout << CYAN_TEXT << "1. Add Bill" << RESET_COLOR << "\n";
    std::cout << CYAN_TEXT << "2. View Bills" << RESET_COLOR << "\n";
    std::cout << CYAN_TEXT << "3. Remove Bill" << RESET_COLOR << "\n";
    std::cout << CYAN_TEXT << "4. Exit" << RESET_COLOR << "\n";
    std::cout << "==================================================\n";
    std::cout << "Enter your choice: ";
}

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// --- Bill Management Definitions ---
void addBill() {
    clearScreen(); // Clear screen when starting to add a bill
    Bill newBill;
    newBill.id = nextBillId; // Assign ID first, only increment if bill is actually added

    std::cout << "\n" << GREEN_TEXT << "--- Add New Bill ---" << RESET_COLOR << "\n";
    std::cout << "Enter bill title (or leave empty to cancel): ";
    std::getline(std::cin, newBill.title);

    if (newBill.title.empty()) {
        std::cout << YELLOW_TEXT << "Add bill cancelled." << RESET_COLOR << std::endl;
        std::cout << YELLOW_TEXT << "Press Enter to continue..." << RESET_COLOR;
        // std::cin.get(); // Not strictly needed here as getline was just used.
                         // If there are issues with subsequent input, this might need adjustment.
        return;
    }

    if (newBill.title.find('|') != std::string::npos) {
        std::cout << RED_TEXT << "Error: Bill title cannot contain the '|' character. Bill not added." << RESET_COLOR << std::endl;
        std::cout << YELLOW_TEXT << "Press Enter to continue..." << RESET_COLOR;
        // std::cin.get();
        return;
    }

    std::cout << "Enter due date (e.g., YYYY-MM-DD or MM/DD): ";
    std::getline(std::cin, newBill.dueDate);
    if (newBill.dueDate.find('|') != std::string::npos) {
        std::cout << RED_TEXT << "Error: Due date cannot contain the '|' character. Bill not added." << RESET_COLOR << std::endl;
        std::cout << YELLOW_TEXT << "Press Enter to continue..." << RESET_COLOR;
        // std::cin.get();
        return;
    }

    std::cout << "Enter amount: $";
    while (!(std::cin >> newBill.amount) || newBill.amount < 0) {
        std::cout << RED_TEXT << "Invalid amount. Please enter a positive number: $" << RESET_COLOR;
        std::cin.clear();
        clearInputBuffer();
    }
    clearInputBuffer();

    char crucialChoice;
    std::cout << "Is this bill crucial? (y/n): ";
    while (true) {
        std::cin >> crucialChoice;
        crucialChoice = tolower(crucialChoice);
        if (crucialChoice == 'y' || crucialChoice == 'n') {
            newBill.isCrucial = (crucialChoice == 'y');
            clearInputBuffer();
            break;
        }
        else {
            std::cout << RED_TEXT << "Invalid input. Please enter 'y' or 'n': " << RESET_COLOR;
            std::cin.clear();
            clearInputBuffer();
        }
    }

    nextBillId++; // Increment ID only now that all inputs are valid and bill will be added
    bills.push_back(newBill);
    std::cout << GREEN_TEXT << "Bill '" << newBill.title << "' added successfully!" << RESET_COLOR << "\n";
    saveBillsToFile(FILENAME);
    std::cout << YELLOW_TEXT << "Press Enter to continue..." << RESET_COLOR;
    std::cin.get(); // Wait for user to press Enter
}

void viewBills() {
    clearScreen(); // Clear screen when starting to view bills
    std::cout << "\n" << YELLOW_TEXT << "--- Your Bills ---" << RESET_COLOR << "\n";
    if (bills.empty()) {
        std::cout << "No bills to display." << std::endl;
    }
    else {
        std::cout << std::left
            << std::setw(5) << "ID"
            << std::setw(30) << "Title"
            << std::setw(15) << "Due Date"
            << std::setw(15) << "Amount"
            << std::setw(10) << "Status" << std::endl;
        std::cout << std::string(75, '-') << std::endl;

        std::cout << std::fixed << std::setprecision(2);

        for (const auto& bill : bills) {
            std::string displayName = bill.title;
            std::string statusText;

            if (bill.isCrucial) {
                displayName = "* " + bill.title;
                statusText = BRIGHT_RED_TEXT + "Crucial" + RESET_COLOR;
                std::cout << BRIGHT_YELLOW_TEXT;
            }
            else {
                statusText = "Optional";
                std::cout << WHITE_TEXT;
            }

            std::cout << std::left << std::setw(5) << bill.id
                << std::setw(30) << displayName
                << std::setw(15) << bill.dueDate
                << "$" << std::setw(13) << bill.amount
                << std::setw(10) << statusText
                << RESET_COLOR << std::endl;
        }
        std::cout << std::string(75, '-') << std::endl;
    }
    std::cout << YELLOW_TEXT << "Press Enter to continue..." << RESET_COLOR;
    std::cin.get();
}

void removeBill() {
    clearScreen(); // Clear screen when starting to remove a bill
    std::cout << "\n" << RED_TEXT << "--- Remove Bill ---" << RESET_COLOR << "\n";
    if (bills.empty()) {
        std::cout << "No bills to remove." << std::endl;
        std::cout << YELLOW_TEXT << "Press Enter to continue..." << RESET_COLOR;
        std::cin.get();
        return;
    }

    // Calling viewBills will clear the screen again and then show bills.
    // It also has its own "Press Enter".
    viewBills();

    // After viewBills, the screen will show the list and wait for Enter.
    // Once Enter is pressed, we are back here.
    // We might want to clear the screen *again* before asking for ID to remove,
    // or ensure viewBills doesn't clear if called from here.
    // For now, viewBills will clear, show list, wait. Then control returns.
    // The prompt for ID will appear below the "Press Enter" from viewBills.
    // This might be slightly clunky. A `viewBillsInternal()` without clear/pause could be better.
    // However, for this change, we'll stick to the current structure.

    if (bills.empty()) { // This check is likely redundant if viewBills was just called.
        return;
    }

    std::cout << "Enter the ID of the bill to remove (or 0 to cancel): ";
    int idToRemove; // Moved declaration here
    while (!(std::cin >> idToRemove)) {
        std::cout << RED_TEXT << "Invalid input. Please enter a numeric ID: " << RESET_COLOR;
        std::cin.clear();
        clearInputBuffer();
        std::cout << "Enter the ID of the bill to remove (or 0 to cancel): "; // Re-prompt
    }
    clearInputBuffer();

    if (idToRemove == 0) {
        std::cout << YELLOW_TEXT << "Removal cancelled." << RESET_COLOR << std::endl;
        // No "Press Enter" here as one will follow shortly
    }
    else {
        bool found = false;
        for (auto it = bills.begin(); it != bills.end(); ++it) {
            if (it->id == idToRemove) {
                std::cout << GREEN_TEXT << "Bill '" << it->title << "' removed successfully." << RESET_COLOR << std::endl;
                bills.erase(it);
                found = true;
                saveBillsToFile(FILENAME);
                break;
            }
        }

        if (!found) {
            std::cout << RED_TEXT << "Bill with ID " << idToRemove << " not found." << RESET_COLOR << std::endl;
        }
    }
    std::cout << YELLOW_TEXT << "Press Enter to continue..." << RESET_COLOR;
    std::cin.get();
}
