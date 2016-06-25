#include "jsonhandler.h"
#include "asset.h"
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

using namespace std;

void JsonHandler::ReadBlocks(QString worldID, 
	list<Block> &blocks, 
	vector<QString> &shortcutMeshPaths, vector<QString> &shortcutTexPaths,
	DirectionalLight &dl, vector<PointLight> &pls, vector<SpotLight> &sls)
{
	Asset *asset = Asset::GetInstance();
	blocks.clear();
	shortcutMeshPaths.clear();
	shortcutTexPaths.clear();
	pls.clear();
	sls.clear();

	QFile file(WorldJsonFilePath(worldID));
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		printf("Read blocks json file failed!\n");
		return;
	}

	QByteArray byteArray = file.readAll();
	file.close();

	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);

	if (jsonError.error != QJsonParseError::NoError)
	{
		printf("Parse blocks json file failed!\n");
		return;
	}

	QJsonObject worldJson = document.object();

	QJsonObject shortcutTexJson = worldJson.value("shortcut textures").toObject();
	for (QJsonObject::const_iterator sti = shortcutTexJson.constBegin(); sti != shortcutTexJson.constEnd(); sti++)
	{
		shortcutTexPaths.push_back(sti->toString());
	}

	QJsonObject shortcutMeshJson = worldJson.value("shortcut meshes").toObject();
	for (QJsonObject::const_iterator smi = shortcutMeshJson.constBegin(); smi != shortcutMeshJson.constEnd(); smi++)
	{
		shortcutMeshPaths.push_back(smi->toString());
	}

	QJsonObject lightsJson = worldJson.value("lights").toObject();

	QJsonObject directionalLightJson = lightsJson.value("directional light").toObject();
	dl.Color = ParseVec3(directionalLightJson.value("color").toString());
	dl.AmbientIntensity = directionalLightJson.value("ambient intensity").toString().toFloat();
	dl.DiffuseIntensity = directionalLightJson.value("diffuse intensity").toString().toFloat();
	dl.Direction = ParseVec3(directionalLightJson.value("direction").toString());

	QJsonObject pointLightsJson = lightsJson.value("point lights").toObject();
	for (QJsonObject::const_iterator pli = pointLightsJson.constBegin(); pli != pointLightsJson.constEnd(); pli++)
	{
		QJsonObject pointLightJson = pli->toObject();
		PointLight pl;
		pl.Color = ParseVec3(pointLightJson.value("color").toString());
		pl.AmbientIntensity = pointLightJson.value("ambient intensity").toString().toFloat();
		pl.DiffuseIntensity = pointLightJson.value("diffuse intensity").toString().toFloat();
		pl.Position = ParseVec3(pointLightJson.value("position").toString());
		pl.Distance = pointLightJson.value("distance").toString().toFloat();
		pl.Attenuation.Constant = pointLightJson.value("attenuation.constant").toString().toFloat();
		pl.Attenuation.Linear = pointLightJson.value("attenuation.linear").toString().toFloat();
		pl.Attenuation.Exp = pointLightJson.value("attenuation.exponent").toString().toFloat();

		pls.push_back(pl);
	}

	QJsonObject spotLightsJson = lightsJson.value("spot lights").toObject();
	for (QJsonObject::const_iterator sli = spotLightsJson.constBegin(); sli != spotLightsJson.constEnd(); sli++)
	{
		QJsonObject spotLightJson = sli->toObject();
		SpotLight sl;
		sl.Color = ParseVec3(spotLightJson.value("color").toString());
		sl.AmbientIntensity = spotLightJson.value("ambient intensity").toString().toFloat();
		sl.DiffuseIntensity = spotLightJson.value("diffuse intensity").toString().toFloat();
		sl.Distance = spotLightJson.value("distance").toString().toFloat();
		sl.Position = ParseVec3(spotLightJson.value("position").toString());
		sl.Attenuation.Constant = spotLightJson.value("attenuation.constant").toString().toFloat();
		sl.Attenuation.Linear = spotLightJson.value("attenuation.linear").toString().toFloat();
		sl.Attenuation.Exp = spotLightJson.value("attenuation.exponent").toString().toFloat();
		sl.Direction = ParseVec3(spotLightJson.value("direction").toString());
		sl.Cutoff = spotLightJson.value("cut off").toString().toFloat();

		sls.push_back(sl);
	}

	QJsonObject blocksJson = worldJson.value("blocks").toObject();

	for (QJsonObject::const_iterator bi = blocksJson.constBegin(); bi != blocksJson.constEnd(); bi++)
	{
		QJsonObject blockJson = bi->toObject();
		QJsonValue meshPath = blockJson.value("mesh");
		QJsonValue texturePath = blockJson.value("texture");
		Block block(asset->GetMesh(meshPath.toString().toStdString()), asset->GetTexture(texturePath.toString().toStdString(), GL_NEAREST, GL_NEAREST));

		QJsonObject cellsJson = blockJson.value("cells").toObject();
		for (QJsonObject::const_iterator ci = cellsJson.constBegin(); ci != cellsJson.constEnd(); ci++)
		{
			block.AddCell(ParseIVec3(ci->toString()));
		}
		blocks.push_back(block);
	}
}

