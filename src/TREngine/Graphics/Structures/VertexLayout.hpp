#pragma once
#include <Graphics/Graphics_Interfaces.hpp>
#include <vector>

TRV2_NAMESPACE_BEGIN
struct VertexElement
{
    int Offset;
    int NumberOfElements;
    EngineDataType DataType;

    VertexElement(int offset, int size, EngineDataType dataType) : Offset(offset), NumberOfElements(size),
        DataType(dataType)
    {

    }
};

class VertexLayout
{
public:
	VertexLayout();
	~VertexLayout();

	void Add(const VertexElement& element);
	size_t GetSize() const;
	const std::vector<VertexElement>& GetVertexElements() const { return _vertexElements; }

private:
    size_t _size;
	std::vector<VertexElement> _vertexElements;
};
TRV2_NAMESPACE_END