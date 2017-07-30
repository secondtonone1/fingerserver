#ifndef LOGSENDER_H_
#define LOGSENDER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <map>
#include <vector>
#include <typeinfo>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <uuid/uuid.h>    //yum install libuuid-devel.x86_64
#include <curl/curl.h>    //yum install libcurl-devel.x86_64
#include "json.hpp"
#include "ini.h"
#include "threadpool.h"

namespace OurpalmLog
{
	using std::string;

	/**
	* 工具类
	*/
	class Utils {
	public:
		static void uuid_create(string prefix, string &uuidStr);
		static string & trim(string &srcStr);
		static bool isNumeric(const std::string & input);
		static void logInfo(string info);
		static bool dir_exists(const char* pzPath);
		static bool mkdirs(const char *szDirectoryPath, int iDirPermission = 0777);
		static void formatDateTime(const char* pattern,string &ret);
		static string map2jstr(map<string, string> &m);
	};

	/**
	* 配置类
	*/
	class Configs {
	private:
		int logSize;
		string logSendurl;
		string errorLogPath;
		IniFile conf;
	public:
		Configs();
		string getString(string key,string d="");
		int getInteger(string key, int d = 0);
	};

	/**
	* 异常类
	*/
	class ValidationException :public exception {
	private:
		const char *msg;
	public:
		ValidationException(const char *msg);
		const char * what();
	};

	/**
	* 日志信息类
	*/
	class Log {
	public:
		string id;
		string customId;
		map<string, string> val;
		string serviceCode;
		time_t timeStamp;
		string uuid;
		time_t createLogTime;
		map<string,string> logInfo;
		map<string,string> roleSpecAttr;
		string clientIp;
	public:
		Log();
		void setId(string id);
		string getId(void) const;
		void setClientIp(string clientIp);
		string getClientIp(void) const;
		void setCustomId(string customId);
		string getCustomId(void) const;
		void setRoleSpecAttr(map<string,string> roleSpecAttr);
		map<string,string> getRoleSpecAttr(void) const;
		map<string, string> getLogInfo(void) const;
		void setLogInfo(map<string, string> &logInfo);
		string getUuid(void) const;
		void setUuid(string uuid);
		time_t getTimeStamp(void) const;
		void setTimeStamp(time_t timeStamp);
		time_t getCreateLogTime(void) const;
		void setCreateLogTime(time_t createLogTime);
		string getServiceCode(void) const;
		void setServiceCode(string serviceCode);
		map<string, string> getVal(void) const;
		void setVal(map<string, string> val);
		void putUserId(string userId);
		void putServer(string server);
		void putRoleId(string roleId);
		void putRoleName(string roleName);
		void putRoleLevel(string roleLevel);
		void putRoleVipLevel(string roleVipLevel);
		template<class T>
		void validate(T log);
		virtual map<string, string> getPropMapOld(void) const;
		virtual map<string, string> getPropMapNew(void) const;
	};

	/**
	* 属性变更日志信息类
	*/
	class LogRolePropUpdate : public Log {
	public:
		map<string, string> propMapOld;
		map<string, string> propMapNew;
	public:
		LogRolePropUpdate();
		map<string, string> getPropMapOld(void) const;
		void setPropMapOld(map<string, string> propMapOld);
		map<string, string> getPropMapNew(void) const;
		void setPropMapNew(map<string, string> propMapNew);
	};
	
	/**
	* 日志发送服务类
	*/
	class PalmLogSender {
	private:
		static string failName_pre;
		static string log_dir;
		static string fileName;
		static Configs conf;
		static ThreadPool pool;
	public:
		static bool debug;
		static void setDebugMode(bool mode);
		static bool getDebugMode();
		static void postLog(string logStr);
		template<class T>
		static void sendLogList(vector<T> &logArrayFinal);
		static void writeSendFailLog(string logStr);
	};
	
	size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

