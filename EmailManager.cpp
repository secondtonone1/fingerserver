#include "EmailManager.h"
#include "PersistSystem.h"
#include "Player.h"
using namespace Lynx;

EmailManager::EmailManager()
{
	
}


EmailManager:: ~EmailManager()
{

}

bool EmailManager::initial(Player* player)
{
	m_pPlayer = player;

	m_pEmailList = &player->mPlayerData.mEmailData.emailList;

	for(List<EmailData>::Iter * listIter = m_pEmailList->begin(); listIter != NULL;  listIter = m_pEmailList->next(listIter) )
	{
		m_emailMap.insert(listIter->mValue.m_nEmailUid, &(listIter->mValue) );

	}
	

	return true;
}


void EmailManager::release()
{
	m_pEmailList = NULL;
	m_emailMap.clear();
}


void EmailManager::addEmail(const EmailData & emailData)
{
	List<EmailData>::Iter * insertIter = m_pEmailList->insertTail(emailData);
	m_emailMap.insert(emailData.m_nEmailUid, &insertIter->mValue);
}

void EmailManager::delEmail(UInt64 emailUid)
{
	Map<UInt64, EmailData*>::Iter * findIter = m_emailMap.find(emailUid);
	if(!findIter)
	{
		return ;
	}

	for(List<EmailData>::Iter * listIter = m_pEmailList->begin(); listIter != NULL; listIter = m_pEmailList->next(listIter) )
	{
		if(listIter->mValue.m_nEmailUid == emailUid)
		{
			m_pEmailList->erase(listIter);
			break;
		}
	}

	m_emailMap.erase(findIter);

	PersistDelEmail emailDelMsg;
	emailDelMsg.mEmailUid = emailUid;
	PersistSystem::getSingleton().postThreadMsg(emailDelMsg, emailUid);
}

void EmailManager::updateEmailState(const EmailData & emailData)
{
	Map<UInt64, EmailData*>::Iter * findIter = m_emailMap.find(emailData.m_nEmailUid);
	if(!findIter)
	{
		return ;
	}

	findIter -> mValue-> m_nTempId = emailData.m_nTempId;
	findIter ->mValue-> m_nOpenState = emailData.m_nOpenState;
	findIter ->mValue-> m_nGetState = emailData.m_nGetState;
	findIter ->mValue-> m_strDes = emailData.m_strDes;
	findIter ->mValue-> m_strContent = emailData.m_strContent;
	findIter->mValue-> m_strFrom = emailData.m_strFrom;
	findIter->mValue-> m_strTittle = emailData.m_strTittle;
	findIter->mValue->m_nTime = emailData.m_nTime;


}

void EmailManager::clearAllEmail()
{
	m_pEmailList->clear();
	m_emailMap.clear();
	PersistClearEmail clearEmail;
	clearEmail.mPlayerUid = m_pPlayer->getPlayerGuid();
	PersistSystem::getSingleton().postThreadMsg(clearEmail, clearEmail.mPlayerUid);
}

void EmailManager::openEmail(UInt64 emailUid)
{
	Map<UInt64, EmailData*>::Iter * findIter = m_emailMap.find(emailUid);
	if(!findIter)
	{
		return ;
	}

	findIter->mValue->m_nOpenState = 1;
	PersistUpdateEmail updateEmail;
	updateEmail.mEmailData = *(findIter->mValue);
	PersistSystem::getSingleton().postThreadMsg(updateEmail,emailUid);
}


std::string EmailManager::convertDataToJson()
{
	Json::Value root;
	
	for(List<EmailData>::Iter * listIter = m_pEmailList->begin(); listIter != NULL;  listIter = m_pEmailList->next(listIter) )
	{
		Json::Value emailRoot;
		emailRoot["emailuid"] = listIter->mValue.m_nEmailUid;
		emailRoot["tempid"] = listIter->mValue.m_nTempId;
		emailRoot["openState"] = listIter->mValue.m_nOpenState;
		emailRoot["getstate"] = listIter->mValue.m_nGetState;
		emailRoot["des"] = listIter->mValue.m_strDes.c_str();
		emailRoot["contant"] = listIter->mValue.m_strContent.c_str();
		emailRoot["from"] = listIter->mValue.m_strFrom.c_str();
		emailRoot["title"] = listIter->mValue.m_strTittle.c_str();
		emailRoot["time"] = listIter->mValue.m_nTime;	
		
		root.append(emailRoot);
	}

	Json::StyledWriter writer;
	 return writer.write(root);

}

