#pragma once

#include "User.h"
#include "String.h"

class Student : public User
{
public:
	Student(size_t id, const String& fName, const String& lName,
		const String& email, const String& pwd);

	UserRole getRole() const override;
};