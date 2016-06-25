#ifndef _JSONHANDLER_H
#define _JSONHANDLER_H

#include "block.h"
#include "light.h"

#include <QString>
#include <list>
#include <vector>

class JsonHandler
{
public:
	static void ReadBlocks(QString worldID, 
		std::list<Block> &blocks, 
		std::vector<QString> &shortcutMeshPaths, std::vector<QString> &shortcutTexPaths,
		DirectionalLight &dl, std::vector<PointLight> &pls, std::vector<SpotLight> &sls);

	static void WriteBlocks(QString worldID, 
		const std::list<Block> &blocks, 
		const std::vector<QString> &shortcutMeshPaths, const std::vector<QString> &shortcutTexPaths,
		const DirectionalLight &dl, const std::vector<PointLight> &pls, const std::vector<SpotLight> &sls);

	static void WriteBlocksDefault(QString worldID);

	static QString WorldJsonFilePath(QString worldID);

	static void ReadAppSet();
	static void WriteAppSet();

private:
	static glm::ivec3 ParseIVec3(QString str);
	static glm::vec3 ParseVec3(QString str);
};

#endif //_JSONHANDLER_H
