#include <stdio.h>
#include <algorithm>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Logger.h"

using namespace std;
using namespace cpplogger;

using ::testing::Return;
using ::testing::_;

int main(int argc, char *argv[])
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

class LoggerTest : public ::testing::Test
{
public:
	
	Logger *logger = NULL;

    virtual void SetUp()
    {
		logger = new Logger();
    }

    virtual void TearDown()
    {	
		delete logger;
    }
};

bool is_file_exist(const char *fileName)
{
	return (access(fileName, F_OK) != -1);
}

//TEST: Init
TEST_F(LoggerTest, Test_Init_0ss1_N)
{
	string aplLogFile = "/home/ec2-user/repos/cpplogger/logs/apl_test_init_01_n.log";
	string dbgLogFile = "/home/ec2-user/repos/cpplogger/logs/debug_test_init_01_n.log";
	string evntLogFile = "/home/ec2-user/repos/cpplogger/logs/event_test_init_01_n.log";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to true
	Logger::EnableFileLogging(true);
	// Set console logging to false
	Logger::EnableConsoleLogging(false);
	// Set logging severity level to INFO
	Logger::SetLogSeverityLevel(INFO);

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");
	// INFO level
	Logger::Info(LOGGER_CODE_INFO_DEFAULT, L"Writing information level logging");
	// ERROR level
	Logger::Error(LOGGER_CODE_ERRR_DEFAULT, L"Writing error level logging");
	// Critical level
	Logger::Crit(LOGGER_CODE_CRIT_DEFAULT, L"Writing critical level logging");
	// WARN level
	Logger::Warn(LOGGER_CODE_WARN_DEFAULT, L"Writing warning level logging");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
}

TEST_F(LoggerTest, Test_Init_02_N)
{
	string aplLogFile = "/home/ec2-user/repos/cpplogger/logs/apl_test_init_02_n.log";
	string dbgLogFile = "/home/ec2-user/repos/cpplogger/logs/debug_test_init_02_n.log";
	string evntLogFile = "/home/ec2-user/repos/cpplogger/logs/event_test_init_02_n.log";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to true
	Logger::EnableFileLogging(true);
	// Set console logging to false
	Logger::EnableConsoleLogging(false);
	// Set logging severity level to DEBUG
	Logger::SetLogSeverityLevel(DEBUG);

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");
	// INFO level
	Logger::Info(LOGGER_CODE_INFO_DEFAULT, L"Writing information level logging");
	// ERROR level
	Logger::Error(LOGGER_CODE_ERRR_DEFAULT, L"Writing error level logging");
	// Critical level
	Logger::Crit(LOGGER_CODE_CRIT_DEFAULT, L"Writing critical level logging");
	// WARN level
	Logger::Warn(LOGGER_CODE_WARN_DEFAULT, L"Writing warning level logging");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
}

TEST_F(LoggerTest, Test_Init_03_N)
{
	string aplLogFile = "/home/ec2-user/repos/cpplogger/logs/apl_test_init_03_n.log";
	string dbgLogFile = "/home/ec2-user/repos/cpplogger/logs/debug_test_init_03_n.log";
	string evntLogFile = "/home/ec2-user/repos/cpplogger/logs/event_test_init_03_n.log";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to true
	Logger::EnableFileLogging(true);
	// Set console logging to false
	Logger::EnableConsoleLogging(false);
	// Set logging severity level to ERROR
	Logger::SetLogSeverityLevel(ERROR);

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");
	// INFO level
	Logger::Info(LOGGER_CODE_INFO_DEFAULT, L"Writing information level logging");
	// ERROR level
	Logger::Error(LOGGER_CODE_ERRR_DEFAULT, L"Writing error level logging");
	// Critical level
	Logger::Crit(LOGGER_CODE_CRIT_DEFAULT, L"Writing critical level logging");
	// WARN level
	Logger::Warn(LOGGER_CODE_WARN_DEFAULT, L"Writing warning level logging");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
}

