#include "User.h"

User::User(size_t id, const String& fName, const String& lName, 
    const String& email, const String& pwd)
    : id_(id), firstName_(fName), lastName_(lName), email_(email), password_(pwd)
{
}

User::User(const User& other)
{
    this->copyFrom(other);
}

User& User::operator=(const User& other)
{
    if (this != &other)
    {
        this->copyFrom(other);
    }

    return *this;
}

UserRole User::getRole() const
{
    return UserRole::None;
}

size_t User::getId() const
{
    return this->id_;
}

const String& User::getFirstName() const
{
    return this->firstName_;
}

const String& User::getLastName() const
{
    return this->lastName_;
}

const String& User::getEmail() const
{
    return this->email_;
}

void User::setFirstName(const String& fName)
{
    this->firstName_ = fName;
}

void User::setLastName(const String& lName)
{
    this->lastName_ = lName;
}

void User::setEmail(const String& email)
{
    this->email_ = email;
}

bool User::checkPassword(const String& pwd) const
{
    return this->password_ == pwd;
}

void User::changePassword(const String& currPwd, const String& newPwd)
{
    if (this->password_ == currPwd)
    {
        this->password_ = newPwd;
    }
}

void User::copyFrom(const User& other)
{
    this->id_ = other.id_;
    this->firstName_ = other.firstName_;
    this->lastName_ = other.lastName_;
    this->email_ = other.email_;
    this->password_ = other.password_;
}

String roleToString(UserRole role)
{
    switch (role)
    {
    case UserRole::Admin: return "Admin";
    case UserRole::Teacher: return "Teacher";
    case UserRole::Student: return "Student";
    default: return "None";
    }
}

UserRole stringToRole(const String& string)
{
    String lower = string.toLower();

    if (lower == "admin") return UserRole::Admin;
    if (lower == "teacher") return UserRole::Teacher;
    if (lower == "student") return UserRole::Student;

    return UserRole::None;
}
