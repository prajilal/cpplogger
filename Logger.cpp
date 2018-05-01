//////////////////////////////////////////////////////////////////////////////
// @File Name:      Logger.cpp                                              //
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

#include "Logger.h"

using namespace std;
using namespace cpplogger;
using namespace std::chrono;

namespace cpplogger {

	//! Instance to LoggerWorker class
	LoggerWorker worker;
	//! Instance to application log write thread
	std::thread *mAplThread = 0;
	//! Instance to debug log write thread
	std::thread *mDbgThread = 0;
	//! Instance to event log write thread
	std::thread *mEvntThread = 0;
	//! Sets the application log write thread interruption status
	volatile bool isInterruptedApl = false;
	//! Sets the debug log write thread interruption status
	volatile bool isInterruptedDbg = false;
	//! Sets the event log write thread interruption status
	volatile bool isInterruptedEvnt = false;

	/**
	 * Checks whether the file exist, returning true if it exists; false otherwise.
	 *
	 * @param 	fileName	Pointer to a file name that requires validation.
	 *
	 * @return 	true is returned in the case that the file exist.
	 *			Otherwise, false is returned.
	 */
	bool LoggerUtil::FileExists(const std::string& fileName)
	{
		return (access(fileName.c_str(), F_OK) != -1);
	}

	/**
	 * Write time stamp in 'yyyy-MM-dd HH:mm:ss.SSS' format to string.<br>
	 * The content is stored as a C string in the buffer pointed by formatBuff.
	 *
	 * @param	buff	Pointer to a buffer where the resulting C-wchar_t is stored.
	 *					The buffer should be large enough to contain the resulting string.
	 */
	void LoggerUtil::GetTimeString(wchar_t *buff)
	{
		
		system_clock::time_point now = system_clock::now();
		long long ms_since_epoch = duration_cast<milliseconds>(now.time_since_epoch()).count();
		time_t sec_since_epoch = time_t(ms_since_epoch / 1000);
		tm time_info;
		localtime_r(&sec_since_epoch, &time_info);

		// Format time in yyyy-MM-dd HH:mm:ss.SSS format
		swprintf(buff, MAX_LEN_DATE_BUFFER, L"%04d-%02d-%02d %02d:%02d:%02d.%03lld",
			1900 + time_info.tm_year, 1 + time_info.tm_mon,
			time_info.tm_mday, time_info.tm_hour, time_info.tm_min,
			time_info.tm_sec, ms_since_epoch % 1000);
	}

	/**
	 * Checks whether proper permissions (exist/read/write) are set, returning 0 if it set; false otherwise.
	 *
	 * @param	dirPath		Pointer to the file/directory name that requires validation
	 *
	 * @return 	0 is returned in the case that proper permissions are set.
	 *			Otherwise, error number is returned.
	 */
	int LoggerUtil::HasPermissions(const char* dirPath)
	{
		// Check file existence
		if (access (dirPath, F_OK) != 0) {
			if (errno == ENOENT) {
				Logger::SysLogError(L"LoggerUtil::HasPermissions() the path(%s) does not exist", dirPath);
			} else if (errno == EACCES) {
				Logger::SysLogError(L"LoggerUtil::HasPermissions() the path(%s) is not accessible", dirPath);
			}

			return errno;
		}

		// Check read access
		if (access (dirPath, R_OK) != 0) {
			Logger::SysLogError(
				L"LoggerUtil::HasPermissions() the path(%s) is not readable (access denied)", dirPath);
			return errno;
		}

		// Check write access
		if (access (dirPath, W_OK) != 0) {
			if (errno == EACCES) {
				Logger::SysLogError(
					L"LoggerUtil::HasPermissions() the path(%s) is not write-able (access denied)", dirPath);
				return errno;
			} else if (errno == EROFS) {
				Logger::SysLogError(
					L"LoggerUtil::HasPermissions() the path(%s) is not write-able (read-only file-system)", dirPath);
				return errno;
			}
			return errno;
		}

		// Success, return zero(0)
		return 0;
	}

	/**
	 * Set sleep to the process for given number of milliseconds.
	 *
	 * @param	mSec	the number of milliseconds to sleep
	 */
	void LoggerUtil::Sleep(unsigned int mSec)
	{
		struct timespec timeReq = { 0 };
		unsigned long val = ((unsigned long) mSec) * 1000000L;
		timeReq.tv_sec = val / 1000000000L; //9 zero
		timeReq.tv_nsec = val % 1000000000L; //9 zero
		nanosleep(&timeReq, (struct timespec *)NULL);
	}

