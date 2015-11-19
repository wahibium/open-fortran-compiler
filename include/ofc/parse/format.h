#ifndef __ofc_parse_format_h__
#define __ofc_parse_format_h__

typedef enum
{
	OFC_PARSE_FORMAT_DESC_INTEGER = 0,
	OFC_PARSE_FORMAT_DESC_REAL,
	OFC_PARSE_FORMAT_DESC_D,
	OFC_PARSE_FORMAT_DESC_E,
	OFC_PARSE_FORMAT_DESC_G,
	OFC_PARSE_FORMAT_DESC_CHARACTER,
	OFC_PARSE_FORMAT_DESC_LOGICAL,
	OFC_PARSE_FORMAT_DESC_HOLLERITH,
	OFC_PARSE_FORMAT_DESC_S,
	OFC_PARSE_FORMAT_DESC_REAL_SCALE,
	OFC_PARSE_FORMAT_DESC_X,
	OFC_PARSE_FORMAT_DESC_T,
	OFC_PARSE_FORMAT_DESC_SLASH,
	OFC_PARSE_FORMAT_DESC_DOLLAR,
	OFC_PARSE_FORMAT_DESC_BACKSLASH,
	OFC_PARSE_FORMAT_DESC_Q,
	OFC_PARSE_FORMAT_DESC_COLON,
	OFC_PARSE_FORMAT_DESC_BN,
	OFC_PARSE_FORMAT_DESC_BZ,
	OFC_PARSE_FORMAT_DESC_SP,
	OFC_PARSE_FORMAT_DESC_SS,
	OFC_PARSE_FORMAT_DESC_TL,
	OFC_PARSE_FORMAT_DESC_TR,
	OFC_PARSE_FORMAT_DESC_STRING,
	OFC_PARSE_FORMAT_DESC_REPEAT,
	OFC_PARSE_FORMAT_DESC_BINARY,
	OFC_PARSE_FORMAT_DESC_OCTAL,
	OFC_PARSE_FORMAT_DESC_HEX,

	OFC_PARSE_FORMAT_DESC_COUNT
} ofc_parse_format_desc_e;

typedef struct ofc_parse_format_desc_s ofc_parse_format_desc_t;
typedef struct ofc_parse_format_desc_list_s ofc_parse_format_desc_list_t;

struct ofc_parse_format_desc_s
{
	ofc_parse_format_desc_e type;

	bool neg;
	unsigned n;
	union
	{
		struct
		{
			unsigned w, d, e;
		};

		ofc_string_t* string;

		ofc_parse_format_desc_list_t* repeat;
	};
};

struct ofc_parse_format_desc_list_s
{
	unsigned                  count;
	ofc_parse_format_desc_t** desc;
};


ofc_parse_format_desc_t* ofc_parse_format_desc(
	const ofc_sparse_t* src, const char* ptr,
	ofc_parse_debug_t* debug,
	unsigned* len);
void ofc_parse_format_desc_delete(
	ofc_parse_format_desc_t* desc);
bool ofc_parse_format_desc_print(
	ofc_colstr_t* cs, const ofc_parse_format_desc_t* desc);

bool ofc_parse_format_is_data_desc(
	ofc_parse_format_desc_t* desc);

ofc_parse_format_desc_list_t* ofc_parse_format_desc_list(
	const ofc_sparse_t* src, const char* ptr,
	ofc_parse_debug_t* debug,
	unsigned* len);
void ofc_parse_format_desc_list_delete(
	ofc_parse_format_desc_list_t* list);
bool ofc_parse_format_desc_list_print(
	ofc_colstr_t* cs, const ofc_parse_format_desc_list_t* list);

#endif