void JsonHandler::WriteBlocks(QString worldID, 
	const list<Block> &blocks, 
	const vector<QString> &shortcutMeshPaths, const vector<QString> &shortcutTexPaths,
	const DirectionalLight &dl, const vector<PointLight> &pls, const vector<SpotLight> &sls)
{
	QJsonObject worldJson;

	QJsonObject shortcutTexJson;
	for (int i=0; i<9; i++)
	{
		shortcutTexJson.insert(QString::number(i), shortcutTexPaths[i]);
	}
	worldJson.insert("shortcut textures", shortcutTexJson);

	QJsonObject shortcutMeshJson;
	for (int i=0; i<9; i++)
	{
		shortcutMeshJson.insert(QString::number(i), shortcutMeshPaths[i]);
	}
	worldJson.insert("shortcut meshes", shortcutMeshJson);

	QJsonObject lightsJson;
	QJsonObject directionalLightJson;
	directionalLightJson.insert("color", QString("(%1, %2, %3)").arg(dl.Color.x).arg(dl.Color.y).arg(dl.Color.z));
	directionalLightJson.insert("ambient intensity", QString::number(dl.AmbientIntensity));
	directionalLightJson.insert("diffuse intensity", QString::number(dl.DiffuseIntensity));
	directionalLightJson.insert("direction", QString("(%1, %2, %3)").arg(dl.Direction.x).arg(dl.Direction.y).arg(dl.Direction.z));

	lightsJson.insert("directional light", directionalLightJson);

	QJsonObject pointLightsJson;
	int pointLightIndex = 0;
	for (vector<PointLight>::const_iterator pli = pls.begin(); pli != pls.end(); pli++, pointLightIndex++)
	{
		QJsonObject pointLightJson;
		pointLightJson.insert("color", QString("(%1, %2, %3)").arg(pli->Color.x).arg(pli->Color.y).arg(pli->Color.z));
		pointLightJson.insert("ambient intensity", QString::number(pli->AmbientIntensity));
		pointLightJson.insert("diffuse intensity", QString::number(pli->DiffuseIntensity));
		pointLightJson.insert("position", QString("(%1, %2, %3)").arg(pli->Position.x).arg(pli->Position.y).arg(pli->Position.z));
		pointLightJson.insert("distance", QString::number(pli->Distance));
		pointLightJson.insert("attenuation.constant", QString::number(pli->Attenuation.Constant));
		pointLightJson.insert("attenuation.linear", QString::number(pli->Attenuation.Linear));
		pointLightJson.insert("attenuation.exponent", QString::number(pli->Attenuation.Exp));

		pointLightsJson.insert(QString::number(pointLightIndex), pointLightJson);
	}
	lightsJson.insert("point lights", pointLightsJson);

	QJsonObject spotLightsJson;
	int spotLightIndex = 0;
	for (vector<SpotLight>::const_iterator sli = sls.begin(); sli != sls.end(); sli++, spotLightIndex++)
	{
		QJsonObject spotLightJson;
		spotLightJson.insert("color", QString("(%1, %2, %3)").arg(sli->Color.x).arg(sli->Color.y).arg(sli->Color.z));
		spotLightJson.insert("ambient intensity", QString::number(sli->AmbientIntensity));
		spotLightJson.insert("diffuse intensity", QString::number(sli->DiffuseIntensity));
		spotLightJson.insert("position", QString("(%1, %2, %3)").arg(sli->Position.x).arg(sli->Position.y).arg(sli->Position.z));
		spotLightJson.insert("distance", QString::number(sli->Distance));
		spotLightJson.insert("attenuation.constant", QString::number(sli->Attenuation.Constant));
		spotLightJson.insert("attenuation.linear", QString::number(sli->Attenuation.Linear));
		spotLightJson.insert("attenuation.exponent", QString::number(sli->Attenuation.Exp));
		spotLightJson.insert("direction", QString("(%1, %2, %3)").arg(sli->Direction.x).arg(sli->Direction.y).arg(sli->Direction.z));
		spotLightJson.insert("cut off", QString::number(sli->Cutoff));

		spotLightsJson.insert(QString::number(spotLightIndex), spotLightJson);
	}
	lightsJson.insert("spot lights", spotLightsJson);

	worldJson.insert("lights", lightsJson);

	QJsonObject blocksJson;
	int blockIndex = 0;
	for (list<Block>::const_iterator bi = blocks.begin(); bi != blocks.end(); bi++, blockIndex++)
	{
		QJsonObject blockJson;
		blockJson.insert("mesh", QString::fromStdString(bi->meshPath));
		blockJson.insert("texture", QString::fromStdString(bi->texturePath));

		QJsonObject cellJson;
		int cellIndex = 0;
		for (list<Cell>::const_iterator ci = bi->cells.begin(); ci != bi->cells.end(); ci++, cellIndex++)
		{
			cellJson.insert(QString("pos %1").arg(cellIndex), QString("(%1, %2, %3)").arg(ci->position.x).arg(ci->position.y).arg(ci->position.z));
		}
		blockJson.insert("cells", cellJson);
		blocksJson.insert(QString::number(blockIndex), blockJson);
	}
	worldJson.insert("blocks", blocksJson);


	QJsonDocument document;
	document.setObject(worldJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Indented);

	QFile file(WorldJsonFilePath(worldID));
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		printf("Write blocks json file failed!\n");
		return;
	}

	file.write(byteArray);
	file.close();
}

