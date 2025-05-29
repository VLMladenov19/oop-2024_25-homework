#pragma once

#include "String.h"

template<typename T>
class Response
{
public:
	Response(bool success) 
		: success(success) {}
	Response(bool success, String message) 
		: success(success), message(message) {}
	Response(bool success, String message, const T& object)
		: success(success), message(message), object(object) {}

	bool success;
	String message;
	T object;
};

template<>
class Response<void>
{
public:
	Response(bool success)
		: success(success) {}
	Response(bool success, String message)
		: success(success), message(message) {}

	bool success;
	String message;
};