#pragma once

#include "String.h"

template<typename T>
class Response
{
public:
	Response(bool success, String message) 
		: success(success), message(message), object() {}
	Response(bool success, String message, const T& object)
		: success(success), message(message), object(object) {}

	bool success;
	String message;
	T object;
};