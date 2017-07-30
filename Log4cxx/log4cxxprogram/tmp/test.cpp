
#include<log4cxx/logger.h>

#include<log4cxx/propertyconfigurator.h>

 

using namespace log4cxx;

 

int main()

{

       // Read configure file

PropertyConfigurator::configure("./test.properties");

// Get root logger

LoggerPtr rootLogger = Logger::getRootLogger();

// Log information

LOG4CXX_TRACE(rootLogger, "TRACE");

LOG4CXX_DEBUG(rootLogger, "DEBUG22");

LOG4CXX_WARN(rootLogger, "WARN");

LOG4CXX_INFO(rootLogger, "INFO");

LOG4CXX_ERROR(rootLogger, "ERROR");

      

       return 0;

}
