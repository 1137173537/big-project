#include "sJsonParser.h"
#include "platform/CCFileUtils.h"
#include "GameData.h"
USING_NS_CC;

sJsonParser * sJsonParser::createWithC_str(const char * data)
{
	sJsonParser * pRef = new sJsonParser();

	if (pRef->initWithC_str(data))
	{
		pRef->autorelease();
		return pRef;
	}
	CC_SAFE_DELETE(pRef);
	return nullptr;
}

bool sJsonParser::initWithC_str(const char * data)
{
	content = data;
	return true;
}

void sJsonParser::decode()
{
	rapidjson::Document document;
	document.Parse<0>(content.c_str());		//���룬0Ϊ������ʶ��Ĭ��ֵ��

	CCASSERT(!document.HasParseError(), "Parsing to document failure.");
	log("Parsing to document succeeded");
	CC_ASSERT(document.IsObject() && document.HasMember("Record"));		//�ж��Ƿ�����Ч�����Ƿ���record������

	const rapidjson::Value& records = document["Record"];

	CC_ASSERT(records.IsArray());

	for (unsigned int i = 0; i < records.Size(); ++i)
	{
		row = ValueMap();
		ValueMap temp_map = ValueMap();

		//ȡһ����¼����
		const rapidjson::Value &record = records[i];

		if (record.HasMember(SWAITINGSCENEDATA))		//for server
		{
			const rapidjson::Value &temp = record[SWAITINGSCENEDATA];

			const rapidjson::Value &val_add = temp[ADDROOM];
			row[ADDROOM] = Value(val_add.GetBool());

			const rapidjson::Value &val_rLabel = temp[ROOMLABEL];
			row[ROOMLABEL] = Value(val_rLabel.GetInt());

			const rapidjson::Value &val_name = temp[PLAYERNAME];
			row[PLAYERNAME] = Value(val_name.GetString());
		}

		list.push_back(Value(row));
	}

	list.push_back(Value(row));
}