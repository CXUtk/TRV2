#include "VertexLayout.hpp"
TRV2_NAMESPACE_BEGIN
VertexLayout::VertexLayout() : _size(0)
{
}

VertexLayout::~VertexLayout()
{
}

void VertexLayout::Add(const VertexElement& element)
{
	int baseSize = 0;
	switch (element.DataType)
	{
	case BufferDataType::BYTE:
	{
		baseSize = 1;
		break;
	}
	case BufferDataType::SHORT:
	{
		baseSize = 2;
		break;
	}
	case BufferDataType::FLOAT:
	{
		baseSize = 4;
		break;
	}
	case BufferDataType::INT:
	{
		baseSize = 4;
		break;
	}
	default:
		break;
	}
	_size += baseSize * element.NumberOfElements;
	_vertexElements.push_back(element);
}

size_t VertexLayout::GetSize() const
{
	return _size;
}
TRV2_NAMESPACE_END