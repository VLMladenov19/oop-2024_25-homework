#pragma once

#include "User.h"

class System
{
public:
	System();
	~System();

	void start();
	void cycle();
	void update();
	void end();

	User* getUserById() const;
	User* getUserByEmail() const;
	//const Vector<User>& getUsers() const;

	void serialize();
	void deserialize();

	User* login();
	void registerUser(const User& user);
	void removeUser(size_t id);

private:
	const char* usersFile = "users.bin";

	//Vector<User*> users;
	User* currentUser;

	void ensureAdminCreated();
};