	/**
	 * Composes a wstring with the same text that would be printed.<br>
	 * After the format parameter, the function expects at least as many additional arguments as needed for format.
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 *
	 * @return	formatted data in wstring format
	 */
	std::wstring LoggerUtil::StrFormat(const wchar_t* format, ...)
	{
		va_list vl;
		va_start(vl, format);
		wchar_t	formatBuffer[MAX_LEN_FMT_BUFFER];
		memset(formatBuffer, 0, sizeof(formatBuffer));
		int ret = vswprintf(formatBuffer, MAX_LEN_FMT_BUFFER, format, vl);
		va_end(vl);

		wstring retString;
		if (ret != -1) {
			retString = wstring(formatBuffer);
		}

		return retString;
	}

	//! Constructor
	LoggerWorker::LoggerWorker()
	{
		severityLevel = ERROR;

		// Disable all logging operations
		hasAplLog = false;
		hasDbgLog = false;
		hasEvntLog = false;
		hasConsoleLogging = false;
		isInterruptedApl = false;
		isInterruptedDbg = false;
		isInterruptedEvnt = false;
	}

	/**
	 * Initialize logger threads and sets the log file names.
	 *
	 * @param	aplLogPath	Pointer to the application log file name
	 * @param	dbgLogPath	Pointer to the debug log file name
	 * @param	evntLogPath	Pointer to the event log file name
	 *
	 * @throw	The logger exception with exception details.
	 */
	void LoggerWorker::Init(std::string&aplLogPath, std::string& dbgLogPath, std::string& evntLogPath)
	{
		try {
			this->aplLogFilePath = aplLogPath;
			this->dbgLogFilePath = dbgLogPath;
			this->evntLogFilePath = evntLogPath;

			// Application log write thread creation
			mAplThread = new std::thread(&LoggerWorker::WriteToAplFile, this);
			mAplThread->detach();
			isInterruptedApl = false;

			// Debug log write thread creation
			mDbgThread = new std::thread(&LoggerWorker::WriteToDbgFile, this);
			mDbgThread->detach();
			isInterruptedDbg = false;

			// Event log write thread creation
			mEvntThread = new std::thread(&LoggerWorker::WriteToEvntFile, this);
			mEvntThread->detach();
			isInterruptedEvnt = false;
		} catch (const std::exception& e) {
			// Write to syslog and throw
			Logger::SysLogError(L"Failed to create logger threads(%s)", e.what());
			throw LoggerException(
				LOGGER_EXCEPTION_INIT,
				LoggerUtil::StrFormat(L"LoggerWorker::Init(): failed to create logger threads(%s)", e.what()));
		} catch (...) {
			// Write to syslog and throw
			Logger::SysLogError(L"Failed to create logger threads(unknown exception)");
			throw LoggerException(
				LOGGER_EXCEPTION_INIT, L"LoggerWorker::Init(): failed to create logger threads(unknown exception)");
		}
	}

	/**
	 * Receives the log record to write, and pushes the record to the application log queue.
	 *
	 * @param	level		The log severity level
	 * @param 	logRecord 	Pointer to the log record which is to be add to the queue.
	 */
	void LoggerWorker::OutputAplLine(SeverityLevel level, const wchar_t *logRecord)
	{
		wstring ws(logRecord);
		std::lock_guard<std::mutex> lock(mtxStdOut);
		aplLogQueue.push(ws);

		if (hasConsoleLogging) {
			// Printing coloured characters to terminal for error, critical and warning log levels. Note that not
			// all terminals support this; if colour sequences are not supported, garbage will show up.
			// 
			// Example:
			// cout << "\033[1;31mBOLD RED TEXT\033[0m\n"; 
			// Here, \033 is the ESC character, ASCII 27. It is followed by [, then zero or more numbers separated
			// by;, and finally the letter m.The numbers describe the colour and format to switch to from that 
			// point onwards. 
			//
			// The codes for foreground colours used are:
			//          foreground background
			// red      31         41
			// yellow   33         43
			// white    37         47
			//
			// Additionally, used numbers are:
			// reset             0  (everything back to normal) - at the end
			// bold/bright       1  (often a brighter shade of the same colour) - to make the text bold
			// inverse           7  (swap foreground and background colours)
			switch (level) {
				case CRITICAL:
				wcout << L"\033[1;7;31;47m" << ws << "\033[0m" << std::endl;
				break;
				case ERROR:
				wcout << L"\033[1;31m" << ws << "\033[0m" << std::endl;
				break;
				case WARNING:
				wcout << L"\033[1;33m" << ws << "\033[0m" << std::endl;
				break;
				default:
				wcout << ws << endl;
				break;
			}
		}
	}

