#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "hpdf.h"

#define FONT_STYLE_NORMAL 0x01
#define FONT_STYLE_ITALIC 0x02
#define FONT_STYLE_BOLD 0x04

emscripten::val Uint8Array = emscripten::val::global("Uint8Array");

void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data);

class JSAPI_PDF {

	private:
		HPDF_Doc pdf;
		HPDF_Font font;
		HPDF_Font normalFont;
		HPDF_Font boldFont;
		HPDF_Font italicFont;
		HPDF_Font boldItalicFont;

		// PDF coordinates start at the bottom-left corner. Flipping
		// the y value makes the coordinates relative to the top-left
		// corner.
		int flipY(int y)
		{
			return getPageHeight() - y;
		}

	public:
		HPDF_STATUS errorNumber;
		HPDF_STATUS errorDetailNumber;

		HPDF_REAL getPageHeight() const
		{
			if (!HPDF_Page_Validate(pdf->cur_page)) {
				return 0;
			}

			return HPDF_Page_GetHeight(pdf->cur_page);
		}

		HPDF_REAL getPageWidth() const
		{
			if (!HPDF_Page_Validate(pdf->cur_page)) {
				return 0;
			}

			return HPDF_Page_GetWidth(pdf->cur_page);
		}

		JSAPI_PDF() : errorNumber(0), errorDetailNumber(0)
		{
			/*
				I think what needs to be done is to pass this instance to HPDF_NEW as the user_data parameter.
				The error_handler will be a top-level non-member function which receives user_data and can then call
				user_data->handleError().
			*/

			pdf = HPDF_New(error_handler, this);

			if (!pdf) {
				printf ("I have no idea how I'm going to handle this.\n");
				return;
			}

			normalFont = HPDF_GetFont(pdf, "Helvetica", NULL);
			font = normalFont;
		}

		void addPage()
		{
			HPDF_AddPage(pdf);
			HPDF_Page_SetSize(pdf->cur_page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
			HPDF_Page_SetFontAndSize (pdf->cur_page, font, 20);
		}

		void fillText(std::string text, int x, int y)
		{
			HPDF_REAL fontHeight = HPDF_Font_GetCapHeight(font) * HPDF_Page_GetCurrentFontSize(pdf->cur_page) / 1000;

			HPDF_Page_BeginText(pdf->cur_page);
			HPDF_Page_TextOut(pdf->cur_page, x, flipY(y) - fontHeight, text.c_str());
			HPDF_Page_EndText(pdf->cur_page);
		}

		void writeJpegImage(std::string imageData, int width, int height, int x, int y)
		{
			HPDF_UINT imageSize = imageData.size();
			uint8_t* imageBuf = (uint8_t*)imageData.c_str();
			HPDF_Image image = HPDF_LoadJpegImageFromMem(pdf, imageBuf, imageSize);
			HPDF_Page_DrawImage(pdf->cur_page, image, x, flipY(y) - height, width, height);
		}

		void writePngImage(std::string imageData, int width, int height, int x, int y)
		{
			HPDF_UINT imageSize = imageData.size();
			uint8_t* imageBuf = (uint8_t*)imageData.c_str();
			HPDF_Image image = HPDF_LoadPngImageFromMem(pdf, imageBuf, imageSize);
			HPDF_Page_DrawImage(pdf->cur_page, image, x, flipY(y) - height, width, height);
		}

		void setFontStyle(uint8_t fontStyle)
		{
			bool isItalic = ((fontStyle & FONT_STYLE_ITALIC) == FONT_STYLE_ITALIC);
			bool isBold = ((fontStyle & FONT_STYLE_BOLD) == FONT_STYLE_BOLD);

			if (isItalic && isBold) {
				if (!HPDF_Font_Validate(boldItalicFont)) {
					boldItalicFont = HPDF_GetFont(pdf, "Helvetica-BoldOblique", NULL);
				}
				font = boldItalicFont;
			} else if (isItalic) {
				if (!HPDF_Font_Validate(italicFont)) {
					italicFont = HPDF_GetFont(pdf, "Helvetica-Oblique", NULL);
				}
				font = italicFont;
			} else if (isBold) {
				if (!HPDF_Font_Validate(boldFont)) {
					boldFont = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);
				}
				font = boldFont;
			} else {
				if (!HPDF_Font_Validate(normalFont)) {
					normalFont = HPDF_GetFont(pdf, "Helvetica", NULL);
				}
				font = normalFont;
			}

			HPDF_Page_SetFontAndSize(pdf->cur_page, font, 20);
		}

		emscripten::val save()
		{
			HPDF_UINT streamSize;
			HPDF_BYTE *buf;
			// This can fail, it should be checked.
			HPDF_SaveToStream(pdf);

			streamSize = pdf->stream->size_fn(pdf->stream);

			buf = (HPDF_BYTE *)HPDF_GetMem(pdf->mmgr, streamSize);

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
		.property("pageHeight", &JSAPI_PDF::getPageHeight)
		.property("pageWidth", &JSAPI_PDF::getPageWidth)
		.function("addPage", &JSAPI_PDF::addPage)
		.function("fillText", &JSAPI_PDF::fillText)
		.function("setFontStyle", &JSAPI_PDF::setFontStyle)
		.function("writeJpegImage", &JSAPI_PDF::writeJpegImage)
		.function("writePngImage", &JSAPI_PDF::writePngImage)
		.function("save", &JSAPI_PDF::save);
}