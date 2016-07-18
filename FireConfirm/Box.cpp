
#include "../LogicSystem.h"
#include "Box.h"



using namespace Lynx;

// using namespace rapidjson;


	void test()
	{
		//read json
		string updateInfo = "{\"UpdateInfo\":[{\"url\":\"aaaa.ipa\",\"platform\":\"ios\"}]}";

		rapidjson::Document doc;
		doc.Parse<0>(updateInfo.c_str());

		rapidjson::Value &dataArray = doc["UpdateInfo"];

		if (dataArray.IsArray())
		{
			for (int i = 0; i < dataArray.Size(); i++)
			{
				const rapidjson::Value& object = dataArray[i];

				string url = object["url"].GetString();
				string platform = object["platform"].GetString();
			}
		}



		//write json
		rapidjson::Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

		rapidjson::Value array(rapidjson::kArrayType);

		for (int i = 0; i < 10; i++)
		{
			rapidjson::Value object(rapidjson::kObjectType);
			object.AddMember("id", i, allocator);
			object.AddMember("name", "test", allocator);
			object.AddMember("version", 1.01, allocator);
			object.AddMember("vip", true, allocator);

// 			object.SetInt(i);
			array.PushBack(object, allocator);
		}

		document.AddMember("title", "PLAYER INFO", allocator);
 		document.AddMember("players", array, allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);
		std::string out = buffer.GetString();
// 		log("out: %s", out);
	}


void BoxManager::testJson()
{
	test();
	printf("Lu//a\"\n");
	rapidjson::Document document;

	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value contact(kArrayType);
	rapidjson::Value contact2(kArrayType);
	rapidjson::Value root(kArrayType);
	contact.PushBack("Lu//a\"", allocator).PushBack("Mio", allocator).PushBack("", allocator);
	contact2.PushBack("Lu// a", allocator).PushBack("Mio", allocator).PushBack("", allocator);
	root.PushBack(contact, allocator);
	root.PushBack(contact2, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	root.Accept(writer);
	std::string reststring = buffer.GetString();
//	cout << reststring << endl;
// 	return 0;
}




