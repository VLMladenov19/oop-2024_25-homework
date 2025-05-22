#include "Admin.h"

Admin::Admin(size_t id, const String& fName, const String& lName,
	const String& email, const String& pwd)
	: User(id, fName, lName, email, pwd)
{
}

String Admin::getRole()
{
	return String("Admin");
}
