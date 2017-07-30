#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("hello"));

int main(int argc, char *argv[])
{
        log4cxx::PropertyConfigurator::configure("./log4cxx_hello.properties");
        LOG4CXX_INFO(logger, "你好，log4cxx!");
        return 0;
}