	template<class T>
	void Log::validate(T log) {
		if (log.getServiceCode() == "") {
			throw ValidationException("异常：serviceCode is null!");
		}

		string logId = log.getId();
		if (logId == "") {
			throw ValidationException("异常：Logid is null！");
		}

		time_t logCreateTime = log.getCreateLogTime();
		if (logCreateTime <= 0) {
			throw ValidationException("异常：log create time is null！");
		}

		if (logId == "11000") {
			if (log.getCustomId() == "") {
				throw ValidationException("异常：customId is null！");
			}
		}
		else {
			if (log.getVal().at("roleId") == "") {
				throw ValidationException("异常：roleId is null！");
			}

			if (log.getVal().at("server") == "") {
				throw ValidationException("异常：server is null！");
			}

			if (log.getVal().at("roleLevel") == "") {
				throw ValidationException("异常：roleLevel is null！");
			}

			if (log.getVal().at("roleVipLevel") == "") {
				throw ValidationException("异常：roleVipLevel is null！");
			}

			if (log.getVal().at("roleName") == "") {
				throw ValidationException("roleName is null！");
			}

			string uid = Utils::trim(log.getVal().at("uid"));
			if (uid == "") {
				throw ValidationException("异常：uid is null!");
			}
			bool isNum = true;
			string sixNum = uid.substr(0, 6);
			if (!Utils::isNumeric(sixNum)) {
				isNum = false;
			}
			if (!isNum) {
				throw ValidationException("异常：uid first 6 chars contains none numeric!");
			}
			if (uid.length() < 40) {
				throw ValidationException("异常：uid length lt 40!");
			}
		}

		if (logId != "10006" && logId != "10007" && !(typeid(log).name() == typeid(LogRolePropUpdate).name())) {
			if (this->logInfo.size() == 0) {
				throw ValidationException("异常：LogInfoList is null or its size is 0！");
			}
		}
	}

	template<class T>
	void PalmLogSender::sendLogList(vector<T> &logArrayFinal) {
		string jstr = "[";
		try {
			vector<T> realLogList;
			if (logArrayFinal.size() <= 0) {
				throw ValidationException("异常：logArray is null or size is 0!");
			}
			typename vector<T>::iterator it;
			typename vector<T>::iterator it2;
			map<string, string>::iterator mit;
			for (it = logArrayFinal.begin(); it != logArrayFinal.end(); it++) {
				it->validate(*it);
				realLogList.push_back(*it);
			}

			if (realLogList.size() <= 0) {
				throw ValidationException("异常：realLogList is null or size is 0!");
			}

			if (realLogList.size() > PalmLogSender::conf.getInteger("log.size")) {
				throw ValidationException("异常：Butch log count more than log.size in config.ini");
			}

			for (it2 = realLogList.begin(); it2 != realLogList.end(); it2++) {
				nlohmann::json jsstr;
				jsstr["id"] = (*it2).getId();
				jsstr["customId"] = (*it2).getCustomId();
				if ((*it2).getVal().size() > 0) {
					map<string, string> tmap((*it2).getVal());
					jsstr["val"] = Utils::map2jstr(tmap);
					map<string, string>().swap(tmap);
				}
				else {
					jsstr["val"] = nlohmann::json::value_t();
				}
				jsstr["serviceCode"] = (*it2).getServiceCode();
				jsstr["timeStamp"] = (*it2).getTimeStamp();
				jsstr["uuid"] = (*it2).getUuid();
				jsstr["createLogTime"] = (*it2).getCreateLogTime();
				if ((*it2).getLogInfo().size() > 0) {
					map<string, string> tmap((*it2).getLogInfo());
					jsstr["logInfo"] = Utils::map2jstr(tmap);
					map<string, string>().swap(tmap);
				}
				else {
					jsstr["logInfo"] = nlohmann::json::object();
				}
				if (typeid((*it2)).name() == typeid(LogRolePropUpdate).name()){
					if ((*it2).getPropMapOld().size() > 0) {
						map<string, string> tmap((*it2).getPropMapOld());
						jsstr["propMapOld"] = Utils::map2jstr(tmap);
						map<string, string>().swap(tmap);
					} else {
						jsstr["propMapOld"] = nlohmann::json::object();
					}
					if ((*it2).getPropMapNew().size() > 0) {
						map<string, string> tmap((*it2).getPropMapNew());
						jsstr["propMapNew"] = Utils::map2jstr(tmap);
						map<string, string>().swap(tmap);
					}
					else {
						jsstr["propMapNew"] = nlohmann::json::object();
					}
				}
				jsstr["roleSpecAttr"] = (*it2).getRoleSpecAttr();
				jsstr["clientIp"] = (*it2).getClientIp();
				jstr += jsstr.dump();
				if (it2 + 1 != realLogList.end()) {
					jstr += ",";
				}
				jsstr = NULL;
			}
			jstr += "]";

			Utils::logInfo("sdkVer=" + PalmLogSender::conf.getString("api.version") + ",start send log:" + jstr);
			//PalmLogSender::postLog(jstr);
			pool.enqueue(PalmLogSender::postLog, jstr);
		}
		catch (ValidationException &e) {
			cout << e.what() << endl;
		}
		catch (exception &e) {
			cout << e.what() << endl;
		}
	}
}

#endif // !LOGSENDER_H_
