#pragma once

#include "User.h"
#include "Response.h"
#include "Vector.h"

class System
{
public:
	System();
	~System();

	void initialize();
	void loop();
	Response<void> updateFile();
	Response<void> updateUsers();
	void finalize();

	Response<User*> getUserById(size_t id) const;
	Response<User*> getUserByEmail(const String& email) const;
	const Vector<User*>& getUsers() const;
	User* getCurrentUser() const;

	Response<void> serialize();
	Response<void> deserialize();

	Response<User*> login();
	Response<User*> registerUser(const User& user);
	void removeUser(size_t id);

private:
	const char* usersFile = "users.bin";

	Vector<User*> users;
	User* currentUser;

	void clearUsers();
	void ensureAdminCreated();
};