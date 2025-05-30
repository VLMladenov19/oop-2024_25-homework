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

		Vector<String> inputs = line.split();
		String command = inputs[System::COMMAND_INDEX];
		if (command == "login")
		{
			Response<User*> res = this->login(inputs[1].toNumber(), inputs[2]);
			std::cout << res.message << '\n';
			continue;
		}
		if (command == "logout")
		{
			Response<void> res = this->logout();
			std::cout << res.message << '\n';
			continue;
		}
		if (command == "add_teacher")
		{
			if (inputs.size() < 4)
			{
				std::cout << "Invalid input!\n";
				continue;
			}
			User* user = new Teacher(
				this->getNextId(), 
				inputs[1], 
				inputs[2], 
				"", 
				inputs[3]);
			Response<User*> res = this->registerUser(user);
			if (!res.success)
			{
				std::cout << res.message << '\n';
				continue;
			}
			std::cout << "Added teacher " << user->getFirstName() << " " <<
				user->getLastName() << " with ID " << user->getId() << "!\n";
			continue;
		}
		if (command == "add_student")
		{
			if (inputs.size() < 4)
			{
				std::cout << "Invalid input!\n";
				continue;
			}
			User* user = new Teacher(
				this->getNextId(), 
				inputs[1], 
				inputs[2], 
				"", 
				inputs[3]);
			Response<User*> res = this->registerUser(user);
			if (!res.success)
			{
				std::cout << res.message << '\n';
				continue;
			}
			std::cout << "Added student " << user->getFirstName() << " " <<
				user->getLastName() << " with ID " << user->getId() << "!\n";
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

Response<User*> System::login(size_t id, String pwd)
{
	if (this->currentUser)
	{
		return Response<User*>(false, "User already logged in", nullptr);
	}

	User* user = this->getUserById(id);

	if (!user)
	{
		return Response<User*>(false, "Invalid user id!", nullptr);
	}

	if (!user->checkPassword(pwd))
	{
		return Response<User*>(false, "Wrong password!", nullptr);
	}

	this->currentUser = user;
	return Response<User*>(true, "Login successful!", user);
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

Response<User*> System::registerUser(User* user)
{
	if (this->currentUser == nullptr ||
		this->currentUser->getRole() != UserRole::Admin)
	{
		delete user;
		return Response<User*>(false, "Access denied!", nullptr);
	}
	if (user->getRole() == UserRole::Admin)
	{
		delete user;
		return Response<User*>(false, "Cannot add admin!", nullptr);
	}
	
	this->users.push_back(user);
	this->updateFile();
	
	return Response<User*>(true, "User added successfully", user);
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
