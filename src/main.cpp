#include "core/application.h"
#include "util/config.h"

int main(void)
{
	core::application app(windowWidth, windowHeight, windowName);
	app.run();
	return 0;
}