TEST_F(LoggerTest, Test_Init_04_N)
{
	string aplLogFile = "/home/ec2-user/repos/cpplogger/logs/apl_test_init_04_n.log";
	string dbgLogFile = "/home/ec2-user/repos/cpplogger/logs/debug_test_init_04_n.log";
	string evntLogFile = "/home/ec2-user/repos/cpplogger/logs/event_test_init_04_n.log";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to true
	Logger::EnableFileLogging(true);
	// Set console logging to false
	Logger::EnableConsoleLogging(false);
	// Set logging severity level to CRITICAL
	Logger::SetLogSeverityLevel(CRITICAL);

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");
	// INFO level
	Logger::Info(LOGGER_CODE_INFO_DEFAULT, L"Writing information level logging");
	// ERROR level
	Logger::Error(LOGGER_CODE_ERRR_DEFAULT, L"Writing error level logging");
	// Critical level
	Logger::Crit(LOGGER_CODE_CRIT_DEFAULT, L"Writing critical level logging");
	// WARN level
	Logger::Warn(LOGGER_CODE_WARN_DEFAULT, L"Writing warning level logging");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
}

TEST_F(LoggerTest, Test_Init_05_N)
{
	string aplLogFile = "/home/ec2-user/repos/cpplogger/logs/apl_test_init_05_n.log";
	string dbgLogFile = "/home/ec2-user/repos/cpplogger/logs/debug_test_init_05_n.log";
	string evntLogFile = "/home/ec2-user/repos/cpplogger/logs/event_test_init_05_n.log";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to true
	Logger::EnableFileLogging(true);
	// Set console logging to false
	Logger::EnableConsoleLogging(false);
	// Set logging severity level to WARNING
	Logger::SetLogSeverityLevel(WARNING);

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");
	// INFO level
	Logger::Info(LOGGER_CODE_INFO_DEFAULT, L"Writing information level logging");
	// ERROR level
	Logger::Error(LOGGER_CODE_ERRR_DEFAULT, L"Writing error level logging");
	// Critical level
	Logger::Crit(LOGGER_CODE_CRIT_DEFAULT, L"Writing critical level logging");
	// WARN level
	Logger::Warn(LOGGER_CODE_WARN_DEFAULT, L"Writing warning level logging");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
}

TEST_F(LoggerTest, Test_Init_06_N)
{
	string aplLogFile = "/home/ec2-user/repos/cpplogger/logs/apl_test_init_06_n.log";
	string dbgLogFile = "/home/ec2-user/repos/cpplogger/logs/debug_test_init_06_n.log";
	string evntLogFile = "/home/ec2-user/repos/cpplogger/logs/event_test_init_06_n.log";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to true
	Logger::EnableFileLogging(true);
	// Set console logging to true
	Logger::EnableConsoleLogging(true);
	// Set logging severity level to INFO
	Logger::SetLogSeverityLevel(INFO);

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");
	// INFO level
	Logger::Info(LOGGER_CODE_INFO_DEFAULT, L"Writing information level logging");
	// ERROR level
	Logger::Error(LOGGER_CODE_ERRR_DEFAULT, L"Writing error level logging");
	// Critical level
	Logger::Crit(LOGGER_CODE_CRIT_DEFAULT, L"Writing critical level logging");
	// WARN level
	Logger::Warn(LOGGER_CODE_WARN_DEFAULT, L"Writing warning level logging");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
}

