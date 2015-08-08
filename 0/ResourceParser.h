#pragma once
#include "Fileutils.h"
#include "../MC/Filestructure.h"
#include "DirectX11Core.h"

class ResourceParser
{
public:
	static void getIEDfromHeader(const MeshHeader &h, D3D11_INPUT_ELEMENT_DESC *ied, UINT &inputLayoutSize, UINT &elementCount);
private:
	ResourceParser() {}
};

