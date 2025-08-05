#include "Account.h"
#include "Transaction.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

// Base Account implementation
Account::Account(const std::string& holderName, AccountType accType, double initialBalance)
    : accountHolderName(holderName), type(accType), balance(initialBalance), isActive(true) {
    generateAccountNumber();
    createdAt = std::chrono::system_clock::now();
}

void Account::generateAccountNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000000, 999999999);
    
    std::ostringstream oss;
    oss << getAccountTypePrefix() << std::setw(9) << std::setfill('0') << dis(gen);
    accountNumber = oss.str();
}

std::string Account::getAccountTypePrefix() const {
    switch (type) {
        case AccountType::SAVINGS: return "S";
        case AccountType::CHECKING: return "C";
        case AccountType::BUSINESS: return "B";
        default: return "X";
    }
}

bool Account::deposit(double amount) {
    if (amount <= 0 || !isActive) return false;
    
    balance += amount;
    
    auto transaction = std::make_shared<Transaction>(
        TransactionType::DEPOSIT, amount, "Deposit", "", accountNumber, balance
    );
    addTransaction(transaction);
    
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0 || !isActive || balance < amount) return false;
    
    balance -= amount;
    
    auto transaction = std::make_shared<Transaction>(
        TransactionType::WITHDRAWAL, amount, "Withdrawal", accountNumber, "", balance
    );
    addTransaction(transaction);
    
    return true;
}

bool Account::transfer(Account& targetAccount, double amount) {
    if (amount <= 0 || !isActive || balance < amount) return false;
    
    balance -= amount;
    targetAccount.balance += amount;
    
    auto transaction = std::make_shared<Transaction>(
        TransactionType::TRANSFER, amount, "Transfer to " + targetAccount.getAccountNumber(),
        accountNumber, targetAccount.getAccountNumber(), balance
    );
    addTransaction(transaction);
    
    auto targetTransaction = std::make_shared<Transaction>(
        TransactionType::TRANSFER, amount, "Transfer from " + accountNumber,
        accountNumber, targetAccount.getAccountNumber(), targetAccount.getBalance()
    );
    targetAccount.addTransaction(targetTransaction);
    
    return true;
}

void Account::addTransaction(std::shared_ptr<Transaction> transaction) {
    transactions.push_back(transaction);
}

std::vector<std::shared_ptr<Transaction>> Account::getTransactions() const {
    return transactions;
}

void Account::applyInterest() {
    // Base implementation does nothing
}

std::string Account::getAccountTypeString() const {
    switch (type) {
        case AccountType::SAVINGS: return "Savings";
        case AccountType::CHECKING: return "Checking";
        case AccountType::BUSINESS: return "Business";
        default: return "Unknown";
    }
}

std::string Account::getFormattedBalance() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << "$" << balance;
    return oss.str();
}

// SavingsAccount implementation
SavingsAccount::SavingsAccount(const std::string& holderName, double initialBalance)
    : Account(holderName, AccountType::SAVINGS, initialBalance), interestRate(0.025) {
    lastInterestDate = std::chrono::system_clock::now();
}

double SavingsAccount::calculateInterest() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - lastInterestDate);
    double hours = duration.count();
    
    // Calculate daily interest
    double dailyRate = interestRate / 365.0;
    double days = hours / 24.0;
    
    return balance * dailyRate * days;
}

void SavingsAccount::applyInterest() {
    double interest = calculateInterest();
    if (interest > 0) {
        balance += interest;
        lastInterestDate = std::chrono::system_clock::now();
        
        auto transaction = std::make_shared<Transaction>(
            TransactionType::INTEREST, interest, "Interest earned", "", accountNumber, balance
        );
        addTransaction(transaction);
    }
}

// CheckingAccount implementation
CheckingAccount::CheckingAccount(const std::string& holderName, double initialBalance)
    : Account(holderName, AccountType::CHECKING, initialBalance), overdraftLimit(500.0) {
}

bool CheckingAccount::withdraw(double amount) {
    if (amount <= 0 || !isActive) return false;
    
    double availableBalance = balance + overdraftLimit;
    if (amount > availableBalance) return false;
    
    balance -= amount;
    
    auto transaction = std::make_shared<Transaction>(
        TransactionType::WITHDRAWAL, amount, "Withdrawal", accountNumber, "", balance
    );
    addTransaction(transaction);
    
    return true;
}

// BusinessAccount implementation
BusinessAccount::BusinessAccount(const std::string& holderName, const std::string& business, 
                               const std::string& tax, double initialBalance)
    : Account(holderName, AccountType::BUSINESS, initialBalance), 
      businessName(business), taxId(tax), monthlyFee(25.0) {
} 