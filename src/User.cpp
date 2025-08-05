#include "User.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <algorithm>

User::User(const std::string& user, const std::string& pass, const std::string& first, 
           const std::string& last, const std::string& emailAddr, const std::string& phone, 
           UserRole userRole)
    : username(user), firstName(first), lastName(last), email(emailAddr), 
      phoneNumber(phone), role(userRole), isActive(true) {
    generateUserId();
    passwordHash = hashPassword(pass);
    createdAt = std::chrono::system_clock::now();
    lastLogin = createdAt;
}

void User::generateUserId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000000, 999999999);
    
    std::ostringstream oss;
    oss << "USR" << std::setw(9) << std::setfill('0') << dis(gen);
    userId = oss.str();
}

std::string User::getFullName() const {
    return firstName + " " + lastName;
}

void User::addAccount(std::shared_ptr<Account> account) {
    accounts.push_back(account);
}

std::vector<std::shared_ptr<Account>> User::getAccounts() const {
    return accounts;
}

std::shared_ptr<Account> User::getAccount(const std::string& accountNumber) const {
    for (const auto& account : accounts) {
        if (account->getAccountNumber() == accountNumber) {
            return account;
        }
    }
    return nullptr;
}

bool User::authenticate(const std::string& password) const {
    return verifyPassword(password, passwordHash);
}

void User::updatePassword(const std::string& newPassword) {
    passwordHash = hashPassword(newPassword);
}

void User::recordLogin() {
    lastLogin = std::chrono::system_clock::now();
}

void User::updateProfile(const std::string& first, const std::string& last, 
                        const std::string& emailAddr, const std::string& phone) {
    firstName = first;
    lastName = last;
    email = emailAddr;
    phoneNumber = phone;
}

std::string User::getRoleString() const {
    switch (role) {
        case UserRole::CUSTOMER: return "Customer";
        case UserRole::ADMIN: return "Administrator";
        default: return "Unknown";
    }
}

std::string User::getFormattedLastLogin() const {
    auto time_t = std::chrono::system_clock::to_time_t(lastLogin);
    std::tm* tm = std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string User::hashPassword(const std::string& password) const {
    // Simple hash implementation - in production, use a proper cryptographic hash
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}

bool User::verifyPassword(const std::string& password, const std::string& hash) const {
    return hashPassword(password) == hash;
} 