	/**
	 * Receives the log record to write, and pushes the record to the debug log queue.
	 *
	 * @param 	logRecord 	Pointer to the log record which is to be add to the queue.
	 */
	void LoggerWorker::OutputDbgLine(const wchar_t *logRecord)
	{
		wstring ws(logRecord);
		std::lock_guard<std::mutex> lock(mtxStdOut);
		dbgLogQueue.push(ws);

		if (hasConsoleLogging) {
			wcout << ws << endl;
		}
	}

	/**
	 * Receives the log record to write, and pushes the record to the event log queue.
	 *
	 * @param 	logRecord 	Pointer to the log record which is to be add to the queue.
	 */
	void LoggerWorker::OutputEvntLine(const wchar_t *logRecord)
	{
		wstring ws(logRecord);
		std::lock_guard<std::mutex> lock(mtxStdOut);
		evntLogQueue.push(ws);

		if (hasConsoleLogging) {
			// Printing coloured characters to terminal for error, critical and warning log levels. Note that not
			// all terminals support this; if colour sequences are not supported, garbage will show up.
			// 
			// Example:
			// cout << "\033[1;31mBOLD RED TEXT\033[0m\n"; 
			// Here, \033 is the ESC character, ASCII 27. It is followed by [, then zero or more numbers separated
			// by;, and finally the letter m.The numbers describe the colour and format to switch to from that
			// point onwards. 
			//
			// The codes for foreground colours used are:
			//          foreground background
			// green    32         42
			//
			// Additionally, used numbers are:
			// reset             0  (everything back to normal) - at the end
			// bold/bright       1  (often a brighter shade of the same colour) - to make the text bold
			wcout << L"\033[1;32m" << ws << "\033[0m" << std::endl;
		}
	}

	/**
	 * Pop log record from application log queue and writes to application log file.
	 *
	 * @throw	The logger exception with exception details.
	 */
	void LoggerWorker::WriteToAplFile()
	{
		while (!isInterruptedApl) {
			try {
				wstring tmp;
				if (!aplLogQueue.pop(tmp)) {
					// Wait for 100 ms to get data filled in queue
					LoggerUtil::Sleep(SLEEP_IN_MS);
					continue;
				}

				if (!aplLogFileStream.is_open())
					aplLogFileStream.open(
						aplLogFilePath, std::ofstream::out | std::ofstream::app | std::ostream::binary);

				// Write errors to syslog when stream error occurred
				if (aplLogFileStream.bad() || aplLogFileStream.fail()) {
					Logger::SysLogInfo(tmp.c_str());
					aplLogFileStream.close();
				} else {
					{
						std::lock_guard<std::mutex> lock(mtxAplLog);
						aplLogFileStream << std::string(tmp.begin(), tmp.end()) << endl;
					}
					aplLogFileStream.flush();
				}
			} catch (std::exception& ex) {
				Logger::SysLogError(
					L"LoggerWorker::WriteToAplFile() failed to write to application log file(%s)", ex.what());
				throw LoggerException(
					LOGGER_EXCEPTION_STREAM,
					LoggerUtil::StrFormat(
						L"LoggerWorker::WriteToAplFile() failed to write to application log file(%s)", ex.what()));
			} catch (...) {
				Logger::SysLogError(
					L"LoggerWorker::WriteToAplFile() failed to write to application log file(unknown exception)");
				throw LoggerException(
					LOGGER_EXCEPTION_STREAM,
					L"LoggerWorker::WriteToAplFile() failed to write to application log file(unknown exception)");
			}
		}
	}

