//////////////////////////////////////////////////////////////////////////////
// @File Name:      Main.cpp                                                //
// @Author:         Prajilal KP                                             //
// @Version:        0.0.1                                                   //
// @L.M.D:          27th April 2018                                         //
// @Description:    Test Class                                              //
//                                                                          // 
// Detail Description:                                                      //
// Test file for Logger.                                                    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Logger.h"

using namespace std;
using namespace cpplogger;

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