#ifndef __LYNX_GAME_MASTER_TOOL_RECORD_SYSTEM_H__
#define __LYNX_GAME_MASTER_TOOL_RECORD_SYSTEM_H__

#include "CommonLib.h"

namespace Lynx
{
    class RecordSystem : public Singleton<RecordSystem>
    {
    public:
        RecordSystem();
        virtual ~RecordSystem();

        bool initial();
        void release();

    private:
       FILE* mOutputFile;
       String mOutputFilePathFolder;
       String mOutputFileName;
       UInt32 mOutputFileIndex;
       UInt32 mOutputFilePageSize;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_RECORD_SYSTEM_H__