	/**
	 * Pop log record from debug log queue and writes to debug log file.
	 *
	 * @throw	The logger exception with exception details.
	 */
	void LoggerWorker::WriteToDbgFile()
	{
		while (!isInterruptedDbg) {
			try {
				wstring tmp;
				if (!dbgLogQueue.pop(tmp)) {
					// Wait for 100 ms to get data filled in queue
					LoggerUtil::Sleep(SLEEP_IN_MS);
					continue;
				}

				if (!dbgLogFileStream.is_open())
					dbgLogFileStream.open(
						dbgLogFilePath, std::ofstream::out | std::ofstream::app | std::ostream::binary);

				// Write errors to syslog when stream error occurred
				if (dbgLogFileStream.bad() || dbgLogFileStream.fail()) {
					Logger::SysLogInfo(tmp.c_str());
					dbgLogFileStream.close();
				} else {
					{
						std::lock_guard<std::mutex> lock(mtxDbgLog);
						dbgLogFileStream << std::string(tmp.begin(), tmp.end()) << endl;
					}
					dbgLogFileStream.flush();
				}
			} catch (std::exception& ex) {
				Logger::SysLogError(
					L"LoggerWorker::WriteToDbgFile() failed to write to debug log file(%s)", ex.what());
				throw LoggerException(
					LOGGER_EXCEPTION_STREAM,
					LoggerUtil::StrFormat(
						L"LoggerWorker::WriteToDbgFile() failed to write to debug log file(%s)", ex.what()));
			} catch (...) {
				Logger::SysLogError(
					L"LoggerWorker::WriteToDbgFile() failed to write to debug log file (unknown exception)");
				throw LoggerException(
					LOGGER_EXCEPTION_STREAM,
					L"LoggerWorker::WriteToDbgFile() failed to write to debug log file(unknown exception)");
			}
		}
	}

	/**
	 * Pop log record from event log queue and writes to event log file.
	 *
	 * @throw	The logger exception with exception details.
	 */
	void LoggerWorker::WriteToEvntFile()
	{
		while (!isInterruptedEvnt) {
			try {
				wstring tmp;
				if (!evntLogQueue.pop(tmp)) {
					// Wait for 100 ms to get data filled in queue
					LoggerUtil::Sleep(SLEEP_IN_MS);
					continue;
				}

				if (!evntLogFileStream.is_open())
					evntLogFileStream.open(
						evntLogFilePath, std::ofstream::out | std::ofstream::app | std::ostream::binary);

				// Write errors to syslog when stream error occurred
				if (evntLogFileStream.bad() || evntLogFileStream.fail()) {
					Logger::SysLogInfo(tmp.c_str());
					evntLogFileStream.close();
				} else {
					{
						std::lock_guard<std::mutex> lock(mtxEvntlog);
						evntLogFileStream << std::string(tmp.begin(), tmp.end()) << endl;
					}
					evntLogFileStream.flush();
				}
			} catch (std::exception& ex) {
				Logger::SysLogError(
					L"LoggerWorker::WriteToEvntFile() failed to write to event log file(%s)", ex.what());
				throw LoggerException(
					LOGGER_EXCEPTION_STREAM,
					LoggerUtil::StrFormat(
						L"LoggerWorker::WriteToEvntFile() failed to write to event log file(%s)", ex.what()));
			} catch (...) {
				Logger::SysLogError(
					L"LoggerWorker::WriteToEvntFile() failed to write to event log file (unknown exception)");
				throw LoggerException(
					LOGGER_EXCEPTION_STREAM,
					L"LoggerWorker::WriteToEvntFile() failed to write to event log file(unknown exception)");
			}
		}
	}

	/**
	 * Release and close all loggers
	 *
	 * @throw	The logger exception with exception details.
	 */
	void LoggerWorker::DropAll()
	{
		// Sleep server for 200 ms before exit
		LoggerUtil::Sleep(2 * SLEEP_IN_MS);

		isInterruptedApl = true;
		isInterruptedDbg = true;
		isInterruptedEvnt = true;

		try {
			if (aplLogFileStream.is_open())
				aplLogFileStream.close();
		} catch (...) {
			throw LoggerException(
				LOGGER_EXCEPTION_EXIT, L"LoggerWorker::DropAll() failed to close application log file stream");
		}

		try {
			if (dbgLogFileStream.is_open())
				dbgLogFileStream.close();
		} catch (...) {
			throw LoggerException(
				LOGGER_EXCEPTION_EXIT, L"LoggerWorker::DropAll() failed to close debug log file stream");
		}

		try {
			if (evntLogFileStream.is_open())
				evntLogFileStream.close();
		} catch (...) {
			throw LoggerException(
				LOGGER_EXCEPTION_EXIT, L"LoggerWorker::DropAll() failed to close event log file stream");
		}

		// Disable all logging operations
		hasAplLog = false;
		hasDbgLog = false;
		hasEvntLog = false;
		hasConsoleLogging = false;
	}