TEST_F(LoggerTest, Test_Init_07_N)
{
	string aplLogFile = "/home/ec2-user/repos/cpplogger/logs/apl_test_init_07_n.log";
	string dbgLogFile = "/home/ec2-user/repos/cpplogger/logs/debug_test_init_07_n.log";
	string evntLogFile = "/home/ec2-user/repos/cpplogger/logs/event_test_init_07_n.log";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to true
	Logger::EnableFileLogging(true);
	// Set console logging to true
	Logger::EnableConsoleLogging(true);
	// Set logging severity level to INFO
	Logger::SetLogSeverityLevel(INFO);
	
	// Disable Application logging
	Logger::EnableAplLogging(false);

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");
	// INFO level
	Logger::Info(LOGGER_CODE_INFO_DEFAULT, L"Writing information level logging");
	// ERROR level
	Logger::Error(LOGGER_CODE_ERRR_DEFAULT, L"Writing error level logging");
	// Critical level
	Logger::Crit(LOGGER_CODE_CRIT_DEFAULT, L"Writing critical level logging");
	// WARN level
	Logger::Warn(LOGGER_CODE_WARN_DEFAULT, L"Writing warning level logging");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_FALSE(is_file_exist(aplLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
}

TEST_F(LoggerTest, Test_Init_08_N)
{
	string aplLogFile = "/home/ec2-user/repos/cpplogger/logs/apl_test_init_08n.log";
	string dbgLogFile = "/home/ec2-user/repos/cpplogger/logs/debug_test_init_08_n.log";
	string evntLogFile = "/home/ec2-user/repos/cpplogger/logs/event_test_init_08_n.log";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to true
	Logger::EnableFileLogging(true);
	// Set console logging to true
	Logger::EnableConsoleLogging(true);
	// Set logging severity level to INFO
	Logger::SetLogSeverityLevel(INFO);
	
	// Disable debug logging
	Logger::EnableDbgLogging(false);

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");
	// INFO level
	Logger::Info(LOGGER_CODE_INFO_DEFAULT, L"Writing information level logging");
	// ERROR level
	Logger::Error(LOGGER_CODE_ERRR_DEFAULT, L"Writing error level logging");
	// Critical level
	Logger::Crit(LOGGER_CODE_CRIT_DEFAULT, L"Writing critical level logging");
	// WARN level
	Logger::Warn(LOGGER_CODE_WARN_DEFAULT, L"Writing warning level logging");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
	EXPECT_FALSE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
}

TEST_F(LoggerTest, Test_Init_09_N)
{
	string aplLogFile = "/home/ec2-user/repos/cpplogger/logs/apl_test_init_09_n.log";
	string dbgLogFile = "/home/ec2-user/repos/cpplogger/logs/debug_test_init_09_n.log";
	string evntLogFile = "/home/ec2-user/repos/cpplogger/logs/event_test_init_09_n.log";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to true
	Logger::EnableFileLogging(true);
	// Set console logging to true
	Logger::EnableConsoleLogging(true);
	// Set logging severity level to INFO
	Logger::SetLogSeverityLevel(INFO);
	
	// Disable event logging
	Logger::EnableEvntLogging(false);

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");
	// INFO level
	Logger::Info(LOGGER_CODE_INFO_DEFAULT, L"Writing information level logging");
	// ERROR level
	Logger::Error(LOGGER_CODE_ERRR_DEFAULT, L"Writing error level logging");
	// Critical level
	Logger::Crit(LOGGER_CODE_CRIT_DEFAULT, L"Writing critical level logging");
	// WARN level
	Logger::Warn(LOGGER_CODE_WARN_DEFAULT, L"Writing warning level logging");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_FALSE(is_file_exist(evntLogFile.c_str()));
}

TEST_F(LoggerTest, Test_Init_10_N)
{
	string aplLogFile = "/home/ec2-user/repos/cpplogger/logs/apl_test_init_10_n.log";
	string dbgLogFile = "/home/ec2-user/repos/cpplogger/logs/debug_test_init_10_n.log";
	string evntLogFile = "/home/ec2-user/repos/cpplogger/logs/event_test_init_10_n.log";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to false
	Logger::EnableFileLogging(false);
	// Set console logging to true
	Logger::EnableConsoleLogging(true);
	// Set logging severity level to INFO
	Logger::SetLogSeverityLevel(INFO);	

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");
	// INFO level
	Logger::Info(LOGGER_CODE_INFO_DEFAULT, L"Writing information level logging");
	// ERROR level
	Logger::Error(LOGGER_CODE_ERRR_DEFAULT, L"Writing error level logging");
	// Critical level
	Logger::Crit(LOGGER_CODE_CRIT_DEFAULT, L"Writing critical level logging");
	// WARN level
	Logger::Warn(LOGGER_CODE_WARN_DEFAULT, L"Writing warning level logging");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_FALSE(is_file_exist(aplLogFile.c_str()));
	EXPECT_FALSE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_FALSE(is_file_exist(evntLogFile.c_str()));
}

TEST_F(LoggerTest, Test_Init_11_N)
{
	string aplLogFile = "/home/ec2-user/repos/cpplogger/logs/apl_test_init_11_n.log";
	string dbgLogFile = "/home/ec2-user/repos/cpplogger/logs/debug_test_init_11_n.log";
	string evntLogFile = "/home/ec2-user/repos/cpplogger/logs/event_test_init_11_n.log";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to false
	Logger::EnableFileLogging(false);
	// Set console logging to true
	Logger::EnableConsoleLogging(true);
	// Set logging severity level to INFO
	Logger::SetLogSeverityLevel((SeverityLevel) 12);	

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");
	// INFO level
	Logger::Info(LOGGER_CODE_INFO_DEFAULT, L"Writing information level logging");
	// ERROR level
	Logger::Error(LOGGER_CODE_ERRR_DEFAULT, L"Writing error level logging");
	// Critical level
	Logger::Crit(LOGGER_CODE_CRIT_DEFAULT, L"Writing critical level logging");
	// WARN level
	Logger::Warn(LOGGER_CODE_WARN_DEFAULT, L"Writing warning level logging");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_FALSE(is_file_exist(aplLogFile.c_str()));
	EXPECT_FALSE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_FALSE(is_file_exist(evntLogFile.c_str()));
}

TEST_F(LoggerTest, Test_Init_12_N)
{
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
}

//TEST: Init -- Default path validation
TEST_F(LoggerTest, Test_Init_012_N)
{
	/*
	 * If there is application/event/debug file names are specified,
	 * default path shall be set.
	 * Default paths are:
	 * Application log file:	"/var/log/cpplogger/apl.log"
	 * Debug log file:			"/var/log/cpplogger/debug.log"
	 * Event log file:			"/var/log/cpplogger/event.log"
     * Important:
	 * Make sure the  directory "/var/log/cpplogger/" exist and read/write permissions are set.
	 */
	string aplLogFile = "";
	string dbgLogFile = "";
	string evntLogFile = "";

	// Initialize the logger with log (application, event, debug) file paths
	Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

	// Set file logging to true
	Logger::EnableFileLogging(true);
	// Set console logging to false
	Logger::EnableConsoleLogging(false);
	// Set logging severity level to INFO
	Logger::SetLogSeverityLevel(INFO);

	// INFO level
	Logger::Info(L"Writing information level logging");
	// ERROR level
	Logger::Error(L"Writing error level logging");
	// Critical level
	Logger::Crit(L"Writing critical level logging");
	// WARN level
	Logger::Warn(L"Writing warning level logging");
	// DEBUG level 
	Logger::Debug(L"Writing debug level logging");
	// EVENT level
	Logger::Event(L"Event: {application has started}");

	// Release and close all loggers
	Logger::DropAll();
	
	EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
	EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
}

//TEST: Init -- invalid application log file path 
TEST_F(LoggerTest, Test_Init_01_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger1/apl.log";
		string dbgLogFile = "/var/log/cpplogger/debug.log";
		string evntLogFile = "/var/log/cpplogger/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);
		// Release and close all loggers
		Logger::DropAll();
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate application log file directory (/var/log/cpplogger1) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate application log file directory (/var/log/cpplogger1) permissions";
    }
}

//TEST: Init -- invalid debug log file path 
TEST_F(LoggerTest, Test_Init_02_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger/apl.log";
		string dbgLogFile = "/var/log/cpplogger1/debug.log";
		string evntLogFile = "/var/log/cpplogger/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);
		// Release and close all loggers
		Logger::DropAll();
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate debug log file directory (/var/log/cpplogger1) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate debug log file directory (/var/log/cpplogger1) permissions";
    }
}

