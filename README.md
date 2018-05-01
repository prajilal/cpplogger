# cpplogger
Fast, simple, C++ logging library. 

## Why another logging library?
I have yet to come across a nice, light-weight logging library for C++ that does everything I want. So I made one!

In particular, I want logging that produces different log files against different severity levels that are both human-readable and easily grep:ed. I also want logging process to print log messages to terminal (if enabled).

## Install
Copy the source files (Logger.cpp & Logger.h) to your build tree and use a C++11 compiler.

## Platform
Linux

## Features
- Very fast
- Feature rich call style
- Conditional Logging
  - Easily turn on/off various logging
  - Easily turn on/off File logging & console logging
- Multi threaded loggers
- CallLog functions. Easy method to write function start/end debug logs
- Various log targets:
  - Support multiple file outputs
	  - Application level log file (writes INFO, CRITICAL, ERROR and WARNING level logs to this file) 
	  - Debug level log file (writes DEBUG level logs to this file)
	  - Event level log file (writes EVENT level logs to this file)
  - Console logging (colors supported)
  - syslog
- Prefixes each log line with:
  - Date and time to millisecond precision
  - Log level
  - Custom message ID. User can define custom message ID for each message in addition to the default message IDs (See Message ID)
- grep:able logs:
  - Each line has all the info you need (e.g. date).
  - You can easily filter out high verbosity levels after the fact.

## Compiling
Just include <Logger.h> where you want to use cpplogger. Then, in one .cpp file:

	#include <Logger.h>

Make sure you compile with -std=c++11 -lpthread on relevant environments.

#### Usage Example
```
#include <iostream>
#include "Logger.h"

using namespace std;
using namespace common;
using namespace common::log;

void CallLogExampleOne();
void CallLogExampleTwo(int val);
void CallLogExampleThree();

int main(void)
{
	try {

		string aplLogFile = "apl.log";
		string dbgLogFile = "debug.log";
		string evntLogFile = "event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

		// Set file logging to true
		Logger::EnableFileLogging(true);
		// Set console logging to true
		Logger::EnableConsoleLogging(true);
		// Set logging severity level to INFO
		Logger::SetLogSeverityLevel(INFO);

		// Application startup log
		Logger::Info(LOGGER_CODE_INFO_APP_START, L"Starting the application...");

		// INFO level
		Logger::Info(L"Information level logging (%u)", 1);
		// ERROR level
		Logger::Error(L"Error level logging (%u)", 1);
		// Critical level
		Logger::Crit(L"Critical level logging (%u)", 1);
		// WARN level
		Logger::Warn(L"Warning level logging (%u)", 1);
		// DEBUG level 
		Logger::Debug(L"Debug level logging (%u)", 1);

		// EVENT level
		Logger::Event(L"Event: {application has started}");

		//syslog() examples
		// INFO level
		Logger::SysLogInfo(L"syslog() information level logging");
		// ERROR level
		Logger::SysLogError(L"syslog() error level logging");
		// Critical level
		Logger::SysLogCrit(L"syslog() critical level logging");
		// WARN level
		Logger::SysLogWarn(L"syslog() warning level logging");
		// DEBUG level 
		Logger::SysLogDebug(L"syslog() debug level logging");

		// Application end log
		Logger::Info(LOGGER_CODE_INFO_APP_STOP, L"Stopping the application...");

		//CallLog example
		CallLogExampleOne();
		CallLogExampleTwo(25);
		CallLogExampleThree();

		// Release and close all loggers
		
		try {
			Logger::DropAll();
		} catch (LoggerException& le) {
			Logger::SysLogError(L"%s", le.GetMsg());
		}

	} catch (LoggerException& le) {
		// Log exception and exit
		Logger::SysLogCrit(L"Failed to start (%s)", le.GetMsg());
		try {
			Logger::DropAll();
		} catch (LoggerException& le) {
			Logger::SysLogError(L"%s", le.GetMsg());
		}
		exit(EXIT_FAILURE);
	} catch (std::exception &e) {
		// Log exception and exit
		Logger::SysLogCrit(L"Failed to start (%s)", e.what());
		try {
			Logger::DropAll();
		} catch (LoggerException& le) {
			Logger::SysLogError(L"%s", le.GetMsg());
		}
		exit(EXIT_FAILURE);
	} catch (...) {
		// Log exception and exit
		Logger::SysLogCrit(L"Failed to start (unknown exception)");
		try {
			Logger::DropAll();
		} catch (LoggerException& le) {
			Logger::SysLogError(L"%s", le.GetMsg());
		}
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

/*
 * CallLogExampleOne()
 */
void CallLogExampleOne() { 
	CallLog(L"%s::%s()", __FILE__, __func__);
	Logger::Event(L"CallLogExampleOne() has executed");
}

/*
* CallLogExampleTwo()
*/
void CallLogExampleTwo(int val)
{
	CallLog(L"%s::%s(val= %d)", __FILE__, __func__, val);
	Logger::Event(L"CallLogExampleTwo() has executed");
}

/*
* CallLogExampleThree()
*/
void CallLogExampleThree()
{
	CallLog log(__FILE__, __LINE__, __func__);
	Logger::Event(L"CallLogExampleThree() has executed");
}
```

## Grep:able logs

```
# Only show information messages:
cat apl.log | egrep "INFO"

# Only show error messages:
cat apl.log | egrep "ERROR"

# Only show warning messages:
cat apl.log | egrep "WARN"

# Only show critical messages:
cat apl.log | egrep "CRIT"
```

## Message ID
Defines an ID for each log message. Available IDs are:

```
// Information level message code
#define LOGGER_CODE_INFO_DEFAULT    		  00001
#define LOGGER_CODE_INFO_APP_START    		00002
#define LOGGER_CODE_INFO_APP_STOP    		  00003

// Error level message code
#define LOGGER_CODE_ERRR_DEFAULT    		  00001
#define LOGGER_CODE_ERRR_APP_START	 		  00002
#define LOGGER_CODE_ERRR_APP_STOP	 		    00003

// Warning level message code
#define LOGGER_CODE_WARN_DEFAULT	 		    00001
#define LOGGER_CODE_WARN_APP_START	 		  00002
#define LOGGER_CODE_WARN_APP_STOP	 		    00003

// Critical level message code
#define LOGGER_CODE_CRIT_DEFAULT	    	  00001
#define LOGGER_CODE_CRIT_APP_START	    	00002
#define LOGGER_CODE_CRIT_APP_STOP	    	  00003
```

#### Usage Example
```
Logger::Info(LOGGER_CODE_INFO_APP_START, L"Starting the application...");
Logger::Error(LOGGER_CODE_ERRR_APP_START, L"Error occured when starting the application");
Logger::Crit(LOGGER_CODE_CRIT_APP_STOP, L"Application failed to stop");
Logger::Warn(LOGGER_CODE_WARN_APP_STOP, L"Error occured when stopping the application");
```

User can define more message IDs in Logger.h file. 

# Documentation
Documentation can be found in the cpplogger/documentation/html/index.html.

# Limitations and TODO
- Rotating log files.
- Daily log files.