	//! Destructor
	LoggerWorker::~LoggerWorker()
	{
		try {
			DropAll();
		} catch (LoggerException& le) {
			std::cout << "LoggerWorker::~LoggerWorker() error closing stream (" << le.GetMsg() << ")" << std::endl;
			syslog(
				LOG_ERR, "LoggerWorker::~LoggerWorker() error closing stream (%s)", le.GetMsg());
		}
	}

	//! Constructor
	Logger::Logger() { }

	/*
	 * Write the formatted log record to the respective log queue.
	 *
	 * @param	level	The log severity level
	 * @param	code	The 5 digit custom defined code to each record.
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	args	The variable argument list (va_list)
	 */
	void Logger::WriteLog(SeverityLevel level, unsigned long code, const wchar_t* format, va_list args)
	{
		wchar_t time[MAX_LEN_DATE_BUFFER];
		wchar_t formatBuffer[MAX_LEN_FMT_BUFFER];
		wchar_t stringBuffer[MAX_LEN_STR_BUFFER];
		memset(formatBuffer, 0, sizeof(formatBuffer));
		memset(stringBuffer, 0, sizeof(stringBuffer));
		LoggerUtil::GetTimeString(time);
		
		int ret = vswprintf(formatBuffer, MAX_LEN_FMT_BUFFER, format, args);

		if (ret != -1) {
			ret = 0;
			switch (level) {
				case CRITICAL:
				ret = swprintf(stringBuffer, MAX_LEN_STR_BUFFER, L"%S [CRIT]: C9%05lu, %S", time, code, formatBuffer);
				break;
				case ERROR:
				ret = swprintf(stringBuffer, MAX_LEN_STR_BUFFER, L"%S [ERR ]: E8%05lu, %S", time, code, formatBuffer);
				break;
				case INFO:
				ret = swprintf(stringBuffer, MAX_LEN_STR_BUFFER, L"%S [INFO]: I0%05lu, %S", time, code, formatBuffer);
				break;
				case WARNING:
				ret = swprintf(stringBuffer, MAX_LEN_STR_BUFFER, L"%S [WARN]: W7%05lu, %S", time, code, formatBuffer);
				break;
				case DEBUG:
				ret = swprintf(stringBuffer, MAX_LEN_STR_BUFFER, L"%S [DEBUG]: %S", time, formatBuffer);
				break;
				case EVENT:
				ret = swprintf(stringBuffer, MAX_LEN_STR_BUFFER, L"%S [EVENT]: %S", time, formatBuffer);
				break;
				default:
				ret = 0;
				break;
			}
			
			if (ret > 0) {
				switch (level) {
					case CRITICAL:
					case ERROR:
					case INFO:
					case WARNING:
					worker.OutputAplLine(level, stringBuffer);
					break;
					case DEBUG:
					worker.OutputDbgLine(stringBuffer);
					break;
					case EVENT:
					worker.OutputEvntLine(stringBuffer);
					break;
					default:
					break;
				}
			}
		}
	}

	/*
	 * Write the formatted log record to syslog (/var/log/messages).
	 *
	 * @param	level	The log level
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	args	The variable argument list (va_list)
	 */
	void Logger::WriteSysLog(int level, const wchar_t* format, va_list args)
	{
		wchar_t formatBuffer[MAX_LEN_FMT_BUFFER];
		wchar_t stringBuffer[MAX_LEN_STR_BUFFER];
		memset(formatBuffer, 0, sizeof(formatBuffer));
		memset(stringBuffer, 0, sizeof(stringBuffer));
		int ret = vswprintf(formatBuffer, MAX_LEN_FMT_BUFFER, format, args);

		if (ret != -1) {
			ret = swprintf(stringBuffer, MAX_LEN_STR_BUFFER, L"%S", formatBuffer);
			if (ret > 0) {
				wstring wstr(stringBuffer);
				string str(wstr.begin(), wstr.end());
				std::wcout << wstr << std::endl;
				syslog(level, str.c_str());
			}
		}
	}

