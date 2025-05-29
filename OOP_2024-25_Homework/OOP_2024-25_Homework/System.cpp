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
	// Add ensureAdminCreated method
	this->updateUsers();
}

Response<void> System::serialize()
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

Response<void> System::deserialize()
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

Response<void> System::updateFile()
{
	return this->serialize();
}

Response<void> System::updateUsers()
{
	return this->deserialize();
}

void System::finalize()
{
	this->updateFile();
}

Response<User*> System::getUserById(size_t id) const
{
	size_t usersCount = this->users.size();
	for (size_t i = 0; i < usersCount; i++)
	{
		if (this->users[i]->getId() == id)
			return Response<User*>(true, 
				"User found!", 
				this->users[i]);
	}

	return Response<User*>(false, "User not found!", nullptr);
}

Response<User*> System::getUserByEmail(const String& email) const
{
	size_t usersCount = this->users.size();
	for (size_t i = 0; i < usersCount; i++)
	{
		if (this->users[i]->getEmail() == email)
			return Response<User*>(true,
				"User found!",
				this->users[i]);
	}

	return Response<User*>(false, "User not found!", nullptr);
}

const Vector<User*>& System::getUsers() const
{
	return this->users;
}

User* System::getCurrentUser() const
{
	return this->currentUser;
}
