#ifndef __LYNX_GAME_SERVER_DB_INTERFACE_H__
#define __LYNX_GAME_SERVER_DB_INTERFACE_H__

#include "PlatformLib.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "mysql.h"

namespace Lynx
{
    class DBInterface
    {
    public:
        DBInterface() : mConnected(false) {}
        virtual ~DBInterface() {}

        bool initial(const char* ip, UInt16 port, const char* username,
            const char* password, const char* dbname)
        {
            mIp = ip;
            mPort = port;
            mUsername = username;
            mPassword = password;
            mDBName = dbname;

            if (!mysql_init(&mHandler))
            {
                LOG_WARN("Failed to call mysql_init function");
                return false;
            }

            char value = 1;
            mysql_options(&mHandler, MYSQL_OPT_RECONNECT, (char*)&value);

            mConnection = mysql_real_connect(&mHandler, mIp.c_str(),
                mUsername.c_str(), mPassword.c_str(), mDBName.c_str(),
                mPort, NULL, CLIENT_MULTI_STATEMENTS);
            if (mConnection == NULL)
            {
                LOG_WARN("Failed to connection mysql server.");
                return false;
            }

            LOG_INFO("Connection mysql server success. [%s %s %s %s]",
                mIp.c_str(), mUsername.c_str(), mPassword.c_str(), mDBName.c_str());

            mConnected = true;

            // 必须确保字符集设置正确，否则不同版本的字符互相存取必出现乱码
            if (mysql_set_character_set(&mHandler, "utf8"))
            {
                LOG_WARN("Failed to mysql_set_character_set utf8");
                mysql_close(mConnection);
                mConnected = false;
                return false;
            }
            return true;
        }

        void release()
        {
            mysql_close(mConnection);
            mConnected = false;
        }

        bool isConnected() { return mConnected; }

        bool execSql(const char* sql)
        {
            if (sql == NULL)
            {
                LOG_WARN("Failed to exec sql for mysql is NULL.");
                return false;
            }

            if (mysql_real_query(&mHandler, sql, (unsigned long)strlen(sql)) != 0)
            {
                LOG_WARN("Failed to exec sql: %s, %s", sql, mysql_error(&mHandler));
                return false;
            }
            return true;
        }

        inline MYSQL_RES* storeResult()
        {
            return mysql_store_result(mConnection);
        }

        inline Int32 getAffectedRows()
        {
            return (Int32)mysql_affected_rows(&mHandler);
        }

        inline Int32 getNumRows(MYSQL_RES* res)
        {
            return (Int32)mysql_num_rows(res);
        }

        inline UInt32 getNumFields(MYSQL_RES* res)
        {
            return mysql_num_fields(res);
        }

        inline MYSQL_ROW fetchRow(MYSQL_RES* res)
        {
            return mysql_fetch_row(res);
        }

        inline Int32 fieldCount()
        {
            return mysql_field_count(&mHandler);
        }

        inline void freeResult(MYSQL_RES** res)
        {
            if(*res == NULL) return;
            mysql_free_result(*res);
            while(mysql_next_result(&mHandler) == 0)
            {
                mysql_free_result(mysql_store_result(mConnection));
            }
            *res = NULL;
        }

        String escapeString(const String& fromStr)
        {
            String toStr;
            char stackBuf[16384*4] = { 0 };
            char* buf = NULL;
            bool isRelease = false;

            if (fromStr.size() >= 16384*4*4)
            {
                return "";
            }

            if (fromStr.size() >= 16384*2)
            {
                buf = (char*)malloc(fromStr.size() * 2 + 1);
                if (!buf)
                {
                    return "";
                }
                isRelease = true;
            }
            else
            {
                buf = stackBuf;
            }

            if (mysql_real_escape_string(&mHandler, buf, fromStr.c_str(), fromStr.size()) == 0)
            {
                toStr.assign("");
            }
            else
            {
                toStr.assign(buf);
            }

            if (isRelease)
            {
                free(buf);
            }
            return toStr;
        }

        MYSQL* getHandler() { return &mHandler; }

    private:
        String mIp;
        UInt16 mPort;
        String mUsername;
        String mPassword;
        String mDBName;
        MYSQL mHandler;
        MYSQL* mConnection;
        bool mConnected;
    };
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_DB_INTERFACE_H__