	/**
	 * Validate all the log files and initialize logger worker process.<br>
	 * Set all log file paths to default, if no values provided from calling module.
	 *
	 * @param	aplLogPath	Pointer to the application log file name
	 * @param	dbgLogPath	Pointer to the debug log file name
	 * @param	evntLogPath	Pointer to the event log file name
	 *
	 * @throw	The logger exception with exception details.
	 */
	void Logger::Init(std::string& aplLogPath, std::string& dbgLogPath, std::string& evntLogPath)
	{
		// Set all log file paths to default, if no values provided from calling module.
		locale loc = locale::global(locale(LOCALE_DEFAULT));
		string tempLocale = LOCALE_DEFAULT;
		SysLogInfo(L"Logger::Init() locale has changed from (%s) to (%s)", loc.name().c_str(), tempLocale.c_str());

		{
			// Set application log file path to default, if empty.
			if (aplLogPath.empty() || aplLogPath[0] == '\0' || aplLogPath[0] == ' ') {
				aplLogPath = APL_LOG_PATH_DEFAULT;
				Logger::SysLogWarn(
					L"Logger::Init() application log file path is not valid, setting to default value (%s)", aplLogPath.c_str());
			}

			// Extract path and check directory permissions
			char* dirPath = strdup(aplLogPath.c_str());
			char* dirPathVal = dirname(dirPath);
			
			if (LoggerUtil::HasPermissions(dirPathVal) != 0) {
				SysLogError(
					L"Logger::Init() failed to validate application log file directory (%s) permissions", dirPathVal);
				throw LoggerException(
					LOGGER_EXCEPTION_PERMISSION,
					LoggerUtil::StrFormat(
						L"Logger::Init() failed to validate application log file directory (%s) permissions", dirPathVal));
			}

			// Validate application log file permission if already exists
			if (LoggerUtil::FileExists(aplLogPath) && LoggerUtil::HasPermissions(aplLogPath.c_str()) != 0) {
				SysLogError(
					L"Logger::Init() failed to validate application log file (%s) permissions", aplLogPath.c_str());
				throw LoggerException(
					LOGGER_EXCEPTION_PERMISSION,
					LoggerUtil::StrFormat(
						L"Logger::Init() failed to validate application log file (%s) permissions", aplLogPath.c_str()));
			}
		}

		{
			// Set debug log file path to default, if empty.
			if (dbgLogPath.empty() || dbgLogPath[0] == '\0' || dbgLogPath[0] == ' ') {
				dbgLogPath = DBG_LOG_PATH_DEFAULT;
				Logger::SysLogWarn(
					L"Logger::Init() debug log file path is not valid, setting to default value (%s)", dbgLogPath.c_str());
			}

			// Extract path and check directory permissions
			char* dirPath = strdup(dbgLogPath.c_str());
			char* dirPathVal = dirname(dirPath);
			
			if (LoggerUtil::HasPermissions(dirPathVal) != 0) {
				SysLogError(L"Logger::Init() failed to validate debug log directory (%s) permissions", dirPathVal);
				throw LoggerException(
					LOGGER_EXCEPTION_PERMISSION,
					LoggerUtil::StrFormat(
						L"Logger::Init() failed to validate debug log file directory (%s) permissions", dirPathVal));
			}

			// Validate debug log file permission if already exists
			if (LoggerUtil::FileExists(dbgLogPath) && LoggerUtil::HasPermissions(dbgLogPath.c_str()) != 0) {
				SysLogError(
					L"Logger::Init() failed to validate debug log file (%s) permissions", dbgLogPath.c_str());
				throw LoggerException(
					LOGGER_EXCEPTION_PERMISSION,
					LoggerUtil::StrFormat(
						L"Logger::Init() failed to validate debug log file (%s) permissions", dbgLogPath.c_str()));

			}
		}

		{
			// Set event log file path to default, if empty.
			if (evntLogPath.empty() || evntLogPath[0] == '\0' || evntLogPath[0] == ' ') {
				evntLogPath = EVNT_LOG_PATH_DEFAULT;
				Logger::SysLogWarn(
					L"Logger::Init() event log file path is not valid, setting to default value (%s)", evntLogPath.c_str());
			}

			// Extract path and check directory permissions
			char* dirPath = strdup(evntLogPath.c_str());
			char* dirPathVal = dirname(dirPath);

			if (LoggerUtil::HasPermissions(dirPathVal) != 0) {
				SysLogError(L"Logger::Init() failed to validate event log directory (%s) permissions", dirPathVal);
				throw LoggerException(
					LOGGER_EXCEPTION_PERMISSION,
					LoggerUtil::StrFormat(
						L"Logger::Init() failed to validate event log file directory (%s) permissions", dirPathVal));
			}

			// Validate event log file permission if already exists
			if (LoggerUtil::FileExists(evntLogPath) && LoggerUtil::HasPermissions(evntLogPath.c_str()) != 0) {
				SysLogError(
					L"Logger::Init() failed to validate event log file (%s) permissions", evntLogPath.c_str());
				throw LoggerException(
					LOGGER_EXCEPTION_PERMISSION,
					LoggerUtil::StrFormat(
						L"Logger::Init() failed to validate event log file (%s) permissions", evntLogPath.c_str()));
			}
		}
		worker.Init(aplLogPath, dbgLogPath, evntLogPath);
	}

