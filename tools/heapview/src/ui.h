#ifndef heapview_ui_h_
#define heapview_ui_h_

#include <bitset>
#include <set>

#include <gtkmm/main.h>
#include <gtkmm/window.h>

#include "heap.h"

class UI : public Heap::Client
{
public:
	UI(int argc, char **argv);
	~UI();

	bool poll();

	void redraw(Cairo::RefPtr<Cairo::Context> cr);

private:
	class Window;

	class Row
	{
	public:
		static const size_t NBYTES = 1024;
		static const size_t MASK = ~(NBYTES - 1);

		explicit Row(uint64_t address) : base(address) {}
		~Row() {}

		uint64_t getBase() { return base; }

		void setBits(size_t first, size_t last);
		void clearBits(size_t first, size_t last);

		void redraw(Cairo::RefPtr<Cairo::Context> cr);

		struct cmp
		{
			bool operator()(const Row *a, const Row *b)
			{
				return a->base < b->base;
			}
		};
	private:
		uint64_t base;
		std::bitset<NBYTES> bytes;

		Row(const Row &rhs);
		Row &operator=(const Row &rhs);
	};

	typedef std::set<Row *, Row::cmp> RowSet;

	RowSet rows;

	int pollCount;

	Gtk::Main gtkenv;
	Window *window;

	UI(const UI &rhs);
	UI &operator=(const UI &rhs);

	void chunkModified(Heap::Client::Type type, const Heap::Chunk &chunk);
	static void bytesForChunk(const Heap::Chunk &chunk, uint64_t rowbase,
			size_t &first, size_t &last);
};

#endif
