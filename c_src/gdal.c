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

/************************************************************************
 *
 *  GDALDataType GDALGetDataTypeByName(const char *)
 *
 ***********************************************************************/
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

/************************************************************************
 *
 *  const char * GDALGetDataTypeName(GDALDataType)
 *
 * TODO: This should return an atom, not a string
 ***********************************************************************/
static ERL_NIF_TERM
get_data_type_name(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  int data_type = 0;
  ERL_NIF_TERM eterm;

  enif_get_int(env, argv[0], &data_type);

  const char *data_type_name = GDALGetDataTypeName(data_type);

  eterm = enif_make_string(env, data_type_name, ERL_NIF_LATIN1);

  return enif_make_tuple2(env, enif_make_atom(env, "ok"), eterm);
}

/************************************************************************
 *
 *  int GDALGetDataTypeSize(GDALDataType)
 *
 ***********************************************************************/
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

/************************************************************************
 *
 *  int GDALDataTypeIsComplex(GDALDataType)
 *
 ***********************************************************************/
static ERL_NIF_TERM
data_type_is_complex(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  int is_complex;
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

  if(strncmp(data_type_name, "Unknown", 7) && data_type == 0) {
    enif_free(data_type_name);
    // Raise an exception here?
    return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_int(env, data_type));
  }

  enif_free(data_type_name);
  is_complex = GDALDataTypeIsComplex(data_type);
  const char *bool_name = is_complex ? "true" : "false";

  eterm = enif_make_atom(env, bool_name);

  return enif_make_tuple2(env, enif_make_atom(env, "ok"), eterm);
}

static ErlNifFunc nif_funcs[] =
{
    {"by_name", 1, get_data_type_by_name},
    {"name", 1, get_data_type_name},
    {"size", 1, get_data_type_size},
    {"complex?", 1, data_type_is_complex}
};

ERL_NIF_INIT(Elixir.Egdal.GDAL.DataType, nif_funcs, &load, NULL, NULL, unload);
