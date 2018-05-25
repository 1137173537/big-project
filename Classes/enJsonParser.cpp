#include "enJsonParser.h"
#include "GameData.h"

USING_NS_CC;
enJsonParser * enJsonParser::createWithArray(ValueVector pListData)
{
	enJsonParser * pRef = new enJsonParser();

	if (pRef->initWithArray(pListData))
	{
		pRef->autorelease();
		return pRef;
	}
	CC_SAFE_DELETE(pRef);
	return nullptr;
}

bool enJsonParser::initWithArray(ValueVector pListData)
{
	listData = pListData;
	return true;
}

void enJsonParser::encode(Information message,string order)		//���ﻻ��message��ֱֹ��ʹ��ȫ�ֱ���information���δ֪��bug
{
	rapidjson::Document document;
	document.SetObject();		//��ʼ��document
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();		//����ڴ�������������˷���������ڴ�ķ���

	rapidjson::Value array(rapidjson::kArrayType);

	for (auto& v : listData)
	{
		ValueMap temp = v.asValueMap();				//�����v����map��Value��һ�ְ�װ�࣬���԰Ѻܶ��������Ͱ�װ����

		rapidjson::Value object(rapidjson::kObjectType);
		if (order == ENTERROOMDATA)
		{
			ValueMap row = temp[ENTERROOMDATA].asValueMap();
			rapidjson::Value v_map(rapidjson::kObjectType);

			rapidjson::Value v_add;
			v_add.SetBool(row[ADDROOM].asBool());
			v_map.AddMember(ADDROOM, v_add, allocator);

			rapidjson::Value v_rTag;
			v_rTag.SetInt(row[ROOMLABEL].asInt());
			v_map.AddMember(ROOMLABEL, v_rTag, allocator);

			rapidjson::Value v_name;
			v_name.SetString(row[PLAYERNAME].asString().c_str(), allocator);
			v_map.AddMember(PLAYERNAME, v_name, allocator);

			object.AddMember(SWAITINGSCENEDATA, v_map, allocator);
		}

		array.PushBack(object, allocator);
	}

	//document.AddMember("change", true, allocator);
	document.AddMember("Record", array, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer < rapidjson:: StringBuffer > writer(buffer);	//����writer���󣬽����ݱ��浽buffer��

	document.Accept(writer);	//ͨ��write������д��buffer
	
	message.setSendBuf(buffer.GetString());
	//log("out: %s", out);
}