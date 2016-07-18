#include "RecordSystem.h"

using namespace Lynx;

RecordSystem::RecordSystem() : mOutputFile(NULL), mOutputFilePathFolder(""),
mOutputFileName(""), mOutputFileIndex(0), mOutputFilePageSize(10 * 1024 * 1024)
{

}

RecordSystem::~RecordSystem()
{

}

bool 
RecordSystem::initial()
{
    return true;
}

void 
RecordSystem::release()
{

}

