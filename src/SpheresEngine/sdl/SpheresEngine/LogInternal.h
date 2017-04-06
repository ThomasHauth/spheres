#pragma once

#include <SpheresEngine/LogMessageLevel.h>

#include <iostream>

#define SPHERES_LOG_FILENAME "descent.log"

namespace logging {

/**
 * Logging on SDL platform is simply using iostream to write on the console
 */
inline void _internal_log(std::string message, MessageLevel) {
	std::cout << "[Thread " << std::this_thread::get_id() << "] " << message
			<< std::endl;
	std::ofstream outFile(SPHERES_LOG_FILENAME, std::ios::app);
	outFile << message << std::endl;
	outFile.close();
}

/**
 * Truncate the logfile to set the content to 0
 */
inline void resetLogFile() {
	// will empty the file
	std::ofstream outFile(SPHERES_LOG_FILENAME, std::ios::trunc);
	outFile.close();
}

}
