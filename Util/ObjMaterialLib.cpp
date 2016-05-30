#include "ObjMaterialLib.h"
#include "Tokenizer.h"
#include "ParsingHelpers.h"

bool SetMaterialProperty(char propChar, Tokenizer& tok, Material* destMaterial);
bool SetMaterialMapProperty(const char* currentTok, Tokenizer& tok, Material* destMaterial);
bool SetMaterialProperty(char propChar, Tokenizer& tok, PBRMaterial* destMaterial);
bool SetMaterialMapProperty(const char* currentTok, Tokenizer& tok, PBRMaterial* destMaterial);

template <>
void RenderResourceLoader::SyncLoad(const MaterialLibResId& matLibId, bool isPbr, Optional<ObjMaterialLib>* resultLib)
{
	ObjMaterialLib lib(*this);
	if (lib.load(matLibId, isPbr))
	{
		*resultLib = std::move(lib);
	}
}

template <>
Material& ObjMaterialLib::addMaterial(const MaterialResId& defaultMatId)
{
	return _materialMap.add(defaultMatId, Material());
}

template <>
PBRMaterial& ObjMaterialLib::addMaterial(const PBRMaterialResId& pbrMatId)
{
	return _pbrMaterialMap.add(pbrMatId, PBRMaterial());
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
		{
			success = SetMaterialProperty(str[1], tok, lastMaterial);
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
		std::for_each(_materialMap.values().cbegin(), _materialMap.values().cend(), [this](const Material& mat)
		{
			loadTexture(mat.diffuseMap);
			loadTexture(mat.bumpMap);
		});
	}
	else
	{
		char str[256];
		sprintf(str, "Couldn't parse material \"%s\"", fileName);
		ErrorMsg(str);
	}
	return success;
}

bool ObjMaterialLib::load(const MaterialLibResId &matLibId, bool isPbr)
{
	bool result;
	if (isPbr)
	{
		result = loadMaterial<PBRMaterial>(matLibId);
	}
	else
	{
		result = loadMaterial<Material>(matLibId);
	}
	return result;
}

void ObjMaterialLib::applyMaterial(const std::string& name, Renderer* renderer)
{
	if (_isPBR)
	{
		applyMaterial(MaterialResId(name), renderer);
	}
	else
	{
		applyMaterial(PBRMaterialResId(name), renderer);
	}
}


void ObjMaterialLib::applyMaterial(const MaterialResId& matId, Renderer* renderer)
{
	MaterialOptRef material = getMaterial(matId);
	ASSERT(material);
	renderer->setShaderConstant3f("MaterialDiffuse", material.getVal().diffuse);
	renderer->applyConstants();
}

void ObjMaterialLib::applyMaterial(const PBRMaterialResId& matId, Renderer* renderer)
{
	PBRMaterialOptRef material = getMaterial(matId);
	ASSERT(material);

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

const ObjMaterialLib::PBRMaterialOptRef ObjMaterialLib::getMaterial(const PBRMaterialResId& name) const
{
	PBRMaterialMap::KeyIterator it = _pbrMaterialMap.getIterator(name);
	if (_pbrMaterialMap.isInRange(it))
	{
		return ObjMaterialLib::PBRMaterialOptRef(_pbrMaterialMap.getValue(it));
	}
	return PBRMaterialOptRef();
}

Optional<TextureID> ObjMaterialLib::getTexture(const TextureResId & name) const
{
	return Optional<TextureID>();
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

bool SetMaterialProperty(char propChar, Tokenizer& tok, Material* destMaterial)
{
	switch (propChar)
	{
	case 'd':
		destMaterial->diffuse = readVector3(tok);
		break;
	case 'a':
		destMaterial->ambient = readVector3(tok);
		break;
	default:
		break;
	}
	return true;
}

bool isFilePath(const char ch)
{
	return isAlphabetical(ch) || ch == '\\' || ch == '.';
}

bool SetMaterialMapProperty(const char* currentTok, Tokenizer& tok, Material* destMaterial)
{
	static const uint32_t MaxTokLen = 64;
	TextureResId textureRes(tok.next(isFilePath));
	bool validProperty = textureRes.isValid();
	if (validProperty)
	{
		if (::strncmp(currentTok, "map_Kd", MaxTokLen) == 0)
		{
			destMaterial->diffuseMap = TextureResId(tok.next(isFilePath));
		}
		else if (::strncmp(currentTok, "map_bump", MaxTokLen) == 0)
		{
			destMaterial->bumpMap = TextureResId(tok.next(isFilePath));
		}
		/*else
		{
			validProperty = false;
		}*/
	}
	return validProperty;
}

bool SetMaterialProperty(char propChar, Tokenizer& tok, PBRMaterial* destMaterial)
{
	return true;
}

bool SetMaterialMapProperty(const char* currentTok, Tokenizer& tok, PBRMaterial* destMaterial)
{
	static const uint32_t MaxTokLen = 64;
	TextureResId textureRes(tok.next(isFilePath));
	bool validProperty = textureRes.isValid();
	if (validProperty)
	{
		if (::strncmp(currentTok, "map_Kd", MaxTokLen) == 0)
		{
			destMaterial->albedoMap = textureRes;
		}
		else if (::strncmp(currentTok, "map_bump", MaxTokLen) == 0)
		{
			destMaterial->normalMap = textureRes;
		}
		else if (::strncmp(currentTok, "map_Ka", MaxTokLen) == 0)
		{
			destMaterial->metallicMap = textureRes;
		}
		else if (::strncmp(currentTok, "map_Ns", MaxTokLen) == 0)
		{
			destMaterial->roughnessMap = textureRes;
		}
		else
		{
			validProperty = false;
		}
	}

	return validProperty;
}