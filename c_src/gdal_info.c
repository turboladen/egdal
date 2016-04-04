#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <gdal.h>

#include "erl_nif.h"

/* From https://github.com/iamaleksey/iconverl/blob/master/c_src/iconverl.c */
  static int
load(ErlNifEnv *env, void **priv, ERL_NIF_TERM load_info)
{
  return 0;
}

  void
unload(ErlNifEnv* env, void* priv_data)
{
  // ?
}

/************************************************************************
 *
 *  GDALVersionInfo
 *
 ***********************************************************************/
static ERL_NIF_TERM
get_version_info(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  /* char *psz_request; */
  const char *psz_request = "RELEASE_NAME";
  ERL_NIF_TERM eterm;

  if (argc != 0) {
    return enif_make_badarg(env);
  }

  const char *version_info = GDALVersionInfo(psz_request);
  eterm = enif_make_string(env, version_info, ERL_NIF_LATIN1);
  /* free(psz_request); */

  return enif_make_tuple2(env, enif_make_atom(env, "ok"), eterm);
}

static ErlNifFunc nif_funcs[] =
{
  {"get_version_info", 0, get_version_info}
};

ERL_NIF_INIT(Elixir.Egdal.GDAL, nif_funcs, &load, NULL, NULL, unload);
