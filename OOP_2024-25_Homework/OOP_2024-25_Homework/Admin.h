#pragma once

#include "User.h"

class Admin : public User
{
public:
	Admin(size_t id, const String& fName, const String& lName,
		const String& email, const String& pwd);

	String getRole() override;

	void registerUser(const User& user);
	void removeUser(size_t id);
};