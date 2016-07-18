#include "BuffManager.h"
#include "Player.h"
using namespace Lynx;

BuffManager::BuffManager()
{
	m_pListBuffers = NULL;
	m_pPlayer = NULL;
}


BuffManager:: ~BuffManager()
{
	
}

bool BuffManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	m_pListBuffers = &m_pPlayer->mPlayerData.mBuffListData.m_listBuffers;

	buffMapInitial();

	return true;

}


void BuffManager::release()
{
	m_pPlayer = NULL;
	m_pListBuffers = NULL;
	m_mapIDToBuffData.clear();
}

std::string BuffManager::convertBuffDataToJson()
{
	MsgBuffDataToJson buffDataToJson;
	buffDataToJson.m_mapIDToBuffData = m_mapIDToBuffData;
	return buffDataToJson.convertDataToJson();
	
}


void BuffManager::buffMapInitial()
{
	for(List<BufferData>::Iter * iter = m_pListBuffers->begin(); iter != NULL; iter = m_pListBuffers->next(iter))
	{
		if(iter->mValue.m_nBufferID)
		{
			m_mapIDToBuffData.insert(iter->mValue.m_nBufferID,&iter->mValue);
		}
	}
	
}



// function T_Buff:getTypes()		-- include type and subtype	
// if self.buffData == nil then
// return nil, nil
// end
// return self.buffData.buffType, self.buffData.subtype
// end
BuffType BuffManager::gettypes(BuffTemplate bufferData)
{
	BuffType buffType;
	buffType.buffBigType = bufferData.mType;
	buffType.buffSubType = bufferData.mSubType;
	return buffType;
}

// function T_Buff:update( t )
// if self.buffData == nil then
// print("self.buffData ")
// return
// end
// 
// if self.timeover == true then
// print("self.timeover ")
// return
// end
// 
// self.buffCD = self.buffCD - t
// if self.buffCD <= 0 then 
// self.buffCD = 0
// self.timeover = true
// -- self:onBuffEnd()  -- 在removeBuff时已经触发 进行了两次触发
// print("self.timeover 123323")
// return
// end
// 
// self:update_logic(t)
// 
// 	 end


void BuffManager::update(Buffer buffer)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	if (buffer.timeOver == true)
	{
		return;
	}
	buffer.bufferCD = buffer.bufferCD - nowTime;
	if (buffer.bufferCD > nowTime)
	{
		buffer.timeOver = true;
		return;
	}
//	update_logic(t);//todo send client
	 

}



// 	 function T_Buff:update_logic( t )
// 	 if self.logicObj ~= nil then
// 		 self.logicObj:update(t)
// 		 end
// 		 end
void BuffManager::updateLogic(Buffer buffer)
{

	update(buffer);
}


void BuffManager::reset(Buffer buffer)
{

	buffer.bufferCD = buffer.bufferData.mKeepTime;
// 	self.buffCD = self.buffData.keepTime//todo
// self:onBuffStartAffect()
}
// 	 function T_Buff:getBuffParam( idx )
// 	 if (idx <= 0) or (idx > GameDefine.MAX_BUFF_CONFIG_PARAM_NUM) then
// 		 return nil
// 		 end
// 		 return self.buffData.param[idx]
// 	 end
String BuffManager::getBuffParam(Buffer buffer,UInt32 id)
{
	if (id == 1)
	{
		return buffer.bufferData.mParam1;
	}

	return "";
}

// 		 function T_Buff:onBuffStartAffect(  )
// 		 if self.logicObj ~= nil then
// 			 self.logicObj:onBuffStart()
// 			 end
// 			 end

void BuffManager::onBuffStartAffect(Buffer buffer)
{

// 	onbuffstart
}
// 
// 			 function T_Buff:onBuffInterrupt(  )
// 			 if self.logicObj ~= nil then
// 				 self.logicObj:onBuffInterrupt()
// 				 end
// 				 end
void BuffManager::onBuffInterrupt(  )
{

}

// 				 function T_Buff:onBuffEffectChange(  )
// 				 if self.logicObj ~= nil then
// 					 self.logicObj:onBuffEffectChange()
// 					 end
// 					 end
void BuffManager::onBuffEffectChange(  )
{

}

// 					 function T_Buff:onBuffEnd(  )
// 
// 					 if self.logicObj ~= nil then
// 						 self.logicObj:onBuffEnd()
// 						 end
// 						 end
// 
// 
// 						 return T_Buff
void BuffManager::onBuffEnd(  )
{

}

bool  BuffManager::checkBuffExistById(UInt32 buffID)
{
	return true;

}
