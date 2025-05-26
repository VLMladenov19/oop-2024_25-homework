#pragma once

#include "User.h"

class System
{
public:
	System();

	User getUserById() const;
	User getUserByEmail() const;
	//const Vector<User>& getUsers() const;

	User login();

	void serialize();
	void deserialize();

private:
	const char* filename = "users.bin";

	//Vector<User> users;
	User currentUser;

	void ensureAdminCreated();
};