#pragma once
#include <string>
#include <chrono>
#include <memory>

enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER,
    INTEREST,
    FEE
};

class Transaction {
private:
    std::string transactionId;
    TransactionType type;
    double amount;
    std::string description;
    std::chrono::system_clock::time_point timestamp;
    std::string fromAccount;
    std::string toAccount;
    double balanceAfter;

public:
    Transaction(TransactionType transType, double transAmount, 
               const std::string& desc, const std::string& fromAcc = "",
               const std::string& toAcc = "", double balanceAfterTrans = 0.0);
    
    // Getters
    std::string getTransactionId() const { return transactionId; }
    TransactionType getType() const { return type; }
    double getAmount() const { return amount; }
    std::string getDescription() const { return description; }
    std::chrono::system_clock::time_point getTimestamp() const { return timestamp; }
    std::string getFromAccount() const { return fromAccount; }
    std::string getToAccount() const { return toAccount; }
    double getBalanceAfter() const { return balanceAfter; }
    
    // Utility methods
    std::string getTypeString() const;
    std::string getFormattedAmount() const;
    std::string getFormattedTimestamp() const;
    std::string getFormattedBalanceAfter() const;
    
private:
    void generateTransactionId();
}; 