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
