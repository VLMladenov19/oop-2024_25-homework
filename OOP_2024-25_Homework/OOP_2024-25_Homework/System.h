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
	void finalize();

private:
	const char* usersFile = "users.bin";

	Vector<User*> users;
	User* currentUser;

	Response<void> updateFile();
	Response<void> updateUsers();

	Response<void> serializeUsers();
	Response<void> deserializeUsers();

	User* getUserById(size_t id) const;
	User* getUserByEmail(const String& email) const;
	User* getCurrentUser() const;

	Response<User*> login(size_t id, String pwd);
	Response<void> logout();
	Response<User*> registerUser(User* user);
	void removeUser(size_t id);

	void clearUsers();
	void ensureAdminCreated();
	size_t getNextId();

	const size_t COMMAND_INDEX = 0;
};