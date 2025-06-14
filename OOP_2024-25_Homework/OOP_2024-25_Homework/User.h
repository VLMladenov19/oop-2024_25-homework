#pragma once

#include <fstream>

#include "String.h"
#include "Mail.h"
#include "Response.h"

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

	Response<void> sendMessage(const Mail& mail) const;
	Vector<Mail> getMail() const;

	virtual std::ofstream& serialize(std::ofstream& os) const;
	virtual std::ifstream& deserialize(std::ifstream& is);

protected:
	size_t id_;
	String firstName_;
	String lastName_;
	String email_;
	String password_;

	void setId(size_t id);
	void setPassword(const String& pwd);

	String readString(std::ifstream& is) const;

	void copyFrom(const User& other);
};