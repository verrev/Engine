#include "Converter.h"

void Converter::getUV(FbxMesh* m, int i, int j, XMFLOAT2& outUV)
{
	FbxStringList lUVSetNameList;
	m->GetUVSetNames(lUVSetNameList);
	FbxVector2 fbxUV;
	bool unmapped = 0;
	m->GetPolygonVertexUV(i, j, lUVSetNameList.GetStringAt(0), fbxUV, unmapped);
	outUV = XMFLOAT2(fbxUV.mData[0], 1 - fbxUV.mData[1]);
}
void Converter::getNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal)
{
	if (inMesh->GetElementNormalCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}
	FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

void Converter::getVertices(FbxMesh* m, std::vector<Vertex1P1N1UV> &vertices)
{
	FbxVector4 *fbxVertices = m->GetControlPoints();
	UINT vertexCounter = 0;
	for (int i = 0; i < m->GetPolygonCount(); i++) {
		for (int j = 0; j < 3; ++j) {
			int index = m->GetPolygonVertex(i, j);
			XMFLOAT3 nor;
			getNormal(m, index, vertexCounter, nor);
			XMFLOAT2 tex;
			getUV(m, i, j, tex);
			vertices.push_back(Vertex1P1N1UV(XMFLOAT3(fbxVertices[index].mData[0], fbxVertices[index].mData[1], fbxVertices[index].mData[2]), nor, tex));
			vertexCounter++;
		}
	}
}
void Converter::getIndices(std::vector<UINT> &indices, UINT vcount)
{
	for (int i = 0; i < vcount; ++i) {
		indices.push_back(i);
	}
}
