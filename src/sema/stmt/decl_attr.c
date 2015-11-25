#include <ofc/sema.h>


bool ofc_sema_stmt_decl_attr(
	ofc_sema_scope_t* scope,
	const ofc_parse_stmt_t* stmt)
{
	if (!scope || !stmt)
		return false;

	unsigned i;
	for (i = 0; i < stmt->decl_attr.count; i++)
	{
		ofc_str_ref_t decl_name = *stmt->decl_attr.name[i];

		const ofc_sema_decl_t* decl
			= ofc_sema_scope_decl_find(
				scope, decl_name, true);
		if (decl)
		{
			/* TODO - Apply attributes to decl. */
			ofc_sema_scope_error(scope, stmt->src,
				"Declaration attributes must be specified prior to use");
			return false;
		}

		ofc_sema_spec_t* spec
			= ofc_sema_scope_spec_modify(
				scope, decl_name);
		if (!spec)
		{
			ofc_sema_scope_error(scope, stmt->src,
				"No declaration for '%.*s' and no valid IMPLICIT rule",
				decl_name.size, decl_name.base);
			return false;
		}

		switch (stmt->type)
		{
			case OFC_PARSE_STMT_DECL_ATTR_AUTOMATIC:
				if (spec->is_static)
				{
					ofc_sema_scope_error(scope, stmt->src,
						"Specifying '%.*s' as STATIC and AUTOMATIC",
						decl_name.size, decl_name.base);
					return false;
				}
				if (spec->is_automatic)
				{
					ofc_sema_scope_warning(scope, stmt->src,
						"Re-declaring '%.*s' as AUTOMATIC",
						decl_name.size, decl_name.base);
				}
				spec->is_automatic = true;
				break;
			case OFC_PARSE_STMT_DECL_ATTR_STATIC:
				if (spec->is_automatic)
				{
					ofc_sema_scope_error(scope, stmt->src,
						"Specifying '%.*s' as AUTOMATIC and STATIC",
						decl_name.size, decl_name.base);
					return false;
				}
				if (spec->is_static)
				{
					ofc_sema_scope_warning(scope, stmt->src,
						"Re-declaring '%.*s' as STATIC",
						decl_name.size, decl_name.base);
				}
				spec->is_static = true;
				break;
			case OFC_PARSE_STMT_DECL_ATTR_VOLATILE:
				if (spec->is_volatile)
				{
					ofc_sema_scope_warning(scope, stmt->src,
						"Re-declaring '%.*s' as VOLATILE",
						decl_name.size, decl_name.base);
				}
				spec->is_volatile = true;
				break;
			case OFC_PARSE_STMT_DECL_ATTR_EXTERNAL:
				if (spec->is_intrinsic)
				{
					ofc_sema_scope_error(scope, stmt->src,
						"Specifying '%.*s' as INTRINSIC and EXTERNAL",
						decl_name.size, decl_name.base);
					return false;
				}
				if (spec->is_external)
				{
					ofc_sema_scope_warning(scope, stmt->src,
						"Re-declaring '%.*s' as EXTERNAL",
						decl_name.size, decl_name.base);
				}
				spec->is_external = true;
				break;
			case OFC_PARSE_STMT_DECL_ATTR_INTRINSIC:
				if (spec->is_external)
				{
					ofc_sema_scope_error(scope, stmt->src,
						"Specifying '%.*s' as EXTERNAL and INTRINSIC",
						decl_name.size, decl_name.base);
					return false;
				}
				if (spec->is_intrinsic)
				{
					ofc_sema_scope_warning(scope, stmt->src,
						"Re-declaring '%.*s' as INTRINSIC",
						decl_name.size, decl_name.base);
				}
				spec->is_intrinsic = true;
				break;
			default:
				return false;
		}
	}

	return true;
}
