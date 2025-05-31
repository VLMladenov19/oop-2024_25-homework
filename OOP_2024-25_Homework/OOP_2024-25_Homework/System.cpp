#include "System.h"
#include "Admin.h"
#include "Teacher.h"
#include "Student.h"

#include <fstream>

System::System() : currentUser(nullptr)
{
}

System::~System()
{
	this->finalize();
	this->clearUsers();
}

void System::initialize()
{
	this->updateUsers();
	this->ensureAdminCreated();
}

void System::loop()
{
	while (true)
	{
		std::cout << "> ";
		String line;
		getline(std::cin, line);

		if (line.empty())
		{
			continue;
		}

		Vector<String> input = line.split();
		String command = input[System::COMMAND_INDEX];
		if (command == "login")
		{
			this->loginHandler(input);
			continue;
		}
		if (command == "logout")
		{
			this->logoutHandler(input);
			continue;
		}
		if (command == "add_teacher")
		{
			this->addTeacherHandler(input);
			continue;
		}
		if (command == "add_student")
		{
			this->addStudentHandler(input);
			continue;
		}
		if (command == "remove")
		{
			this->removeHandler(input);
			continue;
		}
	}
}

void System::finalize()
{
	this->updateFile();
}

Response<void> System::updateFile()
{
	return this->serializeUsers();
}

Response<void> System::updateUsers()
{
	return this->deserializeUsers();
}

Response<void> System::serializeUsers()
{
	std::ofstream os(System::usersFile, std::ios::binary | std::ios::out);

	if (!os.is_open())
	{
		return Response<void>(false,
			String("Failed to open file for writing: ") + System::usersFile);
	}

	size_t usersCount = this->users.size();
	os.write((const char*)&usersCount, sizeof(usersCount));

	for (size_t i = 0; i < usersCount; i++)
	{
		User* user = this->users[i];

		if (user)
		{
			user->serialize(os);
		}
	}

	os.close();

	return Response<void>(true);
}

Response<void> System::deserializeUsers()
{
	std::ifstream is(System::usersFile, std::ios::binary | std::ios::out);

	if (!is.is_open())
	{
		return Response<void>(false,
			String("Failed to open file for writing: ") + System::usersFile);
	}

	size_t usersCount = 0;
	is.read((char*)&usersCount, sizeof(usersCount));

	this->clearUsers();

	for (size_t i = 0; i < usersCount; i++)
	{
		UserRole role;
		is.read((char*)&role, sizeof(role));

		User* user = nullptr;
		switch (role)
		{
		case UserRole::Admin:
			user = new Admin(0, "", "", "", "");
			break;
		case UserRole::Teacher:
			user = new Teacher(0, "", "", "", "");
			break;
		case UserRole::Student:
			user = new Student(0, "", "", "", "");
			break;
		default:
			user = new User(0, "", "", "", "");
			break;
		}

		user->deserialize(is);
		this->users.push_back(user);
	}

	is.close();

	return Response<void>(true);
}

User* System::getUserById(size_t id) const
{
	size_t usersCount = this->users.size();
	for (size_t i = 0; i < usersCount; i++)
	{
		if (this->users[i]->getId() == id)
			return this->users[i];
	}
	return nullptr;
}

User* System::getUserByEmail(const String& email) const
{
	size_t usersCount = this->users.size();
	for (size_t i = 0; i < usersCount; i++)
	{
		if (this->users[i]->getEmail() == email)
			return this->users[i];
	}
	return nullptr;
}

User* System::getCurrentUser() const
{
	return this->currentUser;
}

Response<void> System::login(size_t id, String pwd)
{
	if (this->currentUser)
	{
		return Response<void>(false, "User already logged in");
	}

	User* user = this->getUserById(id);

	if (!user)
	{
		return Response<void>(false, "Invalid user id!");
	}

	if (!user->checkPassword(pwd))
	{
		return Response<void>(false, "Wrong password!");
	}

	this->currentUser = user;
	return Response<void>(true, "Login successful!");
}

Response<void> System::logout()
{
	if (currentUser)
	{
		currentUser = nullptr;
		return Response<void>(true, "Logout successful!");
	}
	return Response<void>(true, "User not logged in!");
}

