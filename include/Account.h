#pragma once
#include <string>
#include <vector>
#include <memory>
#include <chrono>

enum class AccountType {
    SAVINGS,
    CHECKING,
    BUSINESS
};

class Transaction;

class Account {
protected:
    std::string accountNumber;
    std::string accountHolderName;
    double balance;
    AccountType type;
    std::vector<std::shared_ptr<Transaction>> transactions;
    std::chrono::system_clock::time_point createdAt;
    bool isActive;

public:
    Account(const std::string& holderName, AccountType accType, double initialBalance = 0.0);
    virtual ~Account() = default;

    // Getters
    std::string getAccountNumber() const { return accountNumber; }
    std::string getAccountHolderName() const { return accountHolderName; }
    double getBalance() const { return balance; }
    AccountType getType() const { return type; }
    bool getIsActive() const { return isActive; }
    
    // Account operations
    virtual bool deposit(double amount);
    virtual bool withdraw(double amount);
    virtual bool transfer(Account& targetAccount, double amount);
    
    // Transaction history
    void addTransaction(std::shared_ptr<Transaction> transaction);
    std::vector<std::shared_ptr<Transaction>> getTransactions() const;
    
    // Interest calculation (for savings accounts)
    virtual double calculateInterest() const { return 0.0; }
    virtual void applyInterest();
    
    // Account management
    void deactivate() { isActive = false; }
    void activate() { isActive = true; }
    
    // Utility
    std::string getAccountTypeString() const;
    std::string getFormattedBalance() const;
    
protected:
    void generateAccountNumber();
    std::string getAccountTypePrefix() const;
};

class SavingsAccount : public Account {
private:
    double interestRate;
    std::chrono::system_clock::time_point lastInterestDate;

public:
    SavingsAccount(const std::string& holderName, double initialBalance = 0.0);
    
    double calculateInterest() const override;
    void applyInterest() override;
    
    double getInterestRate() const { return interestRate; }
    void setInterestRate(double rate) { interestRate = rate; }
};

class CheckingAccount : public Account {
private:
    double overdraftLimit;

public:
    CheckingAccount(const std::string& holderName, double initialBalance = 0.0);
    
    bool withdraw(double amount) override;
    
    double getOverdraftLimit() const { return overdraftLimit; }
    void setOverdraftLimit(double limit) { overdraftLimit = limit; }
};

class BusinessAccount : public Account {
private:
    std::string businessName;
    std::string taxId;
    double monthlyFee;

public:
    BusinessAccount(const std::string& holderName, const std::string& business, 
                   const std::string& tax, double initialBalance = 0.0);
    
    std::string getBusinessName() const { return businessName; }
    std::string getTaxId() const { return taxId; }
    double getMonthlyFee() const { return monthlyFee; }
    void setMonthlyFee(double fee) { monthlyFee = fee; }
}; 