#pragma once

#include "String.h"

enum class UserRole
{
	None,
	Admin,
	Teacher,
	Student
};

String roleToString(UserRole role);
UserRole stringToRole(const String& string);

class User
{
public:
	User(size_t id, const String& fName, 
		const String& lName, const String& email, const String& pwd);
	User(const User& other);
	User& operator=(const User& other);
	User(User&& other) noexcept;
	User& operator=(User&& other) noexcept;
	virtual ~User() = default;

	virtual UserRole getRole() const;
	size_t getId() const;
	const String& getFirstName() const;
	const String& getLastName() const;
	const String& getEmail() const;

	void setFirstName(const String& fName);
	void setLastName(const String& lName);
	void setEmail(const String& email);

	bool checkPassword(const String& pass) const;
	void changePassword(const String& currPass, const String& newPass);

	virtual std::ofstream& serialize(std::ofstream& os) const;
	virtual std::ofstream& deserialize(std::ofstream& os);

protected:
	size_t id_;
	String firstName_;
	String lastName_;
	String email_;
	String password_;

	void copyFrom(const User& other);
};