void JsonHandler::WriteBlocksDefault(QString worldID)
{
	list<Block> blocks;
	Block block("Resources/model/cube.obj", 
		"Resources/texture/block/constructive/default_block_tex.png");
	block.AddCell(glm::ivec3(0, 0, 0));
	blocks.push_back(block);

	vector<QString> shortcutMeshPaths;
	vector<QString> shortcutTexPaths;
	for (int i=0; i<9; i++)
	{
		shortcutMeshPaths.push_back("Resources/model/cube.obj");
		shortcutTexPaths.push_back("Resources/texture/block/constructive/default_block_tex.png");
	}

	DirectionalLight dl;
	vector<PointLight> pls;
	vector<SpotLight> sls;

	WriteBlocks(worldID, 
		blocks, 
		shortcutMeshPaths, shortcutTexPaths,
		dl, pls, sls);
}

QString JsonHandler::WorldJsonFilePath(QString worldID)
{
	return QString("Resources/world/%1.json").arg(worldID);
}

void JsonHandler::ReadAppSet()
{
	QFile file("Resources/config/appset.json");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		printf("Read appset json file failed!\n");
		appSet.resolution = glm::ivec2(1000, 600);
		appSet.isFullScreen = false;
		appSet.isVSync = true;
		appSet.msaa = -1;
		appSet.musicVolume = 50;
		appSet.audioVolume = 50;
		return;
	}

	QByteArray byteArray = file.readAll();
	file.close();

	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);

	if (jsonError.error != QJsonParseError::NoError)
	{
		printf("Parse appset json file failed!\n");
		return;
	}

	QJsonObject appSetJson = document.object();

	QString resolution = appSetJson.value("Resolution").toString();
	resolution = resolution.mid(1, resolution.length() - 2);
	QStringList resolutionXY = resolution.split(", ");
	appSet.resolution = glm::ivec2(resolutionXY.at(0).trimmed().toInt(), resolutionXY.at(1).trimmed().toInt());

	QString displayMode = appSetJson.value("Display Mode").toString();
	appSet.isFullScreen = displayMode == "Fullscreen";

	QString vSync = appSetJson.value("V-Sync").toString();
	appSet.isVSync = vSync == "Yes";

	int msaa = appSetJson.value("Anti-Aliasing").toString().toInt();
	appSet.msaa = msaa;

	int musicVolume = appSetJson.value("Music Volume").toString().toInt();
	appSet.musicVolume = musicVolume;

	int audioVolume = appSetJson.value("Audio Volume").toString().toInt();
	appSet.audioVolume = audioVolume;
}

void JsonHandler::WriteAppSet()
{
	QJsonObject appSetJson;
	appSetJson.insert("Resolution", QString("(%1, %2)").arg(appSet.resolution.x).arg(appSet.resolution.y));
	appSetJson.insert("Display Mode", appSet.isFullScreen ? "Fullscreen" : "Window");
	appSetJson.insert("V-Sync", appSet.isVSync ? "Yes" : "No");
	appSetJson.insert("Anti-Aliasing", QString::number(appSet.msaa));
	appSetJson.insert("Music Volume", QString::number(appSet.musicVolume));
	appSetJson.insert("Audio Volume", QString::number(appSet.audioVolume));

	QJsonDocument document;
	document.setObject(appSetJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Indented);

	QFile file("Resources/config/appset.json");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		printf("Write appset json file failed!\n");
		return;
	}

	file.write(byteArray);
	file.close();
}

glm::ivec3 JsonHandler::ParseIVec3(QString str)
{
	str = str.mid(1, str.length()-2);
	QStringList strList = str.split(", ");

	return glm::ivec3(strList.at(0).trimmed().toInt(), 
		strList.at(1).trimmed().toInt(), 
		strList.at(2).trimmed().toInt());
}

glm::vec3 JsonHandler::ParseVec3(QString str)
{
	str = str.mid(1, str.length()-2);
	QStringList strList = str.split(", ");

	return glm::vec3(strList.at(0).trimmed().toFloat(), 
		strList.at(1).trimmed().toFloat(), 
		strList.at(2).trimmed().toFloat());
}