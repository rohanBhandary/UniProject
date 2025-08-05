#pragma once
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include "Account.h"

enum class UserRole {
    CUSTOMER,
    ADMIN
};

class User {
private:
    std::string userId;
    std::string username;
    std::string passwordHash;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phoneNumber;
    UserRole role;
    std::vector<std::shared_ptr<Account>> accounts;
    std::chrono::system_clock::time_point createdAt;
    bool isActive;
    std::chrono::system_clock::time_point lastLogin;

public:
    User(const std::string& user, const std::string& pass, const std::string& first, 
         const std::string& last, const std::string& emailAddr, const std::string& phone, 
         UserRole userRole = UserRole::CUSTOMER);
    
    // Getters
    std::string getUserId() const { return userId; }
    std::string getUsername() const { return username; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getFullName() const;
    std::string getEmail() const { return email; }
    std::string getPhoneNumber() const { return phoneNumber; }
    UserRole getRole() const { return role; }
    bool getIsActive() const { return isActive; }
    std::chrono::system_clock::time_point getLastLogin() const { return lastLogin; }
    
    // Account management
    void addAccount(std::shared_ptr<Account> account);
    std::vector<std::shared_ptr<Account>> getAccounts() const;
    std::shared_ptr<Account> getAccount(const std::string& accountNumber) const;
    
    // Authentication
    bool authenticate(const std::string& password) const;
    void updatePassword(const std::string& newPassword);
    void recordLogin();
    
    // User management
    void deactivate() { isActive = false; }
    void activate() { isActive = true; }
    void updateProfile(const std::string& first, const std::string& last, 
                      const std::string& emailAddr, const std::string& phone);
    
    // Utility
    std::string getRoleString() const;
    std::string getFormattedLastLogin() const;
    bool isAdmin() const { return role == UserRole::ADMIN; }
    
private:
    void generateUserId();
    std::string hashPassword(const std::string& password) const;
    bool verifyPassword(const std::string& password, const std::string& hash) const;
}; 