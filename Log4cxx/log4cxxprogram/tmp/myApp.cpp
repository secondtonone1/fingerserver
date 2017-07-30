#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

LoggerPtr logger(Logger::getLogger("MyApp"));

int main(int argc, char **argv)
{
    int result = 0;
    try
    {
        BasicConfigurator::configure();
        LOG4CXX_INFO(logger,"Entering application");
    }
    catch(Exception&)
    {
        result = 1;
    }
    return result;
}
