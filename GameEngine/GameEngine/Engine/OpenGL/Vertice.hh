#ifndef VERTICE_HH_
# define VERTICE_HH_

# include <array>
# include "Data.hh"

template <uint8_t NBR_ATTRIBUTE>
class VertexManager;

template <uint8_t NBR_ATTRIBUTE>
class Vertice
{
public:
	Vertice(uint32_t nbrVertex, std::array<Data, NBR_ATTRIBUTE> attribData, Data const * const indices = NULL);
	~Vertice();
	Vertice(Vertice const &copy);
	Vertice &operator=(Vertice const &vertex);
	bool operator==(Vertice<NBR_ATTRIBUTE> const &vertex) const;
	uint32_t getSizeBuffer() const;
	uint32_t getSizeIndices() const;
	uint32_t getNbrVertex() const;
	int32_t getIndexPool() const;
	bool isDrawable() const;
	bool hasIndices() const;
	void draw() const;
	friend void VertexManager<NBR_ATTRIBUTE>::addVertice(Vertice<NBR_ATTRIBUTE> * const vertice);

private:
	std::array<Data, NBR_ATTRIBUTE> _bufferData;
	uint32_t _sizeBuffer;
	Data *_indices;
	uint32_t _nbrVertex;
	VertexManager<NBR_ATTRIBUTE> *_vertexManager;
	int32_t _index;
};

# include "Vertice.hpp"
# include "VertexManager.hh"

#endif /* !VERTICE_HH_ */