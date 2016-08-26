#include <string>
#include "logging.h"
#include "foobar_callback.h"

/*Send a message to the Foobar console. */
void logToFoobarConsole(std::string message) {
	static_api_ptr_t<main_thread_callback_manager>()->add_callback(
		new service_impl_t<console_debug_callback>(message)
	);
}