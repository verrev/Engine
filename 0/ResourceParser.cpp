#include "ResourceParser.h"

void ResourceParser::getIEDfromHeader(const MeshHeader &h, D3D11_INPUT_ELEMENT_DESC *ied, UINT &inputLayoutSize, UINT &elementCount)
{
	UINT index = 0, size = 0;

	if (h.vsInputLayout & VERTEX_ELEMENT::POSITION) {
		ied[index].SemanticName = "POSITION";
		ied[index].SemanticIndex = 0;
		ied[index].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		ied[index].InputSlot = 0;
		ied[index].AlignedByteOffset = size;
		ied[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[index++].InstanceDataStepRate = 0;
		size += sizeof(XMFLOAT3);
	}
	if (h.vsInputLayout & VERTEX_ELEMENT::NORMAL) {
		ied[index].SemanticName = "NORMAL";
		ied[index].SemanticIndex = 0;
		ied[index].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		ied[index].InputSlot = 0;
		ied[index].AlignedByteOffset = size;
		ied[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[index++].InstanceDataStepRate = 0;
		size += sizeof(XMFLOAT3);
	}
	if (h.vsInputLayout & VERTEX_ELEMENT::TEXCOORD) {
		ied[index].SemanticName = "TEXCOORD";
		ied[index].SemanticIndex = 0;
		ied[index].Format = DXGI_FORMAT_R32G32_FLOAT;
		ied[index].InputSlot = 0;
		ied[index].AlignedByteOffset = size;
		ied[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[index++].InstanceDataStepRate = 0;
		size += sizeof(XMFLOAT2);
	}

	inputLayoutSize = size;
	elementCount = index;
}