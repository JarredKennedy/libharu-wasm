src_files = hpdf_annotation.c hpdf_array.c hpdf_binary.c hpdf_boolean.c \
			hpdf_catalog.c hpdf_destination.c hpdf_dict.c hpdf_doc.c \
			hpdf_doc_png.c hpdf_encoder.c hpdf_encoder_cns.c hpdf_encoder_cnt.c \
			hpdf_encoder_jp.c hpdf_encoder_kr.c hpdf_encrypt.c hpdf_encryptdict.c \
			hpdf_error.c hpdf_ext_gstate.c hpdf_font.c hpdf_font_cid.c \
			hpdf_fontdef_base14.c hpdf_fontdef.c hpdf_fontdef_cid.c \
			hpdf_fontdef_cns.c hpdf_fontdef_cnt.c hpdf_fontdef_jp.c \
			hpdf_fontdef_kr.c hpdf_fontdef_tt.c hpdf_fontdef_type1.c \
			hpdf_font_tt.c hpdf_font_type1.c hpdf_gstate.c hpdf_image.c \
			hpdf_image_ccitt.c hpdf_image_png.c hpdf_info.c hpdf_list.c hpdf_mmgr.c hpdf_name.c \
			hpdf_namedict.c hpdf_null.c hpdf_number.c hpdf_objects.c hpdf_outline.c \
			hpdf_page_label.c hpdf_page_operator.c hpdf_pages.c hpdf_real.c \
			hpdf_streams.c hpdf_string.c hpdf_u3d.c hpdf_utils.c hpdf_xref.c hpdf_pdfa.c \
			hpdf_3dmeasure.c hpdf_exdata.c hpdf_encoder_utf.c

object_files = hpdf_annotation.o hpdf_array.o hpdf_binary.o hpdf_boolean.o \
			hpdf_catalog.o hpdf_destination.o hpdf_dict.o hpdf_doc.o \
			hpdf_doc_png.o hpdf_encoder.o hpdf_encoder_cns.o hpdf_encoder_cnt.o \
			hpdf_encoder_jp.o hpdf_encoder_kr.o hpdf_encrypt.o hpdf_encryptdict.o \
			hpdf_error.o hpdf_ext_gstate.o hpdf_font.o hpdf_font_cid.o \
			hpdf_fontdef_base14.o hpdf_fontdef.o hpdf_fontdef_cid.o \
			hpdf_fontdef_cns.o hpdf_fontdef_cnt.o hpdf_fontdef_jp.o \
			hpdf_fontdef_kr.o hpdf_fontdef_tt.o hpdf_fontdef_type1.o \
			hpdf_font_tt.o hpdf_font_type1.o hpdf_gstate.o hpdf_image.o \
			hpdf_image_ccitt.o hpdf_image_png.o hpdf_info.o hpdf_list.o hpdf_mmgr.o hpdf_name.o \
			hpdf_namedict.o hpdf_null.o hpdf_number.o hpdf_objects.o hpdf_outline.o \
			hpdf_page_label.o hpdf_page_operator.o hpdf_pages.o hpdf_real.o \
			hpdf_streams.o hpdf_string.o hpdf_u3d.o hpdf_utils.o hpdf_xref.o hpdf_pdfa.o \
			hpdf_3dmeasure.o hpdf_exdata.o hpdf_encoder_utf.o

libhpdf.a: $(src_files)
	emcc -s USE_ZLIB=1 -s USE_LIBPNG=1 -I../include -c $(src_files)
	emar rcs libhpdf.a $(object_files)

clean:
	rm -f *.o
	rm -f libhpdf.a