//TEST: Init -- invalid event log file path 
TEST_F(LoggerTest, Test_Init_03_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger/apl.log";
		string dbgLogFile = "/var/log/cpplogger/debug.log";
		string evntLogFile = "/var/log/cpplogger1/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);
		// Release and close all loggers
		Logger::DropAll();
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate event log file directory (/var/log/cpplogger1) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate event log file directory (/var/log/cpplogger1) permissions";
    }
}

//TEST: Init -- no read permissions to application log directory
TEST_F(LoggerTest, Test_Init_04_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger2/apl.log";
		string dbgLogFile = "/var/log/cpplogger/debug.log";
		string evntLogFile = "/var/log/cpplogger/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);
		// Release and close all loggers
		Logger::DropAll();
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate application log file directory (/var/log/cpplogger2) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate application log file directory (/var/log/cpplogger2) permissions";
    }
}

//TEST: Init -- no read permissions to debug log directory
TEST_F(LoggerTest, Test_Init_05_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger/apl.log";
		string dbgLogFile = "/var/log/cpplogger2/debug.log";
		string evntLogFile = "/var/log/cpplogger/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);
		// Release and close all loggers
		Logger::DropAll();
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate debug log file directory (/var/log/cpplogger2) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate debug log file directory (/var/log/cpplogger2) permissions";
    }
}

