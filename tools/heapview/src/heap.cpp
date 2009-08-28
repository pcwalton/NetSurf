#include <fstream>
#include <iostream>
#include <sstream>

#include "heap.h"

class Heap::FileParser
{
public:
	FileParser(Heap &heap, const char *filename)
		: input(filename), heap(heap)
	{
	}

	~FileParser()
	{
		if (input.is_open())
			input.close();
	}

	void parse_step();

private:
	std::ifstream input;
	Heap &heap;

	FileParser(const FileParser &rhs);
	FileParser &operator=(const FileParser &rhs);

	void parseMem(const std::string &line);
	void parseMalloc(const std::string &line, std::string::size_type delim);
	void parseCalloc(const std::string &line, std::string::size_type delim);
	void parseRealloc(const std::string &line, 
			std::string::size_type delim);
	void parseFree(const std::string &line, std::string::size_type delim);
	void parseStrdup(const std::string &line, std::string::size_type delim);
	void parseStrndup(const std::string &line, 
			std::string::size_type delim);
};

Heap::~Heap()
{
	ChunkSet::iterator first = chunks.begin();
	ChunkSet::iterator last = chunks.end();

	for (; first != last; first++) {
		delete (*first);
	}

	chunks.clear();

	clients.clear();

	if (parser != 0)
		delete parser;
}

Heap::Client::~Client()
{
}

void Heap::simulate(const char *filename)
{
	if (parser == 0)
		parser = new FileParser(*this, filename);

	try {
		parser->parse_step();
	} catch(...) {
		delete parser;
		parser = 0;
	}
}

void Heap::registerClient(Client &client)
{
	clients.push_back(&client);
}

void Heap::deregisterClient(Client &client)
{
	ClientVector::iterator it = clients.begin();
	ClientVector::iterator end = clients.end();

	for (; it != end; it++) {
		if ((*it) == &client) {
			clients.erase(it);
			break;
		}
	}
}

void Heap::processEvent(const Op &op)
{
	if (op.type == Op::OP_ALLOC) {
		Chunk *c = new Chunk(op.address, op.length);

		chunks.insert(c);

		dispatchModified(Client::CREATED, *c);
	} else {
		Chunk c(op.address, op.length);

		ChunkSet::iterator it = chunks.find(&c);

		if (it != chunks.end()) {
			dispatchModified(Client::DESTROYED, *(*it));

			delete (*it);
			chunks.erase(it);
		} else {
			std::cerr << "Free of unallocated block @ ";
			std::cerr << op.address << std::endl;
		}
	}
}

void Heap::dispatchModified(Client::Type type, const Chunk &chunk)
{
	/* Ignore large (mmapped) chunks */
	if (chunk.length >= 128 * 1024)
		return;

	ClientVector::iterator first = clients.begin();
	ClientVector::iterator last = clients.end();

	for (; first != last; first++) {
		(*first)->chunkModified(type, chunk);
	}
}

/******************************************************************************
 * File Parser                                                                *
 ******************************************************************************/

void Heap::FileParser::parse_step()
{
	if (input.is_open()) {
		std::string line;

		while (input.eof() == false) {
			std::getline(input, line);

			/* Ensure we have a MEM line */
			if (line.find("MEM", 0, 3) != line.npos) {
				parseMem(line);
				break;
			}
		}

		if (input.eof())
			throw 0;
	} else
		throw 0;
}

void Heap::FileParser::parseMem(const std::string &line)
{
	/* Skip over MEM */
	std::string::size_type delim = line.find_first_of(" ", 0);
	std::string::size_type start = line.find_first_not_of(" ", delim);

	/* Skip over <filename>:<line> */
	delim = line.find_first_of(" ", start);
	start = line.find_first_not_of(" ", delim);

	/* Now, work out the function we're dealing with */
	delim = line.find_first_of("(", start);
	if (line.compare(start, delim - start, "malloc") == 0) {
		parseMalloc(line, delim);
	} else if (line.compare(start, delim - start, "calloc") == 0) {
		parseCalloc(line, delim);
	} else if (line.compare(start, delim - start, "realloc") == 0) {
		parseRealloc(line, delim);
	} else if (line.compare(start, delim - start, "free") == 0) {
		parseFree(line, delim);
	} else if (line.compare(start, delim - start, "strdup") == 0) {
		parseStrdup(line, delim);
	} else if (line.compare(start, delim - start, "strndup") == 0) {
		parseStrndup(line, delim);
	} else {
		std::cerr << "Unexpected function ";
		std::cerr << line.substr(start, delim - start) << std::endl;
	}
}

