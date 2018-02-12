#include "_2dTestApp.h"

int main() {
	
	// allocation
	auto app = new _2dTestApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}