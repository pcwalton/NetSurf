#include <iostream>

#include <gtkmm/drawingarea.h>
#include <gtkmm/scrolledwindow.h>

#include "heap.h"

#include "ui.h"

class UI::Window : public Gtk::Window
{
public:
	Window(UI &ui)
		: area(ui)
	{
		/** \todo Less hacky window size */
		set_default_size(Row::NBYTES + 20, 600);

		scrollpane.set_policy(Gtk::POLICY_AUTOMATIC, 
				Gtk::POLICY_ALWAYS);
		scrollpane.add(area);

		add(scrollpane);

		show_all_children();
	}

	~Window() {}

	void set_dimensions(int width, int height)
	{
		int oldwidth, oldheight;

		area.get_size_request(oldwidth, oldheight);

		if (width != oldwidth || height != oldheight)
			area.set_size_request(width, height);
	}

	void redraw_row(int row, int left, int right)
	{
		area.queue_draw_area(left, row, right - left, 1);
	}
private:
	class DrawingArea : public Gtk::DrawingArea
	{
	public:
		DrawingArea(UI &ui)
		{
			this->ui = &ui;
		}

		virtual ~DrawingArea() {}

	protected:
		virtual bool on_expose_event(GdkEventExpose *event)
		{
			Glib::RefPtr<Gdk::Window> window = get_window();

			if (window) {
				Cairo::RefPtr<Cairo::Context> cr = 
						window->create_cairo_context();

				if (event) {
					/* Clip to redraw region */
					cr->rectangle(event->area.x, 
							event->area.y,
							event->area.width, 
							event->area.height);
					cr->clip();
				}

				/* Antialiasing is pointless */
				cr->set_antialias(Cairo::ANTIALIAS_NONE);

				cr->set_line_width(1.0);
				cr->set_source_rgb(0.0, 0.0, 0.8);

				/* Now, force the UI to redraw */
				ui->redraw(cr);
			}

			return true;
		}

	private:
		UI *ui;
	};

	DrawingArea area;

	Gtk::ScrolledWindow scrollpane;
};

UI::UI(int argc, char **argv)
	: Heap::Client(), pollCount(0), gtkenv(argc, argv)
{
	window = new Window(*this);

	window->show();
}

UI::~UI()
{
	RowSet::iterator first = rows.begin();
	RowSet::iterator last = rows.end();

	for (; first != last; first++) {
		delete (*first);
	}

	rows.clear();

	delete window;
}

bool UI::poll()
{
	while (Gtk::Main::events_pending()) {
		Gtk::Main::iteration();
	}

	/* Periodically update the drawing area dimensions */
	/** \todo Can this be done better? */
	if (++pollCount > 1000) {
		usleep(100);

		window->set_dimensions(Row::NBYTES, rows.size());

		pollCount = 0;
	}

	/* Somewhat nasty hack to detect window being closed */
	return window->is_visible() == false;
}

void UI::redraw(Cairo::RefPtr<Cairo::Context> cr)
{
	RowSet::iterator it = rows.begin();
	RowSet::iterator end = rows.end();
	int32_t linecount = 0;
	double t, r, b, l;

	cr->get_clip_extents(l, t, r, b);

	/* Redraw appropriate rows, culling those outside the clip region */
	for (; it != end; it++) {
		if (linecount >= t && linecount < b) {
			cr->move_to(0, linecount);
			(*it)->redraw(cr);
		}

		linecount++;
	}
}

void UI::chunkModified(Heap::Client::Type type, const Heap::Chunk &chunk)
{
	/* Calculate base address of row */
	uint64_t base = chunk.address & Row::MASK;
	int row_num = -1;

	/* Update each row affected by chunk */
	do {
		Row *row;
		Row r(base);
		RowSet::iterator it = rows.find(&r);

		if (it == rows.end()) {
			/* No row, so create it */
			row = new Row(base);

			std::pair<RowSet::iterator, bool> ret = 
					rows.insert(row);

			it = ret.first;
		} else {
			row = *it;
		}

		/* Update row */
		size_t first, last;
		bytesForChunk(chunk, row->getBase(), first, last);

		if (type == Heap::Client::CREATED)
			row->setBits(first, last);
		else
			row->clearBits(first, last);

		if (row_num == -1) {
			/* Calculate index of first affected row */
			/** \todo Find a better way of doing this */
			RowSet::iterator find = rows.begin();

			for (row_num = 0; find != it; find++)
				row_num++;
		} else {
			row_num++;
		}

		/* Request redraw of affected section of row */
		window->redraw_row(row_num, first, last);

		base += Row::NBYTES;
	} while (base < chunk.address + chunk.length);
}

void UI::bytesForChunk(const Heap::Chunk &chunk, uint64_t rowbase,
		size_t &first, size_t &last)
{
	uint64_t f = chunk.address;
	uint64_t l = chunk.address + chunk.length;

	/* Find first byte in row affected by chunk */
	if (f < rowbase)
		f = rowbase;

	/* Find last byte in row affected by chunk */
	if (l > rowbase + Row::NBYTES)
		l = rowbase + Row::NBYTES;

	first = f - rowbase;
	last = l - rowbase;
}

/******************************************************************************
 * Row                                                                        *
 ******************************************************************************/

void UI::Row::setBits(size_t first, size_t last)
{
	/* Mark affected bytes as allocated */
	for (; first < last; first++) {
		bytes.set(first);
	}
}

void UI::Row::clearBits(size_t first, size_t last)
{
	/* Mark affected bytes as free */
	for (; first < last; first++) {
		bytes.set(first, 0);
	}
}

void UI::Row::redraw(Cairo::RefPtr<Cairo::Context> cr)
{
	/* Don't bother redrawing blank rows */
	if (bytes.none())
		return;

	/* Find the extents of the current clip region */
	double t, r, b, l;
	cr->get_clip_extents(l, t, r, b);

	/* Cull pixels outside clip region */
	const size_t min = l;
	const size_t max = std::min(static_cast<double>(bytes.size()), r);
	int first = -1, last = -1;


	if (bytes.count() == bytes.size()) {
		/* All bits set, so avoid testing them all */
		cr->rel_move_to(min, 0);
		cr->rel_line_to(max, 0);
	} else {
		/* Find spans of allocated bytes */
		for (size_t i = min; i != max; i++) {
			if (bytes.test(i)) {
				if (first == -1) {
					cr->rel_move_to(i - last, 0);

					first = i;
					last = i;
				} else
					last = i;
			} else if (first != -1) {
				cr->rel_line_to(last - first, 0);

				first = -1;
			}
		}

		/* Last span */
		if (first != -1) {
			cr->rel_line_to(last - first, 0);

			first = -1;
		}
	}

	/* Draw the path we've created */
	cr->stroke();
}

