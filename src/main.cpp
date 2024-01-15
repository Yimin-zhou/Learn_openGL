#include "core/application.h"

int main(void)
{
	core::application app(1920, 1080, "my renderer");
	app.run();
	return 0;
}