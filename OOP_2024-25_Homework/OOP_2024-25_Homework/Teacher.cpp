#include "Teacher.h"

#include<fstream>

Teacher::Teacher(size_t id, const String& fName, const String& lName, 
	const String& email, const String& pwd)
	: User(id, fName, lName, email, pwd)
{
}

UserRole Teacher::getRole() const
{
    return UserRole::Teacher;
}
