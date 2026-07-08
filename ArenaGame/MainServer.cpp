#include <boost/asio.hpp>
#include "PongServer.h"

int main() {
	::boost::asio::io_context io;
	PongServer server(io, 7777);
	io.run();
	return 0;
}