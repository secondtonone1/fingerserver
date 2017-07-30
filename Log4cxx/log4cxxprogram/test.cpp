#include <log4cxx/logger.h>    
#include <log4cxx/logstring.h> 
#include <log4cxx/propertyconfigurator.h>

int main(int argc, char* argv[]) 
{ 
        using namespace log4cxx;

        // 读取配置文件 
        PropertyConfigurator::configure("conf.log");

        // 建立两个logger 
        LoggerPtr logger1 = Logger::getLogger("TraceYourMama"); 
        LoggerPtr logger2 = Logger::getLogger("Patch");

        LOG4CXX_TRACE(logger1, "跟踪"); 
        LOG4CXX_WARN(logger1, "警告"); 
        LOG4CXX_DEBUG(logger1, "调试"); 
        LOG4CXX_ASSERT(logger1, false, "断言"); 
        LOG4CXX_FATAL(logger1, "致命");

        LOG4CXX_TRACE(logger2, "跟踪"); 
        LOG4CXX_ERROR(logger2, "错误"); 
        return 0; 
}


