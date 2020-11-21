#include <stardustxr/server/messengermanager.hpp>
#include <stardustxr/server/stardust_scenegraph.hpp>
using namespace StardustXR;

#include <stereokit.h>
using namespace sk;

int main() {
	// log_set_filter(log_diagnostic);
	sk_init("Stardust XR", runtime_mixedreality);

	ServerStardustScenegraph scenegraph;
	MessengerManager messengerManager(&scenegraph);

	while (sk_step([]() {

	})) {}

	sk_shutdown();
	return 0;
}
