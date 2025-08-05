#include "Bank.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

Bank::Bank(const std::string& name, const std::string& code)
    : bankName(name), bankCode(code), totalAssets(0.0), totalAccounts(0), totalUsers(0) {
    initializeBank();
    usersFile = "data/users.dat";
    accountsFile = "data/accounts.dat";
    transactionsFile = "data/transactions.dat";
}

void Bank::initializeBank() {
    // Create default admin user
    registerUser("admin", "admin123", "System", "Administrator", 
                "admin@bank.com", "555-0000", UserRole::ADMIN);
    
    // Create a sample customer
    registerUser("john", "password123", "John", "Doe", 
                "john.doe@email.com", "555-1234", UserRole::CUSTOMER);
    
    // Create sample accounts for the customer
    auto john = findUser("john");
    if (john) {
        auto savings = createAccount("John Doe", AccountType::SAVINGS, 5000.0);
        auto checking = createAccount("John Doe", AccountType::CHECKING, 2500.0);
        john->addAccount(savings);
        john->addAccount(checking);
    }
    
    updateStatistics();
}

std::string Bank::generateBankCode() {
    return "BANK" + std::to_string(std::rand() % 10000);
}

bool Bank::registerUser(const std::string& username, const std::string& password,
                       const std::string& firstName, const std::string& lastName,
                       const std::string& email, const std::string& phone,
                       UserRole role) {
    // Check if username already exists
    if (findUser(username)) {
        return false;
    }
    
    auto user = std::make_shared<User>(username, password, firstName, lastName, email, phone, role);
    users.push_back(user);
    updateUserStatistics();
    return true;
}

bool Bank::authenticateUser(const std::string& username, const std::string& password) {
    auto user = findUser(username);
    if (user && user->authenticate(password) && user->getIsActive()) {
        currentUser = user;
        user->recordLogin();
        return true;
    }
    return false;
}

void Bank::logoutUser() {
    currentUser = nullptr;
}

std::shared_ptr<Account> Bank::createAccount(const std::string& holderName, 
                                           AccountType type, double initialBalance) {
    std::shared_ptr<Account> account;
    
    switch (type) {
        case AccountType::SAVINGS:
            account = std::make_shared<SavingsAccount>(holderName, initialBalance);
            break;
        case AccountType::CHECKING:
            account = std::make_shared<CheckingAccount>(holderName, initialBalance);
            break;
        case AccountType::BUSINESS:
            account = std::make_shared<BusinessAccount>(holderName, "Business", "TAX123", initialBalance);
            break;
    }
    
    if (account) {
        accounts.push_back(account);
        updateAccountStatistics();
    }
    
    return account;
}

std::shared_ptr<Account> Bank::createBusinessAccount(const std::string& holderName,
                                                    const std::string& businessName,
                                                    const std::string& taxId,
                                                    double initialBalance) {
    auto account = std::make_shared<BusinessAccount>(holderName, businessName, taxId, initialBalance);
    accounts.push_back(account);
    updateAccountStatistics();
    return account;
}

std::shared_ptr<Account> Bank::getAccount(const std::string& accountNumber) const {
    for (const auto& account : accounts) {
        if (account->getAccountNumber() == accountNumber) {
            return account;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Account>> Bank::getAllAccounts() const {
    return accounts;
}

bool Bank::processTransaction(const std::string& fromAccount, const std::string& toAccount,
                            double amount, TransactionType type, const std::string& description) {
    auto transaction = std::make_shared<Transaction>(type, amount, description, fromAccount, toAccount);
    allTransactions.push_back(transaction);
    
    // Add transaction to relevant accounts
    if (!fromAccount.empty()) {
        auto account = getAccount(fromAccount);
        if (account) {
            account->addTransaction(transaction);
        }
    }
    
    if (!toAccount.empty() && fromAccount != toAccount) {
        auto account = getAccount(toAccount);
        if (account) {
            account->addTransaction(transaction);
        }
    }
    
    return true;
}

std::vector<std::shared_ptr<Transaction>> Bank::getTransactionHistory(const std::string& accountNumber) const {
    auto account = getAccount(accountNumber);
    return account ? account->getTransactions() : std::vector<std::shared_ptr<Transaction>>();
}

std::vector<std::shared_ptr<Transaction>> Bank::getAllTransactions() const {
    return allTransactions;
}

bool Bank::deposit(const std::string& accountNumber, double amount) {
    auto account = getAccount(accountNumber);
    if (account && account->deposit(amount)) {
        processTransaction("", accountNumber, amount, TransactionType::DEPOSIT, "Deposit");
        updateStatistics();
        return true;
    }
    return false;
}

bool Bank::withdraw(const std::string& accountNumber, double amount) {
    auto account = getAccount(accountNumber);
    if (account && account->withdraw(amount)) {
        processTransaction(accountNumber, "", amount, TransactionType::WITHDRAWAL, "Withdrawal");
        updateStatistics();
        return true;
    }
    return false;
}

bool Bank::transfer(const std::string& fromAccount, const std::string& toAccount, double amount) {
    auto from = getAccount(fromAccount);
    auto to = getAccount(toAccount);
    
    if (from && to && from->transfer(*to, amount)) {
        processTransaction(fromAccount, toAccount, amount, TransactionType::TRANSFER, "Transfer");
        updateStatistics();
        return true;
    }
    return false;
}

void Bank::updateStatistics() {
    updateAccountStatistics();
    updateUserStatistics();
}

void Bank::updateAccountStatistics() {
    totalAssets = 0.0;
    totalAccounts = accounts.size();
    
    for (const auto& account : accounts) {
        if (account->getIsActive()) {
            totalAssets += account->getBalance();
        }
    }
}

void Bank::updateUserStatistics() {
    totalUsers = users.size();
}

std::shared_ptr<User> Bank::findUser(const std::string& username) const {
    for (const auto& user : users) {
        if (user->getUsername() == username) {
            return user;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Account>> Bank::findAccountsByHolder(const std::string& holderName) const {
    std::vector<std::shared_ptr<Account>> result;
    for (const auto& account : accounts) {
        if (account->getAccountHolderName() == holderName) {
            result.push_back(account);
        }
    }
    return result;
}

std::vector<std::shared_ptr<User>> Bank::getAllUsers() const {
    return users;
}

bool Bank::deleteUser(const std::string& userId) {
    auto it = std::find_if(users.begin(), users.end(),
                           [&userId](const std::shared_ptr<User>& user) {
                               return user->getUserId() == userId;
                           });
    
    if (it != users.end()) {
        users.erase(it);
        updateUserStatistics();
        return true;
    }
    return false;
}

bool Bank::deleteAccount(const std::string& accountNumber) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
                           [&accountNumber](const std::shared_ptr<Account>& account) {
                               return account->getAccountNumber() == accountNumber;
                           });
    
    if (it != accounts.end()) {
        (*it)->deactivate();
        updateAccountStatistics();
        return true;
    }
    return false;
}

void Bank::applyInterestToAllSavings() {
    for (auto& account : accounts) {
        if (account->getType() == AccountType::SAVINGS) {
            account->applyInterest();
        }
    }
    updateStatistics();
}

bool Bank::saveData() {
    // Implementation for data persistence
    return true;
}

bool Bank::loadData() {
    // Implementation for data loading
    return true;
} 