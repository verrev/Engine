#include "Converter.h"
void Converter::getMaterials(FbxNode *n, std::vector<std::string> &textures, std::vector<PhongMaterial> &phongMats, std::vector<LambertMaterial> &lambertMats)
{
	for (int i = 0; i < n->GetMaterialCount(); ++i) {
		FbxSurfaceMaterial *material = n->GetMaterial(i);
		getDiffuseTextures(material, textures);
		if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
			phongMats.push_back(getPhongMaterial(material));
		}
		else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
			lambertMats.push_back(getLambertMaterial(material));
		}
	}
}

void Converter::getDiffuseTextures(FbxSurfaceMaterial *material, std::vector<std::string> &textures)
{
	FbxProperty property = material->FindProperty(FbxLayerElement::sTextureChannelNames[0]);
	if (property.IsValid()) {
		for (int j = 0; j < property.GetSrcObjectCount<FbxTexture>(); ++j) {
			if (!property.GetSrcObject<FbxLayeredTexture>(j)) {
				FbxTexture* texture = property.GetSrcObject<FbxTexture>(j);
				if (texture) {
					std::string textureType = property.GetNameAsCStr();
					FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
					if (fileTexture) {
						if (textureType == "DiffuseColor") {
							//std::cout << fileTexture->GetRelativeFileName() << "\n";
							textures.push_back(fileTexture->GetRelativeFileName());
						}
					}
				}
			}
		}
	}
}

LambertMaterial Converter::getLambertMaterial(FbxSurfaceMaterial *material)
{
	auto ambient = ((FbxSurfacePhong *)material)->Ambient;
	XMFLOAT3 amb = XMFLOAT3(ambient.Get()[0], ambient.Get()[1], ambient.Get()[2]);
	auto diffuse = ((FbxSurfacePhong *)material)->Diffuse;
	XMFLOAT3 dif = XMFLOAT3(diffuse.Get()[0], diffuse.Get()[1], diffuse.Get()[2]);
	auto emissive = ((FbxSurfacePhong *)material)->Emissive;
	XMFLOAT3 emi = XMFLOAT3(emissive.Get()[0], emissive.Get()[1], emissive.Get()[2]);
	auto alpha = 1.0 - ((FbxSurfacePhong *)material)->TransparencyFactor;

	return LambertMaterial(amb, dif, emi, alpha);
}

PhongMaterial Converter::getPhongMaterial(FbxSurfaceMaterial *material)
{
	auto ambient = ((FbxSurfacePhong *)material)->Ambient;
	XMFLOAT3 amb = XMFLOAT3(ambient.Get()[0], ambient.Get()[1], ambient.Get()[2]);
	auto diffuse = ((FbxSurfacePhong *)material)->Diffuse;
	XMFLOAT3 dif = XMFLOAT3(diffuse.Get()[0], diffuse.Get()[1], diffuse.Get()[2]);
	auto specular = ((FbxSurfacePhong *)material)->Specular;
	XMFLOAT3 spec = XMFLOAT3(specular.Get()[0], specular.Get()[1], specular.Get()[2]);
	auto emissive = ((FbxSurfacePhong *)material)->Emissive;
	XMFLOAT3 emi = XMFLOAT3(emissive.Get()[0], emissive.Get()[1], emissive.Get()[2]);

	auto alpha = 1.0 - ((FbxSurfacePhong *)material)->TransparencyFactor;
	auto shininess = ((FbxSurfacePhong *)material)->Shininess;
	auto reflectivity = ((FbxSurfacePhong *)material)->ReflectionFactor;

	return PhongMaterial(amb, dif, spec, emi, alpha, shininess, reflectivity);
}


UINT Converter::getMaterialIndex(FbxMesh *m)
{
	int triangleCount = m->GetPolygonCount();
	FbxLayerElementArrayTemplate<int> *indices;
	if (m->GetElementMaterial()) {
		indices = &(m->GetElementMaterial()->GetIndexArray());
		if (indices) {
			switch (m->GetElementMaterial()->GetMappingMode()) {
			case FbxGeometryElement::eByPolygon:
				if (indices->GetCount() == triangleCount)
					for (int i = 0; i < triangleCount; ++i) {
						return indices->GetAt(i);
					}
				break;
			case FbxGeometryElement::eAllSame:
				for (int i = 0; i < triangleCount; ++i) {
					return indices->GetAt(0);
				}
				break;
			default:
				throw std::exception("Material index mapping mode error.");
			}
		}
	}
	return 0;
}