#pragma once

#include "User.h"

class Admin : public User
{
public:
	Admin(size_t id, const String& fName, const String& lName,
		const String& email, const String& pwd);

	UserRole getRole() const override;

	std::ofstream& serialize(std::ofstream& os) const override;
	std::ofstream& deserialize(std::ofstream& os) override;
};