void Heap::FileParser::parseMalloc(const std::string &line, 
		std::string::size_type delim)
{
	Op op;

	op.type = Op::OP_ALLOC;

	/* (<size>) = <addr> */
	std::string::size_type start = line.find_first_not_of("(", delim);
	delim = line.find_first_of(")", start);

	std::istringstream size(line.substr(start, delim - start));
	size >> std::dec >> op.length;

	/* Now, the address */
	start = line.find_first_not_of(" )=", delim);
	std::istringstream address(line.substr(start, line.npos - start));
	address >> std::hex >> op.address;

	/* Send event */
	heap.processEvent(op);
}

void Heap::FileParser::parseCalloc(const std::string &line, 
		std::string::size_type delim)
{
	Op op;

	op.type = Op::OP_ALLOC;

	/* (<nmemb>, <size>) = <addr> */
	std::string::size_type start = line.find_first_not_of("(", delim);
	delim = line.find_first_of(",", start);

	uint64_t nmemb, size;

	std::istringstream ns(line.substr(start, delim - start));
	ns >> std::dec >> nmemb;

	/* Size */
	start = line.find_first_not_of(", ", delim);
	delim = line.find_first_of(")", start);
	std::istringstream ss(line.substr(start, delim - start));
	ss >> std::dec >> size;

	op.length = nmemb * size;

	/* Address */
	start = line.find_first_not_of(" )=", delim);
	std::istringstream as(line.substr(start, line.npos - start));
	as >> std::hex >> op.address;

	/* Send event */
	heap.processEvent(op);
}

void Heap::FileParser::parseRealloc(const std::string &line, 
		std::string::size_type delim)
{
	Op op;

	op.type = Op::OP_FREE;

	/* (<oldaddr>, <size>) = <addr> */
	std::string::size_type start = line.find_first_not_of("(", delim);
	delim = line.find_first_of(",", start);

	std::istringstream ns(line.substr(start, delim - start));
	ns >> std::hex >> op.address;

	/* realloc(NULL, ...) == malloc(...) */
	if (op.address != 0) {
		op.length = 0;

		/* Release old chunk */
		heap.processEvent(op);
	}

	op.type = Op::OP_ALLOC;

	/* Size */
	start = line.find_first_not_of(", ", delim);
	delim = line.find_first_of(")", start);
	std::istringstream ss(line.substr(start, delim - start));
	ss >> std::dec >> op.length;

	/* Address */
	start = line.find_first_not_of(" )=", delim);
	std::istringstream as(line.substr(start, line.npos - start));
	as >> std::hex >> op.address;

	/* realloc(..., 0) == free(...) */
	if (op.length != 0) {
		/* Send event */
		heap.processEvent(op);
	}
}

void Heap::FileParser::parseFree(const std::string &line, 
		std::string::size_type delim)
{
	Op op;

	op.type = Op::OP_FREE;
	op.length = 0;

	/* (<addr>) */
	std::string::size_type start = line.find_first_not_of("(", delim);
	delim = line.find_first_of(")", start);

	std::istringstream ss(line.substr(start, delim - start));
	ss >> std::hex >> op.address;

	/* Send event */
	heap.processEvent(op);
}

void Heap::FileParser::parseStrdup(const std::string &line, 
		std::string::size_type delim)
{
	Op op;

	op.type = Op::OP_ALLOC;

	/* (<oldaddr>) (<size>) = <addr> */
	delim = line.find_first_of("(", delim + 1);

	std::string::size_type start = line.find_first_not_of("(", delim);
	delim = line.find_first_of(")", start);

	std::istringstream ss(line.substr(start, delim - start));
	ss >> std::dec >> op.length;

	start = line.find_first_not_of(" )=", delim);
	std::istringstream as(line.substr(start, line.npos - start));
	as >> std::hex >> op.address;

	/* Send event */
	heap.processEvent(op);
}

void Heap::FileParser::parseStrndup(const std::string &line, 
		std::string::size_type delim)
{
	Op op;

	op.type = Op::OP_ALLOC;

	/* (<oldaddr>, <n>) (<size>) = <addr> */
	delim = line.find_first_of("(", delim + 1);

	std::string::size_type start = line.find_first_not_of("(", delim);
	delim = line.find_first_of(")", start);

	std::istringstream ss(line.substr(start, delim - start));
	ss >> std::dec >> op.length;

	start = line.find_first_not_of(" )=", delim);
	std::istringstream as(line.substr(start, line.npos - start));
	as >> std::hex >> op.address;

	/* Send event */
	heap.processEvent(op);
}

