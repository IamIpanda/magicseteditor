//+----------------------------------------------------------------------------+
//| Description:  Magic Set Editor - Program to make Magic (tm) cards          |
//| Copyright:    (C) 2001 - 2006 Twan van Laarhoven                           |
//| License:      GNU General Public License 2 or later (see file COPYING)     |
//+----------------------------------------------------------------------------+

// ----------------------------------------------------------------------------- : Includes

#include <render/value/symbol.hpp>
#include <render/symbol/filter.hpp>
#include <data/set.hpp>
#include <data/symbol.hpp>
#include <gui/util.hpp> // draw_checker
#include <util/error.hpp>

DECLARE_TYPEOF_COLLECTION(SymbolStyle::VariationP);

// ----------------------------------------------------------------------------- : SymbolValueViewer

void SymbolValueViewer::draw(RotatedDC& dc) {
	drawFieldBorder(dc);
	// draw checker background
	draw_checker(dc, style().getRect());
	double wh = min(style().width, style().height);
	// try to load symbol
	if (symbols.empty() && !value().filename.empty()) {
		try {
			// load symbol
			SymbolP symbol = getSet().readFile<SymbolP>(value().filename);
			// render and filter variations
			FOR_EACH(variation, style().variations) {
				Image img = render_symbol(symbol, *variation->filter, variation->border_radius);
				Image resampled(wh, wh, false);
				resample(img, resampled);
				symbols.push_back(Bitmap(resampled));
			}
		} catch (const Error& e) {
			handle_error(e);
		}
	}
	// draw image, if any
	for (size_t i = 0 ; i < symbols.size() ; ++i) {
		// todo : labels?
		dc.DrawBitmap(symbols[i], style().getPos() + RealSize(i * (wh + 2), 0));
	}
	// draw helper text if there are no symbols
	if (symbols.empty()) {
		dc.SetFont(wxFont(10,wxSWISS,wxNORMAL,wxNORMAL));
		dc.SetTextForeground(*wxBLACK);
		RealSize text_size = dc.GetTextExtent(_("double click to edit symbol"));
		dc.DrawText(_("double click to edit symbol"), align_in_rect(ALIGN_MIDDLE_CENTER, text_size, style().getRect()));
	}
}

void SymbolValueViewer::onValueChange() {
	symbols.clear();
}
