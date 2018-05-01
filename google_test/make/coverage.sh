#/bin/sh

gcov CMakeFiles/cpplogger_test.dir/home/ec2-user/cpplogger/test/Logger.cpp.gcda
lcov -c -d . -o cpplogger_test.info
genhtml -o html/ cpplogger_test.info
