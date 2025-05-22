#include "User.h"

size_t User::getId() const
{
    return this->id_;
}

const String& User::getFirstName() const
{
    return this->fName_;
}

const String& User::getLastName() const
{
    return this->lName_;
}

const String& User::getEmail() const
{
    return this->email_;
}

String& User::setFirstName(String fName)
{
    this->fName_ = fName;
}

String& User::setLastName(String lName)
{
    this->lName_ = lName;
}

String& User::setEmail(String email)
{
    this->email_ = email;
}

bool User::checkPassword(String pass) const
{
    return this->pass_ == pass;
}

void User::changePassword(String currPass, String newPass)
{
    if (this->pass_ == currPass)
        this->pass_ = newPass;
}
