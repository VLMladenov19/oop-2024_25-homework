#pragma once

#include <fstream>

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
	const String usersFile = "users.bin";

	Vector<User*> users;
	User* currentUser;

	Response<void> updateFile();
	Response<void> updateUsers();

	Response<void> serializeUsers();
	Response<void> deserializeUsers();

	User* getUserById(size_t id) const;
	User* getUserByEmail(const String& email) const;
	User* getCurrentUser() const;

	Response<void> login(size_t id, String pwd);
	Response<void> logout();
	Response<void> registerUser(User* user);
	Response<void> removeUser(size_t id);

	void clearUsers();
	void ensureAdminCreated();
	size_t getNextId();

	void loginHandler(const Vector<String>& input);
	void logoutHandler(const Vector<String>& input);
	void addTeacherHandler(const Vector<String>& input);
	void addStudentHandler(const Vector<String>& input);
	void removeHandler(const Vector<String>& input);
	void messageAllHandler(const String& line);
	void mailboxHandler(const Vector<String>& input);

	const size_t COMMAND_INDEX = 0;

	const size_t LOGIN_INPUT_SIZE = 3;
	const size_t LOGIN_ID_INDEX = 1;
	const size_t LOGIN_PASSWORD_INDEX = 2;

	const size_t LOGOUT_INPUT_SIZE = 1;

	const size_t ADD_USER_INPUT_SIZE = 4;
	const size_t ADD_USER_FIRST_NAME_INDEX = 1;
	const size_t ADD_USER_LAST_NAME_INDEX = 2;
	const size_t ADD_USER_PASSWORD_INDEX = 3;

	const size_t REMOVE_USER_INPUT_SIZE = 2;
	const size_t REMOVE_USER_ID_INDEX = 1;

	const size_t MESSAGE_ALL_COMMAND_LEN = 11;

	const size_t MAILBOX_INPUT_SIZE = 1;
};