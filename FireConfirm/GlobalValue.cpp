
#include "GlobalValue.h"
#include "../PlatformLib/FileUtilImpl.h"
#include "InlineActivity.h"



using namespace Lynx;




bool GlobalValueManager::initGlobalValue()
{


	String filePath;
	char* buffer;

	filePath = "Data/globalvalue.lua";	
	int fileLen = FileUtilImpl::loadFile(filePath, &buffer);
	if (fileLen <= 0 )
	{
		LOG_WARN("load file %s false!", filePath.c_str());
		return false;
	}
	String str = (String)buffer;
	
	
	Vector<String> strVector;
	lynxStrSplit(str, "\n", strVector, true);
	Vector<String> strVector1;
	Vector<String> strVector2;
	for(UInt32 i =0;i<strVector.size();i++)
	{
		strVector1.clear();
		strVector2.clear();
		
		lynxStrSplit(strVector[i], "=", strVector1, true);
		
		if(strVector1.size() != 2)
		{
			continue;
		}
		lynxStrTrim(strVector1[0]);


		//begin
		if (checkUTFStringIsSame(strVector1[0],"id"))
		{
			gGlobalValueTemplate.id =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"ids"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.ids);
			continue;			
		}

		//ÃÀÊ³ÎÝ
		if (checkUTFStringIsSame(strVector1[0],"uMSWweight1"))
		{
			gGlobalValueTemplate.uMSWweight1 =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uMSWweight2"))
		{
			gGlobalValueTemplate.uMSWweight2 =  atoi(strVector1[1].c_str());
			continue;
		}

		if (checkUTFStringIsSame(strVector1[0],"uMSWweight3"))
		{
			gGlobalValueTemplate.uMSWweight3 =  atoi(strVector1[1].c_str());
			continue;
		}

		if (checkUTFStringIsSame(strVector1[0],"uMSWmax"))
		{
			gGlobalValueTemplate.uMSWmax =  atoi(strVector1[1].c_str());
			continue;
		}
		
		if (checkUTFStringIsSame(strVector1[0],"uMSWrecover"))
		{
			gGlobalValueTemplate.uMSWrecover =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uMSWeachBuy"))
		{
			gGlobalValueTemplate.uMSWeachBuy =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uMSWprice"))
		{
			gGlobalValueTemplate.uMSWprice =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uMSWtime"))
		{
			gGlobalValueTemplate.uMSWtime =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uMSWrate1"))
		{
			gGlobalValueTemplate.uMSWrate1 =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uMSWrate2"))
		{
			gGlobalValueTemplate.uMSWrate2 =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uMSWrate3"))
		{
			gGlobalValueTemplate.uMSWrate3 =  atoi(strVector1[1].c_str());
			continue;
		}

		//saodang
		if (checkUTFStringIsSame(strVector1[0],"uSDrate"))
		{
			gGlobalValueTemplate.uSDrate =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uSDsweeptimes"))
		{
			gGlobalValueTemplate.uSDsweeptimes =  atoi(strVector1[1].c_str());
			continue;
		}

		if (checkUTFStringIsSame(strVector1[0],"uSDsweepfreetimes"))
		{
			gGlobalValueTemplate.uSDsweepfreetimes =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uSDamount"))
		{
			gGlobalValueTemplate.uSDamount =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uSDprices"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uSDprices);
			continue;			
		}

	
		//specialrelive
		if (checkUTFStringIsSame(strVector1[0],"uSRLdayfreetimes"))
		{
			gGlobalValueTemplate.uSRLdayfreetimes =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uSRLmaxtimes"))
		{
			gGlobalValueTemplate.uSRLmaxtimes =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uSRLcosts"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uSRLcosts);
			continue;
		}		
		if (checkUTFStringIsSame(strVector1[0],"uTRIALFriendRate"))
		{
			gGlobalValueTemplate.uTRIALFriendRate =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTRIALFastRate"))
		{
			gGlobalValueTemplate.uTRIALFastRate =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTRIALGainsRate"))
		{
			gGlobalValueTemplate.uTRIALGainsRate =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTRIALVipNeed"))
		{
			gGlobalValueTemplate.uTRIALVipNeed =  atoi(strVector1[1].c_str());
			continue;
		}

		//twelvepalace
		if (checkUTFStringIsSame(strVector1[0],"uTPdailyreset"))
		{
			gGlobalValueTemplate.uTPdailyreset =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTPneeditemid"))
		{
			gGlobalValueTemplate.uTPneeditemid =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTPcanresetitem"))
		{
			gGlobalValueTemplate.uTPcanresetitem =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTPresetlowerlimit"))
		{
			gGlobalValueTemplate.uTPresetlowerlimit =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTPbuycost"))
		{
			gGlobalValueTemplate.uTPbuycost =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTPbuybosscost"))
		{
			gGlobalValueTemplate.uTPbuybosscost =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTPhornbuycost"))
		{
			gGlobalValueTemplate.uTPhornbuycost =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTPeachdaybuytimes"))
		{
			gGlobalValueTemplate.uTPeachdaybuytimes =  atoi(strVector1[1].c_str());
			continue;
		}

	
		if (checkUTFStringIsSame(strVector1[0],"uTRIALratecosts"))
		{
			gGlobalValueTemplate.uTRIALratecosts =  atoi(strVector1[1].c_str());
			continue;
		}
	
		//climbtower
		if (checkUTFStringIsSame(strVector1[0],"uCLIMBTdailyreset"))
		{
			gGlobalValueTemplate.uCLIMBTdailyreset =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uCLIMBTneeditemid"))
		{
			gGlobalValueTemplate.uCLIMBTneeditemid =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uCLIMBTcanresetitem"))
		{
			gGlobalValueTemplate.uCLIMBTcanresetitem =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uCLIMBTresetlowerlimit"))
		{
			gGlobalValueTemplate.uCLIMBTresetlowerlimit =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uCLIMBTbuycost"))
		{
			gGlobalValueTemplate.uCLIMBTbuycost =  atoi(strVector1[1].c_str());
			continue;
		}

		//strength
		if (checkUTFStringIsSame(strVector1[0],"uSTmax"))
		{
			gGlobalValueTemplate.uSTmax =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uSTtime"))
		{
			gGlobalValueTemplate.uSTtime =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uSTprices"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uSTprices);
			continue;
		}
		//mysticalshopcost	
		if (checkUTFStringIsSame(strVector1[0],"uMSHOPrefreshtimes"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uMSHOPrefreshtimes);
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uSSHOPrefreshtimes"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uSSHOPrefreshtimes);
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uSSHOPbuytimes"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uSSHOPbuytimes);
			continue;
		}

		if (checkUTFStringIsSame(strVector1[0],"uCOINSHOPrefreshtimes"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uCOINSHOPrefreshtimes);
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uCOURAGESHOPrefreshtimes"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uCOURAGESHOPrefreshtimes);
			continue;
		}
		
	
		//territorylucky
		if (checkUTFStringIsSame(strVector1[0],"uTERRITORYLrefreshs"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uTERRITORYLrefreshs);
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTERRITORYLcosts"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uTERRITORYLcosts);
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTERRITORYLbuytimess"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uTERRITORYLbuytimess);
			continue;
		}

		if (checkUTFStringIsSame(strVector1[0],"uTERRITORYLmaxtimes"))
		{
			gGlobalValueTemplate.uTERRITORYLmaxtimes =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTERRITORYLrecovertime"))
		{
			gGlobalValueTemplate.uTERRITORYLrecovertime =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTERRITORYLchallage"))
		{
			gGlobalValueTemplate.uTERRITORYLchallage =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTERRITORYLfreerefreshtimes"))
		{
			gGlobalValueTemplate.uTERRITORYLfreerefreshtimes =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTERRITORYLfreerefresh"))
		{
			gGlobalValueTemplate.uTERRITORYLfreerefresh =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uTERRITORYLrmbrefresh"))
		{
			gGlobalValueTemplate.uTERRITORYLrmbrefresh =  atoi(strVector1[1].c_str());
			continue;
		}

		//gainwealth
		if (checkUTFStringIsSame(strVector1[0],"uGAINWEALTHfreetimes"))
		{
			gGlobalValueTemplate.uGAINWEALTHfreetimes =  atoi(strVector1[1].c_str());
			continue;
		}

		//growthfound
		if (checkUTFStringIsSame(strVector1[0],"GROWTHFOUNDviplvneed"))
		{
			gGlobalValueTemplate.GROWTHFOUNDviplvneed =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"GROWTHFOUNDcost"))
		{
			gGlobalValueTemplate.GROWTHFOUNDcost =  atoi(strVector1[1].c_str());
			continue;
		}

		if (checkUTFStringIsSame(strVector1[0],"uReliveBaseCost"))
		{
			gGlobalValueTemplate.uReliveBaseCost =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uReliveRatio"))
		{
			gGlobalValueTemplate.uReliveRatio =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uReliveMaxCost"))
		{
			gGlobalValueTemplate.uReliveMaxCost =  atoi(strVector1[1].c_str());
			continue;
		}
	
		if (checkUTFStringIsSame(strVector1[0],"uRelivePlayer2BaseCost"))
		{
			gGlobalValueTemplate.uRelivePlayer2BaseCost =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uRelivePlayer2Ratio"))
		{
			gGlobalValueTemplate.uRelivePlayer2Ratio =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uRelivePlayer2MaxCost"))
		{
			gGlobalValueTemplate.uRelivePlayer2MaxCost =  atoi(strVector1[1].c_str());
			continue;
		}	
		if (checkUTFStringIsSame(strVector1[0],"uFISHEATmaxtimes"))
		{
			gGlobalValueTemplate.uFISHEATmaxtimes =  atoi(strVector1[1].c_str());
			continue;
		}	
		if (checkUTFStringIsSame(strVector1[0],"uFISHEATgivecoin"))
		{
			gGlobalValueTemplate.uFISHEATgivecoin =  atoi(strVector1[1].c_str());
			continue;
		}	
		if (checkUTFStringIsSame(strVector1[0],"uFISHEATeatfish"))
		{
			gGlobalValueTemplate.uFISHEATeatfish =  atoi(strVector1[1].c_str());
			continue;
		}	
		if (checkUTFStringIsSame(strVector1[0],"uFISHEATmaxpresscount"))
		{
			gGlobalValueTemplate.uFISHEATmaxpresscount =  atoi(strVector1[1].c_str());
			continue;
		}	

	
		if (checkUTFStringIsSame(strVector1[0],"uRankGameBuyTimesCost"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uRankGameBuyTimesCost);
			continue;
		}

		if (checkUTFStringIsSame(strVector1[0],"uRankGameTimes"))
		{
			gGlobalValueTemplate.uRankGameTimes =  atoi(strVector1[1].c_str());
			continue;
		}

	
		if (checkUTFStringIsSame(strVector1[0],"uRankGameNum"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uRankGameNum);
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uRankGamewinScore"))
		{
			gGlobalValueTemplate.uRankGamewinScore =  atoi(strVector1[1].c_str());
			continue;
		}	
		if (checkUTFStringIsSame(strVector1[0],"uRankGameFailScoreCost"))
		{
			gGlobalValueTemplate.uRankGameFailScoreCost =  atoi(strVector1[1].c_str());
			continue;
		}	
		if (checkUTFStringIsSame(strVector1[0],"uRankGameReportNum"))
		{
			gGlobalValueTemplate.uRankGameReportNum =  atoi(strVector1[1].c_str());
			continue;
		}	
		if (checkUTFStringIsSame(strVector1[0],"uRankGameTime"))
		{
			gGlobalValueTemplate.uRankGameTime =  atoi(strVector1[1].c_str());
			continue;
		}

		if (checkUTFStringIsSame(strVector1[0],"uRankGameShopRefreshTimes"))
		{
			gGlobalValueTemplate.uRankGameShopRefreshTimes =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uRankGameShopRefreshTime"))
		{
			gGlobalValueTemplate.uRankGameShopRefreshTime =  atoi(strVector1[1].c_str());
			continue;
		}

		if (checkUTFStringIsSame(strVector1[0],"uRankGameShopRefreshCost"))
		{
			gGlobalValueTemplate.uRankGameShopRefreshCost =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uRankGameOpenLevel"))
		{
			gGlobalValueTemplate.uRankGameOpenLevel =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uRankGameRewardResetTime"))
		{
			gGlobalValueTemplate.uRankGameRewardResetTime =  atoi(strVector1[1].c_str());
			continue;
		}
		if (checkUTFStringIsSame(strVector1[0],"uRankGameMaxFinishTime"))
		{
			gGlobalValueTemplate.uRankGameMaxFinishTime =  atoi(strVector1[1].c_str());
			continue;
		}	
	
		if (checkUTFStringIsSame(strVector1[0],"uCatTaskInitGold"))
		{
			gGlobalValueTemplate.uCatTaskInitGold =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uCatTaskAddGold"))
		{
			gGlobalValueTemplate.uCatTaskAddGold =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uCatTaskMaxGold"))
		{
			gGlobalValueTemplate.uCatTaskMaxGold =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uBusinessCatRefreshInit"))
		{
			gGlobalValueTemplate.uBusinessCatInitGold =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uBusinessCatRefreshAdd"))
		{
			gGlobalValueTemplate.uBusinessCatAddGold =  atoi(strVector1[1].c_str());
			continue;
		}

		if (checkUTFStringIsSame(strVector1[0],"uBusinessCatRefreshMax"))
		{
			gGlobalValueTemplate.uBusinessCatMaxGold =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uGuildOpenBattle"))
		{
			gGlobalValueTemplate.uGuildOpenBattle =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uGuildOpenYuanWai"))
		{
			gGlobalValueTemplate.uGuildOpenYuanWai =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uGuildOpenBusinessCat"))
		{
			gGlobalValueTemplate.uGuildOpenBusinessCat =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uGuildOpenKitchen"))
		{
			gGlobalValueTemplate.uGuildOpenKitchen =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uGuildOpenTicket"))
		{
			gGlobalValueTemplate.uGuildOpenTicket =  atoi(strVector1[1].c_str());
			continue;
		}	
	
		if (checkUTFStringIsSame(strVector1[0],"uGuildOpenSign"))
		{
			gGlobalValueTemplate.uGuildOpenSign =  atoi(strVector1[1].c_str());
			continue;
		}	
	
		if (checkUTFStringIsSame(strVector1[0],"uGuildOpenCatTask"))
		{
			gGlobalValueTemplate.uGuildOpenCatTask =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uSystemRefreshTime"))
		{
			getListFromString(strVector1[1],gGlobalValueTemplate.uSystemRefreshTime);
			continue;
		}


		if (checkUTFStringIsSame(strVector1[0],"sRole1"))
		{
			gGlobalValueTemplate.sRole1 =  strVector1[1].c_str();
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"sRole2"))
		{
			gGlobalValueTemplate.sRole2 =  strVector1[1].c_str();
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"sRole3"))
		{
			gGlobalValueTemplate.sRole3 =  strVector1[1].c_str();
			continue;
		}	
		if (checkUTFStringIsSame(strVector1[0],"uRolePower1"))
		{
			gGlobalValueTemplate.uRolePower1 =  atoi(strVector1[1].c_str());
			continue;
		}	
		if (checkUTFStringIsSame(strVector1[0],"uRolePower2"))
		{
			gGlobalValueTemplate.uRolePower2 =  atoi(strVector1[1].c_str());
			continue;
		}	
		if (checkUTFStringIsSame(strVector1[0],"uRolePower3"))
		{
			gGlobalValueTemplate.uRolePower3 =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uCopyFinishTimeDeviation"))
		{
			gGlobalValueTemplate.uCopyFinishTimeDeviation =  atoi(strVector1[1].c_str());
			continue;
		}	
		
		if (checkUTFStringIsSame(strVector1[0],"uConsortSignReset"))
		{
			gGlobalValueTemplate.uConsortSignReset =  atoi(strVector1[1].c_str());
			continue;
		}	
		
		if (checkUTFStringIsSame(strVector1[0],"uTicketFriendReset"))
		{
			gGlobalValueTemplate.uTicketFriendReset =  atoi(strVector1[1].c_str());
			continue;
		}	
	
		if (checkUTFStringIsSame(strVector1[0],"uEyesightReset"))
		{
			gGlobalValueTemplate.uEyesightReset =  atoi(strVector1[1].c_str());
			continue;
		}	
	
		if (checkUTFStringIsSame(strVector1[0],"uKitchenReset"))
		{
			gGlobalValueTemplate.uKitchenReset =  atoi(strVector1[1].c_str());
			continue;
		}	
		
		if (checkUTFStringIsSame(strVector1[0],"uXingXiaReset"))
		{
			gGlobalValueTemplate.uXingXiaReset =  atoi(strVector1[1].c_str());
			continue;
		}	
	
		if (checkUTFStringIsSame(strVector1[0],"uRewardLotteryFreeOnceMaxTimes"))
		{
			gGlobalValueTemplate.uRewardLotteryFreeOnceMaxTimes =  atoi(strVector1[1].c_str());
			continue;
		}	


		if (checkUTFStringIsSame(strVector1[0],"uEloquenceReset"))
		{
			gGlobalValueTemplate.uEloquenceReset =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"UWoodencatReset"))
		{
			gGlobalValueTemplate.uWoodencatReset =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uWoodencatattackbuffcost"))
		{
			gGlobalValueTemplate.uWoodencatattackbuffcost =  atoi(strVector1[1].c_str());
			continue;
		}	

	
		if (checkUTFStringIsSame(strVector1[0],"uWoodencatdefensebuffcost"))
		{
			gGlobalValueTemplate.uWoodencatdefensebuffcost =  atoi(strVector1[1].c_str());
			continue;
		}	

		if (checkUTFStringIsSame(strVector1[0],"uWoodencatCritbuffcost"))
		{
			gGlobalValueTemplate.uWoodencatCritbuffcost =  atoi(strVector1[1].c_str());
			continue;
		}	
	
		
	}
	return true;
}

bool GlobalValueManager::checkUTFStringIsSame(String uString,String str)
{
	if (uString.c_str() == InlineActivityManager::getSingleton().stringToUtf(str))
	{
		return true;
	}

	return false;
}

void GlobalValueManager::getListFromString(String str,List<UInt32> &list)
{
	Vector<String> strVector1;
	Vector<String> strVector2;

	lynxStrTrim(str);

	lynxStrSplit(str, "{", strVector2, true);
	strVector1.clear();
	if (strVector2.size() < 1)
	{
		return;
	}
	lynxStrSplit(strVector2[0], "}", strVector1, true);
	strVector2.clear();
	if (strVector1.size() < 1)
	{
		return;
	}
	lynxStrSplit(strVector1[0], ",", strVector2, true);
	for(UInt32 j = 0;j<strVector2.size();j++)
	{
		list.insertTail(lynxAtoi<UInt32>(strVector2[j].c_str()));
	}
}




GlobalValue &GlobalValueManager::getGlobalValue()
{
	return gGlobalValueTemplate;
}

GlobalValue *GlobalValueManager::getGlobalValueData()
{
	return &gGlobalValueTemplate;
}