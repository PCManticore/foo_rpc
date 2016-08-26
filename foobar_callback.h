#pragma once
#include "stdafx.h"

namespace {
	class console_debug_callback : public main_thread_callback {
	public:
		void callback_run() {
			console::formatter() << debugMessage.c_str();
		}

		console_debug_callback(std::string message) : debugMessage(message) {}
	private:
		std::string debugMessage;
	};
};
