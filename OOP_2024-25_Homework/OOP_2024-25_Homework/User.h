#pragma once

#include "String.h"

class User
{
public:
	User(size_t id, const String& fName, 
		const String& lName, const String& email, const String& pwd);
	User(const User& other);
	User& operator=(const User& other);
	virtual ~User() = default;

	virtual String getRole() = 0;
	size_t getId() const;
	const String& getFirstName() const;
	const String& getLastName() const;
	const String& getEmail() const;

	void setFirstName(const String& fName);
	void setLastName(const String& lName);
	void setEmail(const String& email);

	bool checkPassword(const String& pass) const;
	void changePassword(const String& currPass, const String& newPass);

protected:
	size_t id_;
	String firstName_;
	String lastName_;
	String email_;
	String password_;

	void copyFrom(const User& other);
};