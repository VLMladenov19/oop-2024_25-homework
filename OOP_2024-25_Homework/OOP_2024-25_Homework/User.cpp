#include "User.h"

User::User(size_t id, const String& fName, const String& lName, 
    const String& email, const String& pwd)
    : id_(id), firstName_(fName), lastName_(lName), email_(email), password_(pwd)
{
}

User::User(const User& other)
{
    this->copyFrom(other);
}

User& User::operator=(const User& other)
{
    if (this != &other)
    {
        this->copyFrom(other);
    }

    return *this;
}

User::User(User&& other) noexcept
    : id_(other.id_),
    firstName_(std::move(other.firstName_)),
    lastName_(std::move(other.lastName_)),
    email_(std::move(other.email_)),
    password_(std::move(other.password_))
{
    other.id_ = 0;
}

User& User::operator=(User&& other) noexcept
{
    if (this != &other)
    {
        this->id_ = other.id_;
        this->firstName_ = std::move(other.firstName_);
        this->lastName_ = std::move(other.lastName_);
        this->email_ = std::move(other.email_);
        this->password_ = std::move(other.password_);

        other.id_ = 0;
    }
    return *this;
}

UserRole User::getRole() const
{
    return UserRole::None;
}

size_t User::getId() const
{
    return this->id_;
}

const String& User::getFirstName() const
{
    return this->firstName_;
}

const String& User::getLastName() const
{
    return this->lastName_;
}

const String& User::getEmail() const
{
    return this->email_;
}

void User::setFirstName(const String& fName)
{
    this->firstName_ = fName;
}

void User::setLastName(const String& lName)
{
    this->lastName_ = lName;
}

void User::setEmail(const String& email)
{
    this->email_ = email;
}

bool User::checkPassword(const String& pwd) const
{
    return this->password_ == pwd;
}

void User::changePassword(const String& currPwd, const String& newPwd)
{
    if (this->password_ == currPwd)
    {
        this->password_ = newPwd;
    }
}

Response<void> User::sendMessage(const Mail& mail) const
{
    String fileName = String::toString(mail.getReceiverId());
    fileName += ".txt";
    std::ofstream os(fileName, std::ios::app);

    if (!os.is_open())
    {
        return Response<void>(false,
            String("Failed to open file for writing: ") + fileName);
    }

    mail.serialize(os);

    os.close();

    return Response<void>(true, "Mail written successful!");
}

std::ofstream& User::serialize(std::ofstream& os) const
{
    UserRole role = this->getRole();
    os.write((const char*)&role, sizeof(role));

    os.write((const char*)&this->id_, sizeof(this->id_));

    size_t fNameLen = this->firstName_.size();
    os.write((const char*)&fNameLen, sizeof(fNameLen));
    os.write((const char*)firstName_, fNameLen);

    size_t lNameLen = this->lastName_.size();
    os.write((const char*)&lNameLen, sizeof(lNameLen));
    os.write((const char*)lastName_, lNameLen);

    size_t emailLen = this->email_.size();
    os.write((const char*)&emailLen, sizeof(emailLen));
    os.write((const char*)email_, emailLen);

    size_t pwdLen = this->password_.size();
    os.write((const char*)&pwdLen, sizeof(pwdLen));
    os.write((const char*)password_, pwdLen);

    return os;
}

std::ifstream& User::deserialize(std::ifstream& is)
{
    // Id
    size_t id = 0;
    is.read((char*)&id, sizeof(id));
    this->setId(id);

    // First Name
    this->setFirstName(readString(is));

    // Last Name
    this->setLastName(readString(is));

    // Email
    this->setEmail(readString(is));

    // Password
    this->setPassword(readString(is));

    return is;
}

void User::setId(size_t id)
{
    this->id_ = id;
}

void User::setPassword(const String& pwd)
{
    this->password_ = pwd;
}

String User::readString(std::ifstream& is) const
{
    size_t len = 0;
    is.read((char*)&len, sizeof(len));

    char* buffer = new char[len + 1];
    is.read(buffer, len);
    buffer[len] = '\0';

    String result = buffer;
    delete[] buffer;

    return result;
}

void User::copyFrom(const User& other)
{
    this->id_ = other.id_;
    this->firstName_ = other.firstName_;
    this->lastName_ = other.lastName_;
    this->email_ = other.email_;
    this->password_ = other.password_;
}

String roleToString(UserRole role)
{
    switch (role)
    {
    case UserRole::Admin: return "Admin";
    case UserRole::Teacher: return "Teacher";
    case UserRole::Student: return "Student";
    default: return "None";
    }
}

UserRole stringToRole(const String& string)
{
    String lower = string.toLower();

    if (lower == "admin") return UserRole::Admin;
    if (lower == "teacher") return UserRole::Teacher;
    if (lower == "student") return UserRole::Student;

    return UserRole::None;
}
