#include "logsender.h"

using namespace OurpalmLog;
using std::cout;
using std::endl;
using std::type_info;

/* init static */
bool PalmLogSender::debug = false;
string PalmLogSender::failName_pre = "prismclient.error";
string PalmLogSender::log_dir = "";
string PalmLogSender::fileName;
Configs PalmLogSender::conf = Configs();
ThreadPool PalmLogSender::pool(30);

void Utils::uuid_create(string prefix, string &uuidStr) {
	uuid_t xx;
	uuid_generate(xx);
	char buf[40];
	uuid_unparse(xx, buf);
	uuidStr = buf;
}

string & Utils::trim(string & srcStr) {
	if (!srcStr.empty()) {
		srcStr.erase(0, srcStr.find_first_not_of(" "));
		srcStr.erase(srcStr.find_last_not_of(" ") + 1);
	}
	return srcStr;
}

bool OurpalmLog::Utils::isNumeric(const std::string & input) {
	return std::all_of(input.begin(), input.end(), ::isdigit);
}

void Utils::logInfo(string info) {
	if (PalmLogSender::getDebugMode() == true) {
		string dt;
		Utils::formatDateTime("%Y-%m-%d %H:%M:%S", dt);
		cout << dt << " -- " << info << endl;
	}
}

bool Utils::dir_exists(const char * pzPath) {
	if (pzPath == NULL) return false;

	DIR *pDir;
	bool bExists = false;

	pDir = opendir(pzPath);

	if (pDir != NULL)
	{
		bExists = true;
		(void)closedir(pDir);
	}

	return bExists;
}

bool Utils::mkdirs(const char * szDirectoryPath, int iDirPermission) {
	if (NULL == szDirectoryPath) {
		return false;
	}

	const int iPathLength = static_cast< int >(strlen(szDirectoryPath));
	if (iPathLength > PATH_MAX) {
		return false;
	}

	char szPathBuffer[PATH_MAX] = { 0 };
	memcpy(szPathBuffer, szDirectoryPath, iPathLength);

	for (int i = 0; i < iPathLength; ++i) {
		char &refChar = szPathBuffer[i];
		if (('/' == refChar) && (0 != i)) {
			refChar = '\0';
			int iStatus = access(szPathBuffer, F_OK);
			if (0 != iStatus) {
				if ((ENOTDIR == errno) || (ENOENT == errno)) {
					iStatus = mkdir(szPathBuffer, iDirPermission);
					if (0 != iStatus) {
						return false;
					}
				} else {
					return false;
				}
			}
			refChar = '/';
		}
	}
	return true;
}

void Utils::formatDateTime(const char * pattern, string &ret) {
	time_t currTime;
	currTime = time(NULL);
	struct tm *tmLocal;
	tmLocal = localtime(&currTime);
	char tmpbuf[128];
	strftime(tmpbuf, 127, pattern, tmLocal);
	ret = tmpbuf;
}

void PalmLogSender::setDebugMode(bool mode) {
	PalmLogSender::debug = mode;
}

bool PalmLogSender::getDebugMode() {
	return PalmLogSender::debug;
}

void PalmLogSender::postLog(string logStr) {
	try {
		Configs conf;
		string url = conf.getString("log.sendurl", "http://bicenter.gamebean.net/bicenter/prismClient/sendLogV1.do");
		CURL *curl;
		CURLcode res;
		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, logStr.c_str());
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
		}
	} catch (ValidationException &e) {
		Utils::logInfo("日志发送失败，记录失败日志到文件!");
		PalmLogSender::writeSendFailLog(logStr);
	} catch (exception &e) {
		Utils::logInfo("日志发送失败，记录失败日志到文件!");
		PalmLogSender::writeSendFailLog(logStr);
	}
}

void PalmLogSender::writeSendFailLog(string logStr) {
	
	if (PalmLogSender::log_dir == "") {
		PalmLogSender::log_dir = PalmLogSender::conf.getString("log.errorlogpath", "/opt/ourpalm/logs/prismClient/fail");
	}
	
	if (!Utils::dir_exists(PalmLogSender::log_dir.c_str())) {
		Utils::mkdirs(PalmLogSender::log_dir.c_str());
	}
	if (PalmLogSender::fileName == "") {
		string dt;
		Utils::formatDateTime("%Y-%m-%d", dt);
		PalmLogSender::fileName = PalmLogSender::log_dir + "/" + PalmLogSender::failName_pre + "." + dt + ".log";
	}

	ofstream fout;
	fout.open(PalmLogSender::fileName, ios_base::out | ios_base::app);
	fout << logStr << endl;
	fout.close();
	
}

