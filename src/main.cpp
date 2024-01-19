#include "core/application.h"
#include "util/config.h"

int main(void)
{
	core::application app(config::windowWidth, config::windowHeight, config::windowName);
	app.run();
	return 0;
}