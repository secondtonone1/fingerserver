
#include<log4cxx/logger.h>

#include<log4cxx/propertyconfigurator.h>

 

using namespace log4cxx;

 

int main()

{

       // Read configure file

PropertyConfigurator::configure("./file.properties");

// Get root logger

LoggerPtr rootLogger = Logger::getRootLogger();



// Log information

LOG4CXX_TRACE(rootLogger, "TRACE");
for(int i=0;i<100000;i++)
{
	LOG4CXX_WARN(rootLogger,"write log 11111111111111111111111111111111111111111111111111111111112222222222222222 DEBUG");
}

LOG4CXX_DEBUG(rootLogger, "DEBUG");

LOG4CXX_WARN(rootLogger, "WARN");

LOG4CXX_INFO(rootLogger, "INFO");

LOG4CXX_ERROR(rootLogger, "ERROR");

      

       return 0;

}
