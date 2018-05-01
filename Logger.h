//////////////////////////////////////////////////////////////////////////////
// @File Name:      Logger.h                                                //
// @Author:         Prajilal KP                                             //
// @Version:        0.0.1                                                   //
// @L.M.D:          24th April 2018                                         //
// @Description:    For writing various level log information to different	// 
//                  files                                                   //
//                                                                          // 
// Detail Description:                                                      //
// Implemented complete logging mechanism, Supporting multiple logging type //
// like as file based logging, console base logging etc. It also supported  //
// for different log type.                                                  //
//                                                                          //
// Thread Safe logging mechanism. Compatible with G++ (Linux platform)      //
//                                                                          //
// Supported Log Type: ERROR, CRITICAL, DEBUG, INFO, WARN, EVENT            //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#ifndef _LOG_LOGGER_H_
#define _LOG_LOGGER_H_

#include <cstdarg>
#include <syslog.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <libgen.h>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <locale>
#include <sys/stat.h>

#define MAX_LEN_FMT_BUFFER			2048
#define MAX_LEN_STR_BUFFER			2080
#define MAX_LEN_DATE_BUFFER			32
#define SLEEP_IN_MS					100

#define LOCALE_DEFAULT				"en_US.UTF8"
#define APL_LOG_PATH_DEFAULT		"/var/log/cpplogger/apl.log"
#define DBG_LOG_PATH_DEFAULT		"/var/log/cpplogger/debug.log"
#define EVNT_LOG_PATH_DEFAULT		"/var/log/cpplogger/event.log"

// Information level message code
#define LOGGER_CODE_INFO_DEFAULT    		00001
#define LOGGER_CODE_INFO_APP_START    		00002
#define LOGGER_CODE_INFO_APP_STOP    		00003
//......and more

// Error level message code
#define LOGGER_CODE_ERRR_DEFAULT    		00001
#define LOGGER_CODE_ERRR_APP_START	 		00002
#define LOGGER_CODE_ERRR_APP_STOP	 		00003
//......and more

// Warning level message code
#define LOGGER_CODE_WARN_DEFAULT	 		00001
#define LOGGER_CODE_WARN_APP_START	 		00002
#define LOGGER_CODE_WARN_APP_STOP	 		00003
//......and more

// Critical level message code
#define LOGGER_CODE_CRIT_DEFAULT	    	00001
#define LOGGER_CODE_CRIT_APP_START	    	00002
#define LOGGER_CODE_CRIT_APP_STOP	    	00003
//......and more

using namespace std;

namespace cpplogger {

	/**
	 * @enum SeverityLevel
	 *
	 * @brief Enumerator which defined the different log severity levels. <br>
	 * Available log levels are:
	 *
	 * <b>INFO(0)</b>		<br>Information logging level.
	 *
	 * <b>EVENT(1)</b>		<br>Event logging level.
	 *
	 * <b>DEBUG(6)</b>		<br>Debug logging level.
	 *
	 * <b>WARNING(7)</b>	<br>Warnig logging level.
	 *
	 * <b>ERROR(8)</b>		<br>Error logging level.
	 *
	 * <b>CRITICAL(9)</b>	<br>Critical logging level.
	 */
	enum SeverityLevel
	{
		INFO = 0,
		EVENT = 1,
		DEBUG = 6,
		WARNING = 7,
		ERROR = 8,
		CRITICAL = 9
	};

	/**
	 * @enum LoggerExceptionType
	 *
	 * @brief Enumerator which defines the different logger exception types. <br>
	 * Available exception types are:
	 *
	 * <b>LOGGER_EXCEPTION_INIT(1)</b>		<br>Initialization type logger exception.<br>
	 *										Sets this exception type when logger initialization has failed.
	 *
	 * <b>LOGGER_EXCEPTION_STREAM(2)</b>	<br>Stream type logger exception.<br>
	 *										Sets this exception type when stream operation (open/read/write/close)
	 *										has failed.
	 *
	 * <b>LOGGER_EXCEPTION_PERMISSION(3)</b><br>Permission type logger exception.<br>
	 *										Sets this exception type when logger file/directory permissions has not
	 *										set properly.
	 *
	 * <b>LOGGER_EXCEPTION_EXIT(4)</b>		<br>Exit type logger exception.<br>
	 *										Sets this exception type when log file stream closing has failed.
	 */
	enum LoggerExceptionType
	{
		LOGGER_EXCEPTION_INIT = 1, 
		LOGGER_EXCEPTION_STREAM = 2, 
		LOGGER_EXCEPTION_PERMISSION = 3, 
		LOGGER_EXCEPTION_EXIT = 4
	};

