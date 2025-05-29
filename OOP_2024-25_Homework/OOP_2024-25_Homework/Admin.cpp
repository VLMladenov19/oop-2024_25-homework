#include "Admin.h"

#include <fstream>

Admin::Admin(size_t id, const String& fName, const String& lName,
	const String& email, const String& pwd)
	: User(id, fName, lName, email, pwd)
{
}

UserRole Admin::getRole() const
{
	return UserRole::Admin;
}

std::ofstream& Admin::serialize(std::ofstream& os) const
{
    UserRole role = this->getRole();
    os.write((const char*)&role, sizeof(role));

    os.write((const char*)&this->id_, sizeof(this->id_));

    size_t fNameLen = this->firstName_.size();
    os.write((const char*)&fNameLen, sizeof(fNameLen));
    os.write((const char*)firstName_, fNameLen);

    size_t lNameLen = this->lastName_.size();
    os.write((const char*)&lNameLen, sizeof(lNameLen));
    os.write((const char*)lastName_, lNameLen);

    size_t emailLen = this->email_.size();
    os.write((const char*)&emailLen, sizeof(emailLen));
    os.write((const char*)email_, emailLen);

    size_t pwdLen = this->password_.size();
    os.write((const char*)&pwdLen, sizeof(pwdLen));
    os.write((const char*)password_, pwdLen);

    return os;
}