	/**
	 * Set the log severity level (determines minimum log level).
	 *
	 * @param	level	the log severity level
	 */
	void Logger::SetLogSeverityLevel(SeverityLevel level)
	{
		worker.severityLevel = level;
	}

	/**
	 * Enable/disable application/debug/event logging to file.
	 *
	 * @param	value	the parameter to enable or disable logging to file.
	 */
	void Logger::EnableFileLogging(bool value)
	{
		worker.hasDbgLog = value;
		worker.hasAplLog = value;
		worker.hasEvntLog = value;
	}

	/**
	 * Enable/disable application/debug/event logging to console.
	 *
	 * @param	value	the parameter to enable or disable logging to console.
	 */
	void Logger::EnableConsoleLogging(bool value)
	{
		worker.hasConsoleLogging = value;
	}

	/**
	 * Enable/disable debug logging only.
	 *
	 * @param	value	the parameter to enable or disable debug logging.
	 */
	void Logger::EnableDbgLogging(bool value)
	{
		worker.hasDbgLog = value;
	}

	/**
	 * Enable/disable application logging only.
	 *
	 * @param	value	the parameter to enable or disable applicaiton logging.
	 */
	void Logger::EnableAplLogging(bool value)
	{
		worker.hasAplLog = value;
	}

	/**
	 * Enable/disable event logging only.
	 *
	 * @param	value	the parameter to enable or disable event logging.
	 */
	void Logger::EnableEvntLogging(bool value)
	{
		worker.hasEvntLog = value;
	}

