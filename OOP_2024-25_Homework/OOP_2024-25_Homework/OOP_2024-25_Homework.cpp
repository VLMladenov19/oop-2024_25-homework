#include <iostream>

#include "System.h"
#include "String.h"
#include "Response.h"
#include "User.h"

int main()
{
	System sys;

	sys.initialize();
	
	// sys.loop();

	sys.finalize();
}