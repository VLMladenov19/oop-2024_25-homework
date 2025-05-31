#pragma once

#include <fstream>

#include "String.h"

class Mail
{
public:
	Mail();
	Mail(const String& message, const String& senderName, size_t receiverId);
	Mail(const Mail& other);
	Mail& operator=(const Mail& other);

	std::time_t getSentOn() const;
	const String& getMessage() const;
	const String& getSenderName() const;
	size_t getReceiverId() const;

	std::ofstream& serialize(std::ofstream& os) const;
	std::ifstream& deserialize(std::ifstream& os);

private:
	std::time_t sentOn_;
	String message_;
	String senderName_;
	size_t receiverId_;

	void copyFrom(const Mail& other);

	const String mailDelimeter = "|||MAIL_END|||";
};