	/**
	 * Write (application) critical level log records to the application log file.
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::Crit(const wchar_t* format, ...)
	{
		// If requested log level is less than the default log level[set by program], skip writing
		if (CRITICAL < worker.severityLevel) {
			return;
		}

		if (!worker.hasAplLog) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		WriteLog(CRITICAL, LOGGER_CODE_CRIT_DEFAULT, format, vl);
		va_end(vl);
	}

	/**
	 * Write (application) critical level log record to the application log file.
	 *
	 * @param 	code 	The 5 digit custom defined code to each record.
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::Crit(unsigned long code, const wchar_t* format, ...)
	{
		// If requested log level is less than the default log level[set by program], skip writing
		if (CRITICAL < worker.severityLevel) {
			return;
		}

		if (!worker.hasAplLog) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		WriteLog(CRITICAL, code, format, vl);
		va_end(vl);
	}

	/**
	 * Write (application) error level log record to the application log file.
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::Error(const wchar_t* format, ...)
	{
		// If requested log level is less than the default log level[set by program], skip writing
		if (ERROR < worker.severityLevel) {
			return;
		}

		if (!worker.hasAplLog) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		WriteLog(ERROR, LOGGER_CODE_ERRR_DEFAULT, format, vl);
		va_end(vl);
	}

	/**
	 * Write (application) error level log record to the application log file.
	 *
	 * @param 	code 	The 5 digit custom defined code to each record.
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::Error(unsigned long code, const wchar_t* format, ...)
	{
		// If requested log level is less than the default log level[set by program], skip writing
		if (ERROR < worker.severityLevel) {
			return;
		}

		if (!worker.hasAplLog) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		WriteLog(ERROR, code, format, vl);
		va_end(vl);
	}

	/**
	 * Write (application) info level log record to the application log file.
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::Info(const wchar_t* format, ...)
	{
		// If requested log level is less than the default log level[set by program], skip writing
		if (INFO < worker.severityLevel) {
			return;
		}

		if (!worker.hasAplLog) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		WriteLog(INFO, LOGGER_CODE_INFO_DEFAULT, format, vl);
		va_end(vl);
	}

	/**
	 * Write (application) info level log record to the application log file.
	 *
	 * @param 	code 	The 5 digit custom defined code to each record.
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::Info(unsigned long code, const wchar_t* format, ...)
	{
		// If requested log level is less than the default log level[set by program], skip writing
		if (INFO < worker.severityLevel) {
			return;
		}

		if (!worker.hasAplLog) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		WriteLog(INFO, code, format, vl);
		va_end(vl);
	}

	/**
	 * Write (application) warning level log record to the application log file.
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::Warn(const wchar_t* format, ...)
	{
		// If requested log level is less than the default log level[set by program], skip writing
		if (WARNING < worker.severityLevel) {
			return;
		}

		if (!worker.hasAplLog) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		WriteLog(WARNING, LOGGER_CODE_WARN_DEFAULT, format, vl);
		va_end(vl);
	}

	/**
	 * Write (application) warning level log record to the application log file.
	 *
	 * @param 	code 	The 5 digit custom defined code to each record.
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::Warn(unsigned long code, const wchar_t* format, ...)
	{
		// If requested log level is less than the default log level[set by program], skip writing
		if (WARNING < worker.severityLevel) {
			return;
		}

		if (!worker.hasAplLog) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		WriteLog(WARNING, code, format, vl);
		va_end(vl);
	}

	/**
	 * Write debug level log record to the debug log file.
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::Debug(const wchar_t* format, ...)
	{
		if (!worker.hasDbgLog) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		WriteLog(DEBUG, 0, format, vl);
		va_end(vl);
	}

	/**
	 * Write event level log record to the event log file.
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::Event(const wchar_t* format, ...)
	{
		if (!worker.hasEvntLog) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		WriteLog(EVENT, 0, format, vl);
		va_end(vl);
	}

	/**
	 * Write the critical level log record to syslog (/var/log/messages).
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::SysLogCrit(const wchar_t* format, ...)
	{
		va_list vl;
		va_start(vl, format);
		WriteSysLog(LOG_CRIT, format, vl);
		va_end(vl);
	}

	/**
	 * Write the debug level log record to syslog (/var/log/messages).
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::SysLogDebug(const wchar_t* format, ...)
	{
		va_list vl;
		va_start(vl, format);
		WriteSysLog(LOG_DEBUG, format, vl);
		va_end(vl);
	}

	/**
	 * Write the error level log record to syslog (/var/log/messages).
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::SysLogError(const wchar_t* format, ...)
	{
		va_list vl;
		va_start(vl, format);
		WriteSysLog(LOG_ERR, format, vl);
		va_end(vl);
	}

	/**
	 * Write the information level log record to syslog (/var/log/messages).
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::SysLogInfo(const wchar_t* format, ...)
	{
		va_list vl;
		va_start(vl, format);
		WriteSysLog(LOG_INFO, format, vl);
		va_end(vl);
	}

	/**
	 * Write the warning level log record to syslog (/var/log/messages).
	 *
	 * @param	format	C string that contains a format string that follows the same specifications as format in
	 *					printf (see printf for details)
	 * @param	...		(additional arguments) Depending on the format string, the function may expect a sequence
	 *					of additional arguments, each containing a value to be used to replace a format specifier
	 *					in the format string (or a pointer to a storage location, for n). There should be at least
	 *					as many of these arguments as the number of values specified in the format specifiers.
	 *					Additional arguments are ignored by the function.
	 */
	void Logger::SysLogWarn(const wchar_t* format, ...)
	{
		va_list vl;
		va_start(vl, format);
		WriteSysLog(LOG_WARNING, format, vl);
		va_end(vl);
	}

	/**
	 * Release and close all loggers
	 *
	 * @throw	The logger exception with exception details.
	 */
	void Logger::DropAll()
	{
		// Sleep server for 200ms to break the write threads wait
		LoggerUtil::Sleep(2 * SLEEP_IN_MS);

		try {
			worker.DropAll();
		} catch (LoggerException& le) {
			SysLogError(L"Logger::DropAll() error closing stream (%s)", le.GetMsg());
			throw LoggerException(
				LOGGER_EXCEPTION_EXIT,
				LoggerUtil::StrFormat(L"Logger::DropAll() error closing stream (%s)", le.GetMsg()));
		}			
	}

	//! Destructor
	Logger::~Logger()
	{
		try {
			DropAll();
		} catch (LoggerException& le) {
			std::cout << "Logger::~Logger() error closing stream (" << le.GetMsg() << ")" << std::endl;
			syslog(
				LOG_ERR, "Logger::~Logger() error closing stream (%s)", le.GetMsg());
		}
	}
} /* namespace cpplogger */