	/**
	 * @class LoggerException
	 *
	 * @brief Class which implements logger general exception handler.
	 */
	class LoggerException
	{

	private:

		//! Logger exception message
		std::string message;
		//! Logger exception type
		LoggerExceptionType type;

	public:

		//! Constructor 
		/*! Sets the logger exception type.
		 *
		 * @param	type The logger exception type.
		 */
		LoggerException(LoggerExceptionType type)
		{
			this->type = type;
		};

		//! Constructor 
		/*! Sets the logger exception type & message in C-wstring format.
		 *
		 * @param	type	The logger exception type.
		 * @param	msg		The logger exception message in C-wstring format.
		 */
		LoggerException(LoggerExceptionType type, const wstring& msg)
		{
			this->type = type;
			string message(msg.begin(), msg.end());
			this->message = message;
		};

		//! Constructor 
		/*! Sets the logger exception type & message in C-string format.
		 *
		 * @param	type	The logger exception type.
		 * @param	msg		The logger exception message in C-string format.
		 */
		LoggerException(LoggerExceptionType type, const string& msg)
		{
			this->type = type;
			this->message = msg;
		};

		/**
		 * Retrieves the logger exception type.
		 *
		 * @return	The logger exception type.
		 */
		LoggerExceptionType GetType()
		{
			return type;
		};

		/**
		 * Retrieves the logger exception message as C-string.
		 *
		 * @return	The logger exception message as C-string.
		 */
		const char *GetMsg()
		{
			return message.c_str();
		};
	};

	/**
	 * @class BlockingWStringQueue
	 *
	 * @brief Utility queue class which performs wstring pop/push operation.
	 */
	class BlockingWStringQueue
	{

	private:

		//! mutex lock
		std::mutex mtx;
		//! wstring queue
		std::queue<std::wstring> queue;

	public:

		/**
		 * <b>Pop element from queue</b> <br>
		 * Pop elements from queue, returning true if an item poped from the queue; false otherwise.
		 *
		 * @param	rslt	Pointer to a wstring where the resulting C-wstring is stored.
		 *
		 * @return 	true is returned in the case that an item poped from the queue.
		 *			Otherwise, false is returned.
		 */
		bool pop(std::wstring& rslt)
		{
			std::lock_guard<std::mutex> lock(mtx);
			if (queue.empty())
				return false;
			rslt = queue.front();
			queue.pop();
			return true;
		};

		/**
		 * <b>Push element to the queue</b><br>
		 * Push an element to the queue.
		 *
		 * @param	src		The C-wstring element that requires insertion.
		 */
		void push(std::wstring src)
		{
			std::lock_guard<std::mutex> lock(mtx);
			queue.push(src);
		};
	};

	/**
	 * @class LoggerUtil
	 *
	 * @brief Utility class which provides helper functions to logging operations.
	 */
	class LoggerUtil
	{

	public:
		
		//! <b>Check file existence.</b><br>
		static bool FileExists(const std::string& fileName);

		//! <b>Get time stamp in 'yyyy-MM-dd HH:mm:ss.SSS' format.</b><br>
		static void GetTimeString(wchar_t *dst);

		//! <b>Validate file/directory permission.</b><br>
		static int HasPermissions(const char* dirPath);

		//! <b>Set sleep to the process.</b><br>
		static void Sleep(unsigned int milliseconds);

		//! <b>Write formatted data to wstring (wstring version sprintf).</b><br>
		static wstring StrFormat(const wchar_t* format, ...);
	};

	/**
	 * @class LoggerWorker
	 *
	 * @brief Worker class which manages logging threads and all logging operations.
	 */
	class LoggerWorker
	{

	public:

		//! Log severity level
		SeverityLevel severityLevel;
		//! std out mutex lock
		std::mutex mtxStdOut;
		//! Enable/disable the logging to console
		volatile bool hasConsoleLogging;

