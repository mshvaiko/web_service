#include <iostream>

#include "web_server.h"

int main() {
	std::cout << "hello" << std::endl;

	server::web_server srv("127.0.0.1", 8080);
	auto res = srv.init();

	if (res) {
		srv.run();
	} else
	{
		std::cout << "Can't run\n";
	}
	
	return 0;
}
