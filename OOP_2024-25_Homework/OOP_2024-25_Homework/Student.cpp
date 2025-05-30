#include "Student.h"

Student::Student(size_t id, const String& fName, const String& lName,
	const String& email, const String& pwd)
	: User(id, fName, lName, email, pwd)
{
}

UserRole Student::getRole() const
{
	return UserRole::Student;
}
