#pragma once

#include "String.h"

class User
{
public:
	virtual ~User() = 0;

	virtual String getRole() = 0;
	size_t getId() const;
	const String& getFirstName() const;
	const String& getLastName() const;
	const String& getEmail() const;

	String& setFirstName(String fName);
	String& setLastName(String lName);
	String& setEmail(String email);

	bool checkPassword(String pass) const;
	void changePassword(String currPass, String newPass);

protected:
	size_t id_;
	String firstName_;
	String lastName_;
	String email_;
	String password_;
};