void EmailManager::getContant(UInt64 emailUid)
{
	const ConnId & connId  = m_pPlayer->getConnId();
	Map<UInt64, EmailData*>::Iter * findIter = m_emailMap.find(emailUid);

	if(!findIter)
	{
		EmailGetContantResp getContantResp;
		getContantResp.mPacketID = EMAIL_GETCONTANT_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::EmailNotExist;
		Json::StyledWriter writer;
		getContantResp.mRespJsonStr = writer.write(root);
		cout << getContantResp.mRespJsonStr;

		return;
	}

	
		string contantStr = findIter->mValue->m_strContent.c_str();
		
		/////////////////////////////////////////////////
		
		std::string ::size_type  findIndex = contantStr.find(';');
		std::stringstream mystream;

		Map<UInt64, List<UInt64> > mapRes2Sub;
		List<JewelryData *> listJewelryData;
	
		while(findIndex != std::string::npos)
		{
			dealSubStr(contantStr,findIndex, mapRes2Sub, listJewelryData);
		   contantStr = contantStr.substr(findIndex +1);
	       findIndex = contantStr.find(';');
		}

		if(contantStr != "")
		{
			dealSubStr(contantStr,findIndex,mapRes2Sub,listJewelryData);
		}

	findIter->mValue->m_nOpenState = 1;
	findIter->mValue->m_nGetState = 1;
	PersistUpdateEmail updateEmail;
	updateEmail.mEmailData = *(findIter->mValue);
	PersistSystem::getSingleton().postThreadMsg(updateEmail, emailUid);

	EmailGetContantResp getContantResp;
		getContantResp.mPacketID = EMAIL_GETCONTANT_RESP;
	
		getContantResp.mRespJsonStr = convertGetToJson(mapRes2Sub,listJewelryData);
		cout << getContantResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(getContantResp,connId);
}

void EmailManager::getContant(List<UInt64> emailList)
{
	const ConnId & connId  = m_pPlayer->getConnId();	
	Map<UInt64, List<UInt64> > mapRes2Sub;
		List<JewelryData *> listJewelryData;

	for(List<UInt64>::Iter * emailIter = emailList.begin();  emailIter != NULL; emailIter = emailList.next(emailIter))
	{
		Map<UInt64, EmailData*>::Iter * findIter = m_emailMap.find(emailIter->mValue);

		if(!findIter)
		{
			continue;
		}

		string contantStr = findIter->mValue->m_strContent.c_str();
		
		/////////////////////////////////////////////////
		
		std::string ::size_type  findIndex = contantStr.find(';');
		std::stringstream mystream;

		while(findIndex != std::string::npos)
		{
			dealSubStr(contantStr,findIndex, mapRes2Sub, listJewelryData);
		    contantStr = contantStr.substr(findIndex +1);
	        findIndex = contantStr.find(';');
		}

		if(contantStr != "")
		{
			dealSubStr(contantStr,findIndex,mapRes2Sub,listJewelryData);
		}

		findIter->mValue->m_nOpenState = 1;
		findIter->mValue->m_nGetState = 1;
		PersistUpdateEmail updateEmail;
		updateEmail.mEmailData = *(findIter->mValue);
		PersistSystem::getSingleton().postThreadMsg(updateEmail, emailIter->mValue);

	}


	EmailGetContantResp getContantResp;
		getContantResp.mPacketID = EMAIL_GETCONTANT_RESP;
	
		getContantResp.mRespJsonStr = convertGetToJson(mapRes2Sub,listJewelryData);
		cout << getContantResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(getContantResp,connId);
}


void EmailManager::dealSubStr(std::string &contantStr, std::string ::size_type  findIndex,Map<UInt64, List<UInt64> > &mapRes2Sub, List<JewelryData *> &listJewelryData)
{
		//;号之前的全部截取，放入到列表里
			std::string contantEleStr = contantStr.substr(0,findIndex);
			//去括号
			contantEleStr = contantEleStr.substr(1,contantEleStr.length()-2);
			std::string ::size_type firstNode = contantEleStr.find(',');

			std::string resTypeStr = contantEleStr.substr(0,firstNode);
			contantEleStr = contantEleStr.substr(firstNode+1);

			std::string ::size_type secondNode = contantEleStr.find(',');

			std::string subTypeStr = contantEleStr.substr(0,secondNode);
			std::string countStr = contantEleStr.substr(secondNode+1);

			std::stringstream mystream;
			mystream.clear();
			mystream.str("");

			mystream << resTypeStr;
			UInt64 resType;
			mystream >> resType;

			mystream.clear();
			mystream.str("");

			UInt64 subType = 0;
			mystream << subTypeStr;
			mystream >> subType;

			mystream.clear();
			mystream.str("");

			mystream << countStr;
			UInt32 count = 0;
			mystream >> count;

			if(resType != AWARD_JEWELRY)
			{
				m_pPlayer->getAllItemManager().addAwardMaterial(resType,subType,count);

				Map<UInt64, List<UInt64> >::Iter * mapIterFind = mapRes2Sub.find(resType);
				
				if(mapIterFind)
				{
					List<UInt64>& listFind = mapIterFind->mValue;
					List<UInt64>::Iter * listIter = listFind.find(subType, listFind.begin(), listFind.end());
					
					if(!listIter)
					{
						listFind.insertTail(subType);
					}
				
				}
				else
				{
					List<UInt64> listFind;
					listFind.insertTail(subType);
					mapRes2Sub.insert(resType, listFind);
				}
			}
			else
			{
				listJewelryData += m_pPlayer->getJewelryManager().playerGetJewelry(subType,count);
			}


}


