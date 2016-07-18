#include "ItemMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void 
ItemMsgHandler::onUseItemReq(const ConnId& connId, UseItemReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by ConnId: %llu", connId);
        return;
    }
    UseItemResp resp;
    resp.merrorId = player->getItemManager().useItem(msg.mItemId, msg.mCount);
}

void 
ItemMsgHandler::onSellItemReq(const ConnId& connId, SellItemReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by ConnId: %llu", connId);
        return;
    }
	
	Json::Value root;
	Json::Reader reader;

	List<UInt64> sellItemList;
	if(reader.parse(msg.mReqJsonStr,root))
	{
		for(UInt32 i = 0 ; i < root["sellItemList"].size(); i++)
		{
			UInt64 itemUid = root["sellItemList"].asUInt64();
			sellItemList.insertTail(itemUid);
		}

		SellItemResp resp;
		resp.merrorId = player->getItemManager().sellItem(sellItemList, resp.mGetGold);
		player->sendMsgWithAck(resp);
	}
	else
	{
		LOG_INFO("empty message polishequipreq!");
	}
	
   
}

void ItemMsgHandler::onPolishEquipReq(const ConnId& connId, EquipPolishReq& msg)
{
	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Value root;
	Json::Reader reader;

	if(reader.parse(reqJsonStr,root))
	{
		UInt64 objEquipUid = root["objEquip"].asUInt64();
		UInt64 materialEquipUid = root["materialEquip"].asUInt64();
		UInt32 objAttrIndex = root["objAttrIndex"].asUInt();
		
		Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
		if (!player)
		{
			LOG_WARN("Failed to get Player by ConnId: %llu", connId);
			return;
		}

		player->getItemManager().polishEquip(objEquipUid,materialEquipUid,objAttrIndex);
	}
	else
	{
		LOG_INFO("empty message polishequipreq!");
	}
	
}

void ItemMsgHandler::onEhanceEquipReq(const ConnId& connId, EquipEhanceReq& msg)
{
	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Value root;  
	
	Json::Reader reader;  
	
	List<UInt64> m_nMaterialList;
	UInt64 m_nEquipID;
	if (reader.parse(reqJsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{  
		m_nEquipID = root["equip"].asUInt64();  // 访问节点，equip  
		
		for(int i = 0; i < root["material"].size(); i++)
		{
			UInt64 id = root["material"][i].asUInt64();
			m_nMaterialList.insertTail(id);
		}

		Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
		if (!player)
		{
			LOG_WARN("Failed to get Player by ConnId: %llu", connId);
			return;
		}
		
		player->getItemManager().enhanceEquip(m_nEquipID, m_nMaterialList);

	}
	else
	{	
		LOG_WARN("Empty message", connId);
	}

}

void ItemMsgHandler::onPolishResetEquipReq(const ConnId& connId, EquipPolishResetReq& msg)
{
	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Value root;  

	Json::Reader reader;  
	
	if (reader.parse(reqJsonStr, root))
	{
		UInt64 equipUid = root["objEquipUid"].asUInt64();
		UInt32 attrIndex = root["objRdAttrIndex"].asUInt();

		Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
		if (!player)
		{
			LOG_WARN("Failed to get Player by ConnId: %llu", connId);
			return;
		}

		player->getItemManager().polishResetEquip(equipUid);
		
	}
	else
	{
		LOG_INFO("empty message polishresetEquipReq!");
	}
}

void ItemMsgHandler::onGemCombineReq(const ConnId& connId, GemCombineReq& msg)
{
	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Value root;

	Json::Reader reader;

	if (reader.parse(reqJsonStr, root))
	{
		UInt64 gemItemId = root["gemItemId"].asUInt64();
		UInt32 count = root["count"].asUInt();
		
		Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
		if (!player)
		{
			LOG_WARN("Failed to get Player by ConnId: %llu", connId);
			return;
		}

		player->getItemManager().combineGemsList(gemItemId,count);

	}
	else
	{
		LOG_INFO("empty message gemCombineReq!");
	}

}

void ItemMsgHandler::onGemCombineOnceReq(const ConnId& connId, GemCombineOnceReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	player->getItemManager().gemCombineOnce();
}


void ItemMsgHandler::onGemSetReq(const ConnId& connId, GemSetReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Value root;

	Json::Reader reader;

	if (reader.parse(reqJsonStr, root))
	{
		UInt64 equipUid = root["equipUid"].asUInt64();
		UInt32 gemSlot = root["equipGemSlot"].asUInt();
		UInt64 gemUid = root["gemUid"].asUInt64();

		Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
		if (!player)
		{
			LOG_WARN("Failed to get Player by ConnId: %llu", connId);
			return;
		}

		player->getItemManager().gemSetEquip(equipUid,gemSlot,gemUid);

	}
	else
	{
		LOG_INFO("empty message gemCombineReq!");
	}

	
}


void ItemMsgHandler::onEquipGemCombineReq(const ConnId& connId, EquipGemCombineReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Value root;

	Json::Reader reader;

	if (reader.parse(reqJsonStr, root))
	{
		UInt64 equipUid = root["equipUid"].asUInt64();
		UInt32 gemSlot = root["equipGemSlot"].asUInt();
		
		Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
		if (!player)
		{
			LOG_WARN("Failed to get Player by ConnId: %llu", connId);
			return;
		}

		player->getItemManager().equipGemCombine(equipUid,gemSlot);

	}
	else
	{
		LOG_INFO("empty message gemCombineReq!");
	}

}

void ItemMsgHandler::onGemUnloadReq(const ConnId& connId, GemUnloadReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Value root;

	Json::Reader reader;

	if (reader.parse(reqJsonStr, root))
	{
		UInt64 equipUid = root["equipUid"].asUInt64();
		UInt32 gemSlot = root["equipGemSlot"].asUInt();

		Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
		if (!player)
		{
			LOG_WARN("Failed to get Player by ConnId: %llu", connId);
			return;
		}

		player->getItemManager().equipGemUnload(equipUid,gemSlot);

	}
	else
	{
		LOG_INFO("empty message gemUnloadReq!");
	}

}