		//! Enable/disable the application logging
		volatile bool hasAplLog;
		//! Application log file path
		std::string aplLogFilePath;
		//! Application log file stream
		std::ofstream aplLogFileStream;
		//! Application log mutex lock
		std::mutex mtxAplLog;
		//! Application log queue
		BlockingWStringQueue aplLogQueue;

		//! Enable/disable the debug logging
		volatile bool hasDbgLog;
		//! Debug log file path
		std::string dbgLogFilePath;
		//! Debug log file stream
		std::ofstream dbgLogFileStream;
		//! Debug log mutex lock
		std::mutex mtxDbgLog;
		//! Debug log queue
		BlockingWStringQueue dbgLogQueue;

		//! Enable/disable the event logging
		volatile bool hasEvntLog;
		//! Event log file path
		std::string evntLogFilePath;
		//! Event log file stream
		std::ofstream evntLogFileStream;
		//! Event log mutex lock
		std::mutex mtxEvntlog;
		//! Event log queue
		BlockingWStringQueue evntLogQueue;

	public:

		//! Constructor
		LoggerWorker();

		//! Destructor
		~LoggerWorker();

		//! <b>Initialize LoggerWorker.</b><br>
		void Init(std::string& aplLogPath, std::string& dbgLogPath, std::string& evntLogPath);

		//! <b>Push log record to the application log queue.</b><br>
		void OutputAplLine(SeverityLevel level, const wchar_t *logRecord);

		//! <b>Push log record to the debug log queue.</b><br>
		void OutputDbgLine(const wchar_t *logRecord);

		//! <b>Push log record to the event log queue.</b><br>
		void OutputEvntLine(const wchar_t *logRecord);

		//! <b>Write to application log file.</b><br>
		void WriteToAplFile();

		//! <b>Write to debug log file.</b><br>
		void WriteToDbgFile();

		//! <b>Write to event log file.</b><br>
		void WriteToEvntFile();

		//! <b>Release and close all loggers</b><br>
		void DropAll();
	};

	/**
	 * @class Logger
	 *
	 * @brief Utility class which provides the interfaces to write application/debug/event logs.
	 */
	class Logger
	{
	private:

		//! <b>Write the formatted log record to the respective log queue.</b><br>
		static void WriteLog(SeverityLevel level, unsigned long code, const wchar_t* format, va_list args);

		//! <b>Write the formatted log record to syslog.</b><br>
		static void WriteSysLog(int level, const wchar_t* format, va_list args);

	public:

		//! Constructor
		Logger();

		//! Destructor
		~Logger();

		//! <b>Interface to initialize the logger.</b><br>
		static void Init(std::string& pathAplLog, std::string& pathDbgLog, std::string& pathEvntLog);

		//! <b>Interface to set log level.</b><br>
		static void SetLogSeverityLevel(SeverityLevel level);

		//! <b>Interface to enable/disable file logging.</b><br>
		static void EnableFileLogging(bool value);

		//! <b>Interface to enable/disable console logging.</b><br>
		static void EnableConsoleLogging(bool value);

		//! <b>Interface to enable/disable application logging.</b><br>
		static void EnableAplLogging(bool value);

		//! <b>Interface to enable/disable debug logging.</b><br>
		static void EnableDbgLogging(bool value);

		//! <b>Interface to enable/disable event logging.</b><br>
		static void EnableEvntLogging(bool value);

		//! <b>Interface to write (application) critical level log records.</b><br>
		static void Crit(const wchar_t * format, ...);
		//! <b>Interface to write (application) critical level log records.</b><br>
		static void Crit(unsigned long code, const wchar_t * format, ...);

		//! <b>Interface to write (application) error level log records.</b><br>
		static void Error(const wchar_t * format, ...);
		//! <b>Interface to write (application) error level log records.</b><br>
		static void Error(unsigned long code, const wchar_t * format, ...);

		//! <b>Interface to write (application) info level log records.</b><br>
		static void Info(const wchar_t * format, ...);
		//! <b>Interface to write (application) info level log records.</b><br>
		static void Info(unsigned long code, const wchar_t * format, ...);

		//! <b>Interface to write (application) warning level log records.</b><br>
		static void Warn(const wchar_t * format, ...);
		//! <b>Interface to write (application) warning level log records.</b><br>
		static void Warn(unsigned long code, const wchar_t * format, ...);

		//! <b>Interface to write debug level log records.</b><br>
		static void Debug(const wchar_t * format, ...);