std::string EmailManager::convertGetToJson(Map<UInt64, List<UInt64> > mapRes2Sub, List<JewelryData *> listJewelryData)
{
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	for(Map<UInt64, List<UInt64> >::Iter * mapIter = mapRes2Sub.begin();  mapIter != NULL; mapIter = mapRes2Sub.next(mapIter))
	{
		UInt32 resType = mapIter->mKey;
		if(resType == AWARD_SERVANT || resType == AWARD_SERVANTPIECE)
		{
			for(List<UInt64>::Iter * listIter = mapIter->mValue.begin(); listIter != NULL; listIter = mapIter->mValue.next(listIter) )
			{
				ServantData * servantData = m_pPlayer->getServantManager().getServantById(listIter->mValue);
				Json::Value emailRoot;
				emailRoot["resType"] = resType;
				emailRoot["subType"] = listIter->mValue;
				emailRoot["count"] = servantData->pieceCount;
				

				////////////////////////////////////////////////////////////////////////
				
				emailRoot["attr"]["servantUid"] = servantData->servantUid;
				emailRoot["attr"]["servantId"] = servantData->servantId;
				emailRoot["attr"]["pieceCount"] = servantData->pieceCount;
				emailRoot["attr"]["level"] = servantData->level;
				emailRoot["attr"]["lvexp"] = servantData->lvexp;
				emailRoot["attr"]["star"] = servantData->star;
				emailRoot["attr"]["floor"] = servantData->floor;

				for(List<UInt64>::Iter * treasureIter = servantData->equipTreasures.begin(); treasureIter != NULL;
					treasureIter = servantData->equipTreasures.next(treasureIter))
				{
					emailRoot["attr"]["equipTreasures"].append(treasureIter->mValue);
				}

				root["data"].append(emailRoot);

				//////////////////////////////////////////////////////////////////////

			}
			
		}
		else
		{
	
			for(List<UInt64>::Iter * listIter = mapIter->mValue.begin(); listIter != NULL; listIter = mapIter->mValue.next(listIter) )
			{
				UInt32 count = m_pPlayer->getAllItemManager().getAwardCount(mapIter->mKey, listIter->mValue);
				Json::Value emailRoot;
				emailRoot["resType"] = mapIter->mKey;
				emailRoot["subType"] = listIter->mValue;
				emailRoot["count"] = count;

				root["data"].append(emailRoot);
			

			}

			
		}

	}

	for(List<JewelryData *> ::Iter * jewelryIter = listJewelryData.begin();  jewelryIter != NULL; 
		jewelryIter = listJewelryData.next(jewelryIter) )
	{		
			Json::Value emailRoot;
			emailRoot["resType"] = AWARD_JEWELRY;
			emailRoot["subType"] =  jewelryIter->mValue->m_nJewelryId;
			emailRoot["count"] = 1;
				

				////////////////////////////////////////////////////////////////////////
				
			emailRoot["attr"]["uuid"] = jewelryIter->mValue->m_nJewelryUid;
			emailRoot["attr"]["typeId"] = jewelryIter->mValue->m_nJewelryId;
		    



		
		for(List<JewelryAttr>::Iter * jewelryAttrIter = jewelryIter->mValue->m_randomAttrList.begin(); jewelryAttrIter != NULL; 
			jewelryAttrIter = jewelryIter->mValue->m_randomAttrList.next(jewelryAttrIter))
		{
			Json::Value attrRoot;
			attrRoot["randomID"] = jewelryAttrIter->mValue.mRandomID;
			attrRoot["key"] = jewelryAttrIter->mValue.mKey;
            attrRoot["value"] = jewelryAttrIter->mValue.mValue;

			emailRoot["attr"]["randomAttrList"].append(attrRoot);
			
		}
		
		 
		emailRoot["attr"]["staticAttrKey"] = jewelryIter->mValue->m_staticAttrKey;
		emailRoot["attr"]["staticAttrValue"] = jewelryIter->mValue->m_staticAttrValue;

		
		
		emailRoot["attr"]["purplePoints"] = jewelryIter->mValue->m_purplePoints;

		emailRoot["attr"]["purpleTimes"] = jewelryIter->mValue->m_purpleTimes;

		emailRoot["attr"]["orangePoints"] = jewelryIter->mValue->m_orangePoints;

		emailRoot["attr"]["orangeTimes"] = jewelryIter->mValue->m_orangeTimes;

		root["data"].append(emailRoot);

	}

	Json::StyledWriter  writer;
//	root["errorId"] = 0;

	return writer.write(root);
}