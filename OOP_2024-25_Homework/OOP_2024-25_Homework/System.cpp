#include "System.h"
#include "Admin.h"

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
	}
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
		case UserRole::Teacher: return "Teacher";
			//user = new Teacher(0, "", "", "", "");
			break;
		case UserRole::Student: return "Student";
			//user = new Student(0, "", "", "", "");
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

Response<void> System::updateFile()
{
	return this->serializeUsers();
}

Response<void> System::updateUsers()
{
	return this->deserializeUsers();
}

void System::finalize()
{
	this->updateFile();
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

const Vector<User*>& System::getUsers() const
{
	return this->users;
}

User* System::getCurrentUser() const
{
	return this->currentUser;
}
