/* #include <arpa/inet.h> */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <gdal.h>

#include "erl_nif.h"

/* From https://github.com/iamaleksey/iconverl/blob/master/c_src/iconverl.c */
static int
load(ErlNifEnv *env, void **priv, ERL_NIF_TERM load_info)
{
      GDALAllRegister();
      return 0;
}

void
unload(ErlNifEnv* env, void* priv_data)
{
      // ?
}

/* static int */
/* upgrade(ErlNifEnv *env, void **priv,, void **old_priv, ERL_NIF_TERM load_info) */
/* { */
/*   GDALAllRegister(); */
/*   return 0; */
/* } */

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

static ERL_NIF_TERM
get_data_type_by_name(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  unsigned len;
  if (argc > 0 && !enif_get_atom_length(env, argv[0], &len, ERL_NIF_LATIN1)) {
    return enif_make_badarg(env);
  }
  char *psz_name = enif_alloc(sizeof(char)*(len+1));

  if(!enif_get_atom(env, argv[0], psz_name, len+1, ERL_NIF_LATIN1)) {
    return enif_make_badarg(env);
  }

  int data_type = GDALGetDataTypeByName(psz_name);
  ERL_NIF_TERM e_data_type = enif_make_int(env, data_type);
  enif_free(psz_name);

  return enif_make_tuple2(env, enif_make_atom(env, "ok"), e_data_type);
}

static ERL_NIF_TERM
get_data_type_name(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  int data_type = 0;
  ERL_NIF_TERM eterm;

  enif_get_int(env, argv[0], &data_type);

  const char *data_type_name = GDALGetDataTypeName(data_type);

  eterm = enif_make_string(env, data_type_name, ERL_NIF_LATIN1);

  return enif_make_tuple2(env, enif_make_atom(env, "ok"), eterm);
}

static ERL_NIF_TERM
get_data_type_size(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  int data_type_size = 0;
  ERL_NIF_TERM eterm;

  unsigned len;
  if (argc > 0 && !enif_get_atom_length(env, argv[0], &len, ERL_NIF_LATIN1)) {
    return enif_make_badarg(env);
  }
  char *data_type_name = enif_alloc(sizeof(char)*(len+1));

  if(!enif_get_atom(env, argv[0], data_type_name, len+1, ERL_NIF_LATIN1)) {
    return enif_make_badarg(env);
  }

  GDALDataType data_type = GDALGetDataTypeByName(data_type_name);
  data_type_size = GDALGetDataTypeSize(data_type);
  eterm = enif_make_int(env, data_type_size);

  if(strncmp(data_type_name, "Unknown", 7) && data_type_size == 0) {
    // Raise an exception here?
    return enif_make_tuple2(env, enif_make_atom(env, "error"), eterm);
  }

  return enif_make_tuple2(env, enif_make_atom(env, "ok"), eterm);
}

static ErlNifFunc nif_funcs[] =
{
    {"get_version_info", 0, get_version_info},
    {"get_data_type_by_name", 1, get_data_type_by_name},
    {"get_data_type_name", 1, get_data_type_name},
    {"get_data_type_size", 1, get_data_type_size}
};

/* ERL_NIF_INIT(Elixir.Egdal, nif_funcs, &load, NULL, &upgrade, unload); */
ERL_NIF_INIT(Elixir.Egdal.GDAL.DataType, nif_funcs, &load, NULL, NULL, unload);