Response<void> System::registerUser(User* user)
{
	if (!this->currentUser ||
		this->currentUser->getRole() != UserRole::Admin)
	{
		delete user;
		return Response<void>(false, "Access denied!");
	}
	if (user->getRole() == UserRole::Admin)
	{
		delete user;
		return Response<void>(false, "Cannot add admin!");
	}
	
	this->users.push_back(user);
	this->updateFile();
	
	return Response<void>(true, "User added successfully");
}

Response<void> System::removeUser(size_t id)
{
	if (!this->currentUser ||
		this->currentUser->getRole() != UserRole::Admin)
	{
		return Response<void>(false, "Access denied!");
	}

	if (this->currentUser->getId() == id)
	{
		return Response<void>(false, "Can't remove current user!");
	}

	size_t usersCount = this->users.size();
	for (size_t i = 0; i < usersCount; i++)
	{
		if (this->users[i]->getId() == id)
		{
			this->users.remove(i);
			this->updateFile();
			return Response<void>(true, "User successfully removed!");
		}
	}

	return Response<void>(false, "Invalid id!");
}

void System::clearUsers()
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		delete this->users[i];
	}
	this->users.clear();

	// currentUser is owned by users, do not delete separately
	this->currentUser = nullptr;
}

void System::ensureAdminCreated()
{
	size_t usersCount = this->users.size();
	for (size_t i = 0; i < usersCount; i++)
	{
		if (this->users[i] && this->users[i]->getRole() == UserRole::Admin)
		{
			return;
		}
	}

	Admin* admin = new Admin(0, "admin", "", "admin@email.com", "0000");
	this->users.insert(0, admin);
	this->updateFile();
}

size_t System::getNextId()
{
	size_t lastId = this->users[this->users.size() - 1]->getId();
	return lastId == 0 ? 100 : lastId + 1;
}

void System::loginHandler(const Vector<String>& input)
{
	if (input.size() != System::LOGIN_INPUT_SIZE)
	{
		std::cout << "Invalid input!\n";
		return;
	}

	Response<void> res = this->login(input[System::LOGIN_ID_INDEX].toNumber(), 
		input[System::LOGIN_PASSWORD_INDEX]);
	if (!res.success)
	{
		std::cout << res.message << '\n';
		return;
	}
	UserRole role = this->currentUser->getRole();
	if (role == UserRole::Admin)
	{
		std::cout << res.message << '\n';
		return;
	}
	std::cout << this->currentUser->getFirstName() << " " <<
		this->currentUser->getLastName() << " | ";
	if (role == UserRole::Teacher)
	{
		std::cout << "Teacher | ";
	}
	else if (role == UserRole::Student)
	{
		std::cout << "Student | ";
	}
	std::cout << this->currentUser->getId() << '\n';
}

void System::logoutHandler(const Vector<String>& input)
{
	if (input.size() != System::LOGOUT_INPUT_SIZE)
	{
		std::cout << "Invalid input!\n";
		return;
	}

	Response<void> res = this->logout();
	std::cout << res.message << '\n';
}

void System::addTeacherHandler(const Vector<String>& input)
{
	if (input.size() != System::ADD_USER_INPUT_SIZE)
	{
		std::cout << "Invalid input!\n";
		return;
	}

	User* user = new Teacher(
		this->getNextId(),
		input[1],
		input[2],
		"",
		input[3]);
	Response<void> res = this->registerUser(user);
	if (!res.success)
	{
		std::cout << res.message << '\n';
		return;
	}
	std::cout << "Added teacher " << user->getFirstName() << " " <<
		user->getLastName() << " with ID " << user->getId() << "!\n";
}

void System::addStudentHandler(const Vector<String>& input)
{
	if (input.size() != System::ADD_USER_INPUT_SIZE)
	{
		std::cout << "Invalid input!\n";
		return;
	}

	User* user = new Student(
		this->getNextId(),
		input[1],
		input[2],
		"",
		input[3]);
	Response<void> res = this->registerUser(user);
	if (!res.success)
	{
		std::cout << res.message << '\n';
		return;
	}
	std::cout << "Added student " << user->getFirstName() << " " <<
		user->getLastName() << " with ID " << user->getId() << "!\n";
}

void System::removeHandler(const Vector<String>& input)
{
	Response<void> res = this->removeUser(input[1].toNumber());
	std::cout << res.message << '\n';
}