		//! <b>Interface to write event level log records.</b><br>
		static void Event(const wchar_t * format, ...);

		//! <b>Interface to write the critical level log records to syslog.</b><br>
		static void SysLogCrit(const wchar_t* format, ...);

		//! <b>Interface to write the debug level log records to syslog.</b><br>
		static void SysLogDebug(const wchar_t* format, ...);

		//! <b>Interface to write the error level log records to syslog.</b><br>
		static void SysLogError(const wchar_t* format, ...);

		//! <b>Interface to write the information level log records to syslog.</b><br>
		static void SysLogInfo(const wchar_t* format, ...);

		//! <b>Interface to write the warning level log records to syslog.</b><br>
		static void SysLogWarn(const wchar_t* format, ...);

		//! <b>Release and close all loggers.</b><br>
		static void DropAll();
	};

	/**
	 * @class CallLog
	 *
	 * @brief Utility class which provides debug call history logger separately.
	 */
	 /*!
	  <pre>Usage:
		 1) CallLog log(L"ClassName::FunctionName()");
			 Description:
				 Enter and Leave logs shall be write to the log file
			 Result:
				 ClassName::FunctionName() Enter
				 ClassName::FunctionName() Leave
		 2) CallLog log(L"ClassName::FunctionName(param1=%d, param2=%u, param3=%s)", value1, value2, value3);
			 Description:
				 Enter and Leave logs shall be write to the log file
			 Result:
				 ClassName::FunctionName(param1=value1, param2=value2, param3=value3) Enter
				 ClassName::FunctionName(param1=value1, param2=value2, param3=value3) Leave
		 3) CallLog log(__FILE__, __LINE__, __func__);
			 Description:
				 Enter and Leave logs shall be write to the log file
			 Result:
				 ClassName.cpp [FunctionName():LineNo] START
				 ClassName.cpp [FunctionName():LineNo] END</pre>
	 */
	class CallLog
	{
	private:
		//! Variable to differentiate START/END & Enter/Leave logging
		int mode;
		//! The line number (macro __LINE__ from caller)
		int line;
		//! The file name (macro __FILE__ from caller)
		const char *file;
		//! The function name (macro __func__ from caller)
		const char *func;
		//! Buffer to hold the log information
		wchar_t formatBuffer[MAX_LEN_FMT_BUFFER];

	public:

		//! Constructor
		/*!
		 * Writes the function start-up log to the debug log file.
		 *
		 * @param	file	Pointer to the file name set by macro __FILE__
		 * @param	line	Pointer to the line number set by macro __LINE__
		 * @param	func	Pointer to the function name set by macro __func__
		 */
		CallLog(const char *file, int line, const char *func)
		{
			this->file = file;
			this->line = line;
			this->func = func;
			mode = 0;
			memset(formatBuffer, 0, sizeof(formatBuffer));
			Logger::Debug(L"%s [%s():%d] START", file, func, line);
		};

		//! Constructor
		/*!
		 * Writes the function start-up log to the debug log file.
		 *
		 * @param	format	C string that contains a format string that follows the same specifications as format in
		 *					printf (see printf for details)
		 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
		 *					of additional arguments, each containing a value to be used to replace a format specifier
		 *					in the format string (or a pointer to a storage location, for n). There should be at least
		 *					as many of these arguments as the number of values specified in the format specifiers.
		 *					Additional arguments are ignored by the function.
		 */
		CallLog(const wchar_t* format, ...)
		{
			this->file = "";
			this->line = 0;
			this->func = "";

			va_list vl;
			va_start(vl, format);
			memset(formatBuffer, 0, sizeof(formatBuffer));
			int ret = vswprintf(formatBuffer, MAX_LEN_FMT_BUFFER, format, vl);
			va_end(vl);

			if (ret != -1) {
				mode = 1;
				Logger::Debug(L"%S Enter", formatBuffer);
			}
		};

		//! Destructor
		/*!
		 * Writes the function end log to the debug log file.
		 */
		~CallLog()
		{
			if (mode == 0) {
				Logger::Debug(L"%s()[%s:%d] END", func, file, line);
			} else {
				Logger::Debug(L"%S Leave", formatBuffer);
			}
		};
	};
} /* namespace cpplogger */
#endif /* _LOG_LOGGER_H_ */