//TEST: Init -- no read permissions to event log directory
TEST_F(LoggerTest, Test_Init_06_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger/apl.log";
		string dbgLogFile = "/var/log/cpplogger/debug.log";
		string evntLogFile = "/var/log/cpplogger2/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

		// Set file logging to true
		Logger::EnableFileLogging(true);
		// Set console logging to false
		Logger::EnableConsoleLogging(false);
		// Set logging severity level to INFO
		Logger::SetLogSeverityLevel(INFO);

		// INFO level
		Logger::Info(L"Writing information level logging");
		// ERROR level
		Logger::Error(L"Writing error level logging");
		// Critical level
		Logger::Crit(L"Writing critical level logging");
		// WARN level
		Logger::Warn(L"Writing warning level logging");
		// DEBUG level 
		Logger::Debug(L"Writing debug level logging");
		// EVENT level
		Logger::Event(L"Event: {application has started}");

		// Release and close all loggers
		Logger::DropAll();
		
		EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
		EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
		EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate event log file directory (/var/log/cpplogger2) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate event log file directory (/var/log/cpplogger2) permissions";
    }
}

//TEST: Init -- no read permissions to application log directory
TEST_F(LoggerTest, Test_Init_07_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger_no_write/apl.log";
		string dbgLogFile = "/var/log/cpplogger/debug.log";
		string evntLogFile = "/var/log/cpplogger/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);
		// Release and close all loggers
		Logger::DropAll();
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate application log file directory (/var/log/cpplogger_no_write) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate application log file directory (/var/log/cpplogger_no_write) permissions";
    }
}

//TEST: Init -- no read permissions to debug log directory
TEST_F(LoggerTest, Test_Init_08_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger/apl.log";
		string dbgLogFile = "/var/log/cpplogger_no_write/debug.log";
		string evntLogFile = "/var/log/cpplogger/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);
		// Release and close all loggers
		Logger::DropAll();
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate debug log file directory (/var/log/cpplogger_no_write) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate debug log file directory (/var/log/cpplogger_no_write) permissions";
    }
}

//TEST: Init -- no read permissions to event log directory
TEST_F(LoggerTest, Test_Init_09_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger/apl.log";
		string dbgLogFile = "/var/log/cpplogger/debug.log";
		string evntLogFile = "/var/log/cpplogger_no_write/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

		// Release and close all loggers
		Logger::DropAll();
		
		EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
		EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
		EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate event log file directory (/var/log/cpplogger_no_write) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate event log file directory (/var/log/cpplogger_no_write) permissions";
    }
}

//TEST: Init -- no read permissions to application log file
TEST_F(LoggerTest, Test_Init_10_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger_file_noaccess/apl.log";
		string dbgLogFile = "/var/log/cpplogger/debug.log";
		string evntLogFile = "/var/log/cpplogger/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);
		// Release and close all loggers
		Logger::DropAll();
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate application log file (/var/log/cpplogger_file_noaccess/apl.log) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate application log file (/var/log/cpplogger_file_noaccess/apl.log) permissions";
    }
}

//TEST: Init -- no read permissions to debug log file
TEST_F(LoggerTest, Test_Init_11_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger/apl.log";
		string dbgLogFile = "/var/log/cpplogger_file_noaccess/debug.log";
		string evntLogFile = "/var/log/cpplogger/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);
		// Release and close all loggers
		Logger::DropAll();
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate debug log file (/var/log/cpplogger_file_noaccess/debug.log) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate debug log file (/var/log/cpplogger_file_noaccess/debug.log) permissions";
    }
}

//TEST: Init -- no read permissions to event log file
TEST_F(LoggerTest, Test_Init_12_A)
{	
    try {
		string aplLogFile = "/var/log/cpplogger/apl.log";
		string dbgLogFile = "/var/log/cpplogger/debug.log";
		string evntLogFile = "/var/log/cpplogger_file_noaccess/event.log";

		// Initialize the logger with log (application, event, debug) file paths
		Logger::Init(aplLogFile, dbgLogFile, evntLogFile);

		// Release and close all loggers
		Logger::DropAll();
		
		EXPECT_TRUE(is_file_exist(aplLogFile.c_str()));
		EXPECT_TRUE(is_file_exist(dbgLogFile.c_str()));
		EXPECT_TRUE(is_file_exist(evntLogFile.c_str()));
	} catch (LoggerException& le) {
        EXPECT_EQ(le.GetMsg(),std::string("Logger::Init() failed to validate event log file (/var/log/cpplogger_file_noaccess/event.log) permissions"));
    } catch(...) {
        FAIL() << "Expected Logger::Init() failed to validate event log file (/var/log/cpplogger_file_noaccess/event.log) permissions";
    }
}