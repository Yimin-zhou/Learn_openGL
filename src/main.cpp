#include "core/application.h"
#include "util/config.h"

int main(void)
{
	std::cout << "Current Path: " << std::filesystem::current_path() << std::endl;
	application app(config::windowWidth, config::windowHeight, config::windowName);
	app.run();
	return 0;
}