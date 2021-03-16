#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "hpdf.h"

emscripten::val Uint8Array = emscripten::val::global("Uint8Array");

void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data);

class JSAPI_PDF {

	private:
		HPDF_Doc pdf;
		HPDF_Font font;

	public:
		float x;
		float y;
		int pageNumber;
		float pageHeight;
		float pageWidth;
		HPDF_STATUS errorNumber;
		HPDF_STATUS errorDetailNumber;

		JSAPI_PDF() : x(0), y(0), pageNumber(0), pageHeight(0), pageWidth(0), errorNumber(0), errorDetailNumber(0)
		{
			/*
				I think what needs to be done is to pass this instance to HPDF_NEW as the user_data paraeter.
				The error_handler will be a top-level non-member function which receives user_data and can then call
				user_data->handleError().
			*/

			pdf = HPDF_New(error_handler, this);

			if (!pdf) {
				printf ("I have no idea how I'm going to handle this.\n");
				return;
			}

			font = HPDF_GetFont (pdf, "Helvetica", NULL);
		}

		void addPage()
		{
			HPDF_AddPage(pdf);
			HPDF_Page_SetFontAndSize (pdf->cur_page, font, 20);
		}

		emscripten::val save()
		{
			HPDF_UINT streamSize;
			HPDF_BYTE *buf;
			// This can fail, it should be checked.
			HPDF_SaveToStream(pdf);

			streamSize = pdf->stream->size_fn(pdf->stream);
			pdf->stream->read_fn(pdf->stream, buf, &streamSize);

			// Stolen from squoosh, like the rest of it.
			auto js_result = Uint8Array.new_(emscripten::typed_memory_view(streamSize, buf));

			return js_result;
		}

		void handleError(
			HPDF_STATUS error_no,
            HPDF_STATUS detail_no
		)
		{
			errorNumber = error_no;
			errorDetailNumber = detail_no;
		}

};

void error_handler(
	HPDF_STATUS error_no,
    HPDF_STATUS detail_no,
    void *user_data)
{
	printf("There was some error");
    ((JSAPI_PDF *) user_data)->handleError(error_no, detail_no);
}

EMSCRIPTEN_BINDINGS(libharu_wasm) {
	emscripten::class_<JSAPI_PDF>("JSAPI_PDF")
		.constructor<>()
		.function("addPage", &JSAPI_PDF::addPage)
		.function("save", &JSAPI_PDF::save);
}