string OurpalmLog::Utils::map2jstr(map<string, string>& m) {
	nlohmann::json jstr;
	map<string, string>::iterator it;
	for (it = m.begin(); it != m.end(); it++) {
		jstr[(*it).first] = (*it).second;
	}
	return jstr.dump();
}

Configs::Configs():conf("config.ini"){
	ifstream iniF;
	const char *iniFilePath = "./config.ini";
	iniF.open(iniFilePath);
	if (!iniF.is_open()) {
		cout << "config.ini not found ! exit..." << endl;
		exit(0);
	}
}

string Configs::getString(string key, string d) {
	this->conf.setSection("global");
	return this->conf.readStr(key, d);
}

int Configs::getInteger(string key, int d) {
	this->conf.setSection("global");
	return this->conf.readInt(key, d);
}

Log::Log() {
	string uuidStr;
	Utils::uuid_create("", uuidStr);
	this->uuid = uuidStr;
	this->timeStamp = time(NULL) * 1000;
}

void Log::setId(string id) {
	this->id = id;
}

string Log::getId(void) const {
	return id;
}

void Log::setClientIp(string clientIp) {
	this->clientIp = clientIp;
}

string Log::getClientIp(void) const {
	return clientIp;
}

void Log::setCustomId(string customId) {
	this->customId = customId;
}

string Log::getCustomId(void) const {
	return customId;
}

void Log::setRoleSpecAttr(map<string, string> roleSpecAttr) {
	this->roleSpecAttr = roleSpecAttr;
}

map<string, string> Log::getRoleSpecAttr(void) const {
	return roleSpecAttr;
}

map<string, string> Log::getLogInfo(void) const {
	return logInfo;
}

void Log::setLogInfo(map<string, string> &logInfo) {
	this->logInfo = logInfo;
}

string Log::getUuid(void) const {
	return this->uuid;
}

void Log::setUuid(string uuid) {
	this->uuid = uuid;
}

time_t Log::getTimeStamp(void) const {
	return this->timeStamp;
}

void Log::setTimeStamp(time_t timeStamp) {
	this->timeStamp = timeStamp;
}

time_t Log::getCreateLogTime(void) const {
	return this->createLogTime;
}

void Log::setCreateLogTime(time_t createLogTime) {
	this->createLogTime = createLogTime;
}

string Log::getServiceCode(void) const{
	return this->serviceCode;
}

void Log::setServiceCode(string serviceCode) {
	this->serviceCode = serviceCode;
}

map<string, string> Log::getVal(void) const {
	return this->val;
}

void Log::setVal(map<string, string> val) {
	this->val = val;
}

void Log::putUserId(string userId) {
	this->val.insert(pair<string, string>("uid", userId));
}

void Log::putServer(string server) {
	this->val.insert(pair<string, string>("server", server));
}

void Log::putRoleId(string roleId) {
	this->val.insert(pair<string, string>("roleId", roleId));
}

void Log::putRoleName(string roleName) {
	this->val.insert(pair<string, string>("roleName", roleName));
}

void Log::putRoleLevel(string roleLevel) {
	this->val.insert(pair<string, string>("roleLevel", roleLevel));
}

void Log::putRoleVipLevel(string roleVipLevel) {
	this->val.insert(pair<string, string>("roleVipLevel", roleVipLevel));
}

map<string, string> OurpalmLog::Log::getPropMapOld(void) const
{
	return map<string, string>();
}

map<string, string> OurpalmLog::Log::getPropMapNew(void) const
{
	return map<string, string>();
}



LogRolePropUpdate::LogRolePropUpdate() {
	this->setId("10010");
}

map<string, string> LogRolePropUpdate::getPropMapOld(void) const {
	return this->propMapOld;
}

void LogRolePropUpdate::setPropMapOld(map<string, string> propMapOld) {
	this->propMapOld = propMapOld;
}

map<string, string> LogRolePropUpdate::getPropMapNew(void) const {
	return this->propMapNew;
}

void LogRolePropUpdate::setPropMapNew(map<string, string> propMapNew) {
	this->propMapNew = propMapNew;
}

size_t OurpalmLog::write_data(void * buffer, size_t size, size_t nmemb, void * userp) {
	char *result = (char *)buffer;

	auto jsonstr = nlohmann::json::parse(result);
	if (jsonstr["reset"] != "1000") {
		throw ValidationException("异常：返回的reset不为1000，日志发送失败！");
	} else {
		Utils::logInfo("日志发送OK.");
	}
	return 0;
}

ValidationException::ValidationException(const char * msg):msg(msg){}

const char * ValidationException::what() {
	return this->msg;
}
