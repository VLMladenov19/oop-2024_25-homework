#include "Mail.h"

Mail::Mail()
    : sentOn_(), message_(), senderName_(), receiverId_()
{
}

Mail::Mail(const String& message, const String& senderName, size_t receiverId)
	: message_(message), senderName_(senderName), receiverId_(receiverId)
{
	this->sentOn_ = std::time(nullptr);
}

Mail::Mail(const Mail& other)
{
    copyFrom(other);
}

Mail& Mail::operator=(const Mail& other)
{
    if (this != &other)
    {
        copyFrom(other);
    }
    return *this;
}

std::time_t Mail::getSentOn() const
{
	return this->sentOn_;
}

const String& Mail::getMessage() const
{
	return this->message_;
}

const String& Mail::getSenderName() const
{
	return this->senderName_;
}

size_t Mail::getReceiverId() const
{
	return this->receiverId_;
}

std::ofstream& Mail::serialize(std::ofstream& os) const
{
	os << this->sentOn_ << '\n';
	os << this->senderName_ << '\n';
	os << this->receiverId_ << '\n';
	os << this->message_ << '\n';
	os << Mail::mailDelimeter << '\n';

	return os;
}

std::ifstream& Mail::deserialize(std::ifstream& is)
{
    String sentOnStr, sender, receiverIdStr, message, delimiter;

    if (!getline(is, sentOnStr) ||
        !getline(is, sender) ||
        !getline(is, receiverIdStr) ||
        !getline(is, message) ||
        !getline(is, delimiter))
    {
        return is;
    }

    if (delimiter != Mail::mailDelimeter)
    {
        return is;
    }

    this->sentOn_ = sentOnStr.toNumber();
    this->senderName_ = sender;
    this->receiverId_ = receiverIdStr.toNumber();
    this->message_ = message;

	return is;
}

void Mail::copyFrom(const Mail& other)
{
    this->sentOn_ = other.sentOn_;
    this->message_ = other.message_;
    this->senderName_ = other.senderName_;
    this->receiverId_ = other.receiverId_;
}
