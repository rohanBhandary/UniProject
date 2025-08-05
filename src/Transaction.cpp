#include "Transaction.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

Transaction::Transaction(TransactionType transType, double transAmount, 
                       const std::string& desc, const std::string& fromAcc,
                       const std::string& toAcc, double balanceAfterTrans)
    : type(transType), amount(transAmount), description(desc), 
      fromAccount(fromAcc), toAccount(toAcc), balanceAfter(balanceAfterTrans) {
    generateTransactionId();
    timestamp = std::chrono::system_clock::now();
}

void Transaction::generateTransactionId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dis(100000000000LL, 999999999999LL);
    
    std::ostringstream oss;
    oss << "TXN" << std::setw(12) << std::setfill('0') << dis(gen);
    transactionId = oss.str();
}

std::string Transaction::getTypeString() const {
    switch (type) {
        case TransactionType::DEPOSIT: return "Deposit";
        case TransactionType::WITHDRAWAL: return "Withdrawal";
        case TransactionType::TRANSFER: return "Transfer";
        case TransactionType::INTEREST: return "Interest";
        case TransactionType::FEE: return "Fee";
        default: return "Unknown";
    }
}

std::string Transaction::getFormattedAmount() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << "$" << amount;
    return oss.str();
}

std::string Transaction::getFormattedTimestamp() const {
    auto time_t = std::chrono::system_clock::to_time_t(timestamp);
    std::tm* tm = std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Transaction::getFormattedBalanceAfter() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << "$" << balanceAfter;
    return oss.str();
} 