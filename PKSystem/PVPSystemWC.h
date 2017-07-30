#ifndef __LYNX_GAME_SERVER_PVPSYSTEMWC_H__
#define __LYNX_GAME_SERVER_PVPSYSTEMWC_H__

#include "../MainThread.h"
#include "../Guild.h"


//ÍõÎì³½
namespace Lynx
{
	class PVPDataWC;
	class PVPSystemWC : public MainThread, public Singleton < PVPSystemWC >
	{
	public:

		PVPSystemWC();
		virtual ~PVPSystemWC();

		bool initial();
		void release();

		void update(const UInt64& accTime);
	
		void addWaitForPVP(UInt64 playerUid, std::string  data);
		void dealConClose(UInt64 playerUid);
		void cancelWaitForPVP(UInt64 playerUid);
		void readyForPVP(UInt64 playerUid);
		void cancelPVPIng(UInt64 playerUid);
		void sendMsgToOther(UInt64 playerUid, std::string res);


	private:

		Map<UInt64, PVPDataWC > m_mapWaitForPVP;
		Map<UInt64, PVPDataWC> m_mapPVPIng;
	
		
	};

	struct PVPDataWC
	{
		PVPDataWC():mSelfUid(0), mOtherUid(0), mData(""), mReady(0){}
		UInt64 mSelfUid;
	    UInt64 mOtherUid;
		String mData;
		UInt32 mReady;
		
	};

} // namespace Lynx


#endif // __LYNX_GAME_SERVER_PVPSYSTEMWC_H__

