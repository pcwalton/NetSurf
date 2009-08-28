#ifndef heapview_heap_h_
#define heapview_heap_h_

#include <set>
#include <vector>

class Heap
{
public:
	Heap() : parser(0) {}
	~Heap();

	void simulate(const char *filename);

	struct Chunk
	{
		Chunk(uint64_t address, size_t length)
		{
			this->address = address;
			this->length = length;
		}

		uint64_t address;
		size_t length;
	};

	class Client
	{
	public:
		Client() {}
		virtual ~Client() = 0;

		enum Type { CREATED, DESTROYED };

		virtual void chunkModified(Type type, const Chunk &chunk) = 0;
	};

	void registerClient(Client &client);
	void deregisterClient(Client &client);

private:
	class FileParser;

	FileParser *parser;

	struct cmp {
		bool operator()(const Chunk *a, const Chunk *b)
		{
			return a->address < b->address;
		}
	};

	typedef std::set<Chunk *, cmp> ChunkSet;

	ChunkSet chunks;

	typedef std::vector<Client *> ClientVector;

	ClientVector clients;

	Heap(const Heap &rhs);
	Heap &operator=(const Heap &rhs);

	struct Op
	{
		enum {
			OP_ALLOC,
			OP_FREE
		} type;

		uint64_t address;
		size_t length;
	};

	void processEvent(const Op &op);
	void dispatchModified(Client::Type type, const Chunk &chunk);
};

#endif
