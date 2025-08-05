#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "User.h"
#include "Account.h"
#include "Transaction.h"

class Bank {
private:
    std::string bankName;
    std::string bankCode;
    std::vector<std::shared_ptr<User>> users;
    std::vector<std::shared_ptr<Account>> accounts;
    std::vector<std::shared_ptr<Transaction>> allTransactions;
    std::shared_ptr<User> currentUser;
    
    // Statistics
    double totalAssets;
    int totalAccounts;
    int totalUsers;
    
    // File paths for persistence
    std::string usersFile;
    std::string accountsFile;
    std::string transactionsFile;

public:
    Bank(const std::string& name, const std::string& code);
    
    // Bank information
    std::string getBankName() const { return bankName; }
    std::string getBankCode() const { return bankCode; }
    
    // User management
    bool registerUser(const std::string& username, const std::string& password,
                     const std::string& firstName, const std::string& lastName,
                     const std::string& email, const std::string& phone,
                     UserRole role = UserRole::CUSTOMER);
    bool authenticateUser(const std::string& username, const std::string& password);
    void logoutUser();
    std::shared_ptr<User> getCurrentUser() const { return currentUser; }
    
    // Account management
    std::shared_ptr<Account> createAccount(const std::string& holderName, 
                                         AccountType type, double initialBalance = 0.0);
    std::shared_ptr<Account> createBusinessAccount(const std::string& holderName,
                                                  const std::string& businessName,
                                                  const std::string& taxId,
                                                  double initialBalance = 0.0);
    std::shared_ptr<Account> getAccount(const std::string& accountNumber) const;
    std::vector<std::shared_ptr<Account>> getAllAccounts() const;
    
    // Transaction management
    bool processTransaction(const std::string& fromAccount, const std::string& toAccount,
                          double amount, TransactionType type, const std::string& description = "");
    std::vector<std::shared_ptr<Transaction>> getTransactionHistory(const std::string& accountNumber) const;
    std::vector<std::shared_ptr<Transaction>> getAllTransactions() const;
    
    // Banking operations
    bool deposit(const std::string& accountNumber, double amount);
    bool withdraw(const std::string& accountNumber, double amount);
    bool transfer(const std::string& fromAccount, const std::string& toAccount, double amount);
    
    // Statistics and reporting
    double getTotalAssets() const { return totalAssets; }
    int getTotalAccounts() const { return totalAccounts; }
    int getTotalUsers() const { return totalUsers; }
    void updateStatistics();
    
    // Search and query
    std::shared_ptr<User> findUser(const std::string& username) const;
    std::vector<std::shared_ptr<Account>> findAccountsByHolder(const std::string& holderName) const;
    std::vector<std::shared_ptr<Transaction>> findTransactionsByDate(const std::string& accountNumber,
                                                                    const std::string& startDate,
                                                                    const std::string& endDate) const;
    
    // Data persistence
    bool saveData();
    bool loadData();
    
    // Admin functions
    std::vector<std::shared_ptr<User>> getAllUsers() const;
    bool deleteUser(const std::string& userId);
    bool deleteAccount(const std::string& accountNumber);
    void applyInterestToAllSavings();
    
private:
    void initializeBank();
    std::string generateBankCode();
    void updateAccountStatistics();
    void updateUserStatistics();
}; 