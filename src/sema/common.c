#include <ofc/sema.h>


ofc_sema_common_t* ofc_sema_common_create(
	ofc_str_ref_t name)
{
	ofc_sema_common_t* common
		= (ofc_sema_common_t*)malloc(
			sizeof(ofc_sema_common_t));
	if (!common) return NULL;

	common->count = 0;
	common->decl  = NULL;
	common->spec  = NULL;

	common->name = name;
	return common;
}

void ofc_sema_common_delete(
	ofc_sema_common_t* common)
{
	if (!common)
		return;

	free(common->decl);
	free(common->spec);
	free(common);
}

bool ofc_sema_common_add(
	ofc_sema_common_t*     common,
	const ofc_sema_spec_t* spec)
{
	if (!common || !spec)
		return false;

	const ofc_sema_decl_t** ndecl
		= (const ofc_sema_decl_t**)realloc(common->decl,
			sizeof(const ofc_sema_decl_t*) * (common->count + 1));
	if (!ndecl) return false;
	common->decl = ndecl;

	const ofc_sema_spec_t** nspec
		= (const ofc_sema_spec_t**)realloc(common->spec,
			sizeof(const ofc_sema_spec_t*) * (common->count + 1));
	if (!nspec) return false;
	common->spec = nspec;

	common->decl[common->count] = NULL;
	common->spec[common->count] = spec;
	common->count++;
	return true;
}

bool ofc_sema_common_define(
	ofc_sema_common_t* common,
	unsigned offset,
	const ofc_sema_decl_t* decl)
{
	if (!common || (offset >= common->count))
		return false;

	if (common->decl[offset])
		return (common->decl[offset] == decl);

	common->decl[offset] = decl;
	return true;
}


static const ofc_str_ref_t* ofc_sema_common__key(
	const ofc_sema_common_t* common)
{
	return (common ? &common->name : NULL);
}


ofc_sema_common_map_t* ofc_sema_common_map_create(
	bool case_sensitive)
{
	ofc_sema_common_map_t* map
		= (ofc_sema_common_map_t*)malloc(
			sizeof(ofc_sema_common_map_t));
	if (!map) return NULL;

	map->map = ofc_hashmap_create(
		(void*)(case_sensitive
			? ofc_str_ref_ptr_hash
			: ofc_str_ref_ptr_hash_ci),
		(void*)(case_sensitive
			? ofc_str_ref_ptr_equal
			: ofc_str_ref_ptr_equal_ci),
		(void*)ofc_sema_common__key, NULL);

	if (!map->map)
	{
		free(map);
		return NULL;
	}

	map->count  = 0;
	map->common = NULL;

	return map;
}

void ofc_sema_common_map_delete(
	ofc_sema_common_map_t* map)
{
	if (!map)
		return;

	unsigned i;
	for (i = 0; i < map->count; i++)
		ofc_sema_common_delete(map->common[i]);
	free(map->common);

	ofc_hashmap_delete(map->map);
	free(map);
}

ofc_sema_common_t* ofc_sema_common_map_find_modify(
	ofc_sema_common_map_t* map, ofc_str_ref_t name)
{
	if (!map)
		return NULL;
	return ofc_hashmap_find_modify(
		map->map, &name);
}

bool ofc_sema_common_map_add(
	ofc_sema_common_map_t* map,
	ofc_sema_common_t* common)
{
	if (!map || !common)
		return false;

	if (ofc_hashmap_find(
		map->map, &common->name))
		return false;

	ofc_sema_common_t** ncommon
		= (ofc_sema_common_t**)realloc(map->common,
			(sizeof(ofc_sema_common_t*) * (map->count + 1)));
	if (!ncommon) return false;
	map->common = ncommon;

	if (!ofc_hashmap_add(
		map->map, common))
		return false;

	map->common[map->count++] = common;
	return true;
}
