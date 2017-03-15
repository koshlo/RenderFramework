#include "ObjMaterialLib.h"
#include "Tokenizer.h"
#include "ParsingHelpers.h"

#include "../Shaders/Material.data.fx"

struct ObjMaterialLib::MaterialDataHolder
{
	typedef ArrayMap<std::string, MaterialShaderData> MaterialDataMap;

	MaterialDataMap materialDataMap;
};

bool SetMaterialProperty(const std::string& prop, Tokenizer& tok, Material* destMaterial);
bool SetMaterialMapProperty(const std::string& currentTok, Tokenizer& tok, Material* destMaterial);

template <>
void RenderResourceLoader::SyncLoad(const MaterialLibResId& matLibId, Optional<ObjMaterialLib>* resultLib) const
{
	ObjMaterialLib lib(*this);
	if (lib.load(matLibId))
	{
		*resultLib = std::move(lib);
	}
}

template <>
Material& ObjMaterialLib::addMaterial(const MaterialResId& defaultMatId)
{
	return _materialMap.add(defaultMatId, Material());
}

template<typename MatType>
bool ObjMaterialLib::loadMaterial(const MaterialLibResId& matLibId)
{
	String matFile; matFile.sprintf("%s%s.mtl", RenderResourceLoader::DataPath(), matLibId.getId().c_str());
	const char* fileName = matFile.dataPtr();
	Tokenizer tok;
	bool success = tok.setFile(fileName);

	if (!success)
	{
		char str[256];
		sprintf(str, "Couldn't open \"%s\"", fileName);
		ErrorMsg(str);
	}

	MatType* lastMaterial = nullptr;
	char* str;
	while (success && (str = tok.next()) != nullptr)
	{
		switch (str[0])
		{
		case 'n': // newmtl
		{
			MatType& newMat = addMaterial(ResourceId<MaterialTraits<MatType>>(tok.next()));
			lastMaterial = &newMat;
			break;
		}
		case 'K':
		case 'N':
		{
			success = SetMaterialProperty(str, tok, lastMaterial);
			break;
		}
		case 'm':
		{
			success = SetMaterialMapProperty(str, tok, lastMaterial);
			break;
		}
		default:
		{
			tok.goToNextLine();
			break;
		}
		}
	}

	if (success)
	{
		std::for_each(_materialMap.values().begin(), _materialMap.values().end(), [this](const Material& mat)
		{
			loadTexture(mat.albedoMap);
			loadTexture(mat.metallicMap);
			loadTexture(mat.normalMap);
			loadTexture(mat.roughnessMap);
		});

		for (uint32_t i = 0; i < _materialMap.keys().size(); ++i)
		{
			const MaterialResId& matId = _materialMap.keys()[i];
			const Material& material = _materialMap.values()[i];

			MaterialShaderData matShaderData;
			matShaderData.SetAlbedoMap(getTexture(material.albedoMap));
			matShaderData.SetNormalMap(getTexture(material.normalMap));
			matShaderData.SetMetallicMap(getTexture(material.metallicMap));
			matShaderData.SetRoughnessMap(getTexture(material.roughnessMap));

			_materialDataHolder->materialDataMap.add(matId.getId(), matShaderData);
		}
	}
	else
	{
		char str[256];
		sprintf(str, "Couldn't parse material \"%s\"", fileName);
		ErrorMsg(str);
	}

	

	return success;
}

ObjMaterialLib::ObjMaterialLib(const RenderResourceLoader& resourceLoader) 
	: _resourceLoader(resourceLoader)
	, _materialDataHolder(new MaterialDataHolder)
{
}

ObjMaterialLib::~ObjMaterialLib() = default;

bool ObjMaterialLib::load(const MaterialLibResId &matLibId)
{
	return loadMaterial<Material>(matLibId);
}

const MaterialShaderData& ObjMaterialLib::getMaterialShaderData(const std::string& matName) const
{
	return _materialDataHolder->materialDataMap[matName];
}

MaterialShaderData* ObjMaterialLib::getMaterialShaderData(const std::string& matName)
{
	return _materialDataHolder->materialDataMap.getPtrFor(matName);
}

const ObjMaterialLib::MaterialOptRef ObjMaterialLib::getMaterial(const MaterialResId& name) const
{
	MaterialMap::KeyIterator it = _materialMap.getIterator(name);
	if (_materialMap.isInRange(it))
	{
		return ObjMaterialLib::MaterialOptRef(_materialMap.getValue(it));
	}
	return ObjMaterialLib::MaterialOptRef();
}

TextureID ObjMaterialLib::getTexture(const TextureResId& name, TextureID defaultTexture) const
{
	TextureMap::KeyIterator it = _loadedTextures.getIterator(name);
	if (_loadedTextures.isInRange(it))
	{
		return _loadedTextures.getValue(it);
	}
	return defaultTexture;
}

void ObjMaterialLib::loadTexture(const TextureResId& name)
{
	if (name.isValid())
	{
		Optional<TextureID> texture = _resourceLoader.SyncLoad(name);
		if (texture)
		{
			_loadedTextures.add(name, texture.getVal());
		}
	}
}

bool isFilePath(const char ch)
{
	return isAlphabetical(ch) || ch == '\\' || ch == '.';
}

bool SetMaterialProperty(const std::string& prop, Tokenizer& tok, Material* destMaterial)
{
	static const uint32_t MaxTokLen = 2;
	if (prop == "Kd")
	{
		destMaterial->albedo = readVector3(tok);
	}
	else if (prop == "Ka")
	{
		destMaterial->metallic = readVector3(tok).x;
	}
	else if (prop == "Ns")
	{
		destMaterial->roughness = readFloat(tok);
	}
	return true;
}

bool SetMaterialMapProperty(const std::string& currentTok, Tokenizer& tok, Material* destMaterial)
{
	static const uint32_t MaxTokLen = 64;
	TextureResId textureRes(tok.next(isFilePath));
	bool validProperty = textureRes.isValid();
	if (validProperty)
	{
		if (currentTok == "map_Kd")
		{
			destMaterial->albedoMap = textureRes;
		}
		else if (currentTok == "map_bump")
		{
			destMaterial->normalMap = textureRes;
		}
		else if (currentTok == "map_Ka")
		{
			destMaterial->metallicMap = textureRes;
		}
		else if (currentTok == "map_Ns")
		{
			destMaterial->roughnessMap = textureRes;
		}
		else if (currentTok != "map_d")
		{
			validProperty = false;
		}
	}

	return validProperty;
}