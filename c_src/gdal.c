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

/***************************************************************************//**
 *
 *  Helper function for making sure we got a good GDALDataType.
 *
 *  @returns 0 if we got a bad data type name (as an Atom); otherwise returns
 *  the length of the Atom.
 *
 ******************************************************************************/
unsigned is_valid_data_type_name(ErlNifEnv* env, int argc, ERL_NIF_TERM e_data_type_name) {
  unsigned length;

  if (argc > 0 && !enif_get_atom_length(env, e_data_type_name, &length, ERL_NIF_LATIN1)) {
    return 0;
  }

  char *c_data_type_name = enif_alloc(sizeof(char)*(length+1));

  if(!enif_get_atom(env, e_data_type_name, c_data_type_name, length+1, ERL_NIF_LATIN1)) {
    return 0;
  }

  GDALDataType data_type = GDALGetDataTypeByName(c_data_type_name);
  int data_type_size = GDALGetDataTypeSize(data_type);

  if(strncmp(c_data_type_name, "Unknown", 7) && data_type_size == 0) {
    enif_free(c_data_type_name);
    // Raise an exception here?
    return 0;
  }

  return length;
}

/***************************************************************************//**
 *
 *  Helper function for extracting a DataType string from the Erlang Atom.
 *
 *  @returns an empty string if we got a bad data type name (as an Atom);
 *  otherwise returns the data type name as a string.
 *
 ******************************************************************************/
unsigned data_type_string_from_atom(ErlNifEnv* env, char *data_type_name, unsigned len, ERL_NIF_TERM data_type_atom) {
  /* char *data_type_name = enif_alloc(sizeof(char)*(len+1)); */

  if(!enif_get_atom(env, data_type_atom, data_type_name, len+1, ERL_NIF_LATIN1)) {
    /* return ""; */
    return 0;
  }

  return 1;
}
/***************************************************************************//**
 *
 * Wrapper for:
 *  GDALDataType GDALGetDataTypeByName(const char *)
 *
 ******************************************************************************/
static ERL_NIF_TERM
get_data_type_by_name(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  unsigned len = is_valid_data_type_name(env, argc, argv[0]);
  if (!len) { return enif_make_badarg(env); }

  char *psz_name = enif_alloc(sizeof(char)*(len+1));

  if(!enif_get_atom(env, argv[0], psz_name, len+1, ERL_NIF_LATIN1)) {
    return enif_make_badarg(env);
  }

  int data_type = GDALGetDataTypeByName(psz_name);
  enif_free(psz_name);

  ERL_NIF_TERM e_data_type = enif_make_int(env, data_type);

  return enif_make_tuple2(env, enif_make_atom(env, "ok"), e_data_type);
}

/***************************************************************************//**
 *
 *  Wrapper for:
 *    const char * GDALGetDataTypeName(GDALDataType)
 *
 * @todo This should return an atom, not a string
 ******************************************************************************/
static ERL_NIF_TERM
get_data_type_name(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  int data_type = 0;
  ERL_NIF_TERM eterm;

  if(!enif_get_int(env, argv[0], &data_type)) {
    /* return enif_make_badarg(env); */
  }

  const char *data_type_name = GDALGetDataTypeName(data_type);
  if (strcmp(data_type_name, "")) {
    /* return enif_make_badarg(env); */
  }

  printf("data type name is %s", data_type_name);

  eterm = enif_make_atom(env, data_type_name);

  return enif_make_tuple2(env, enif_make_atom(env, "ok"), eterm);
}

/***************************************************************************//**
 *
 *  Wrapper for:
 *    int GDALGetDataTypeSize(GDALDataType)
 *
 ******************************************************************************/
static ERL_NIF_TERM
get_data_type_size(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  int data_type_size = 0;
  ERL_NIF_TERM eterm;

  unsigned len = is_valid_data_type_name(env, argc, argv[0]);
  if (!len) { return enif_make_badarg(env); }

  char *data_type_name = enif_alloc(sizeof(char)*(len+1));

  if(!enif_get_atom(env, argv[0], data_type_name, len+1, ERL_NIF_LATIN1)) {
    return enif_make_badarg(env);
  }

  GDALDataType data_type = GDALGetDataTypeByName(data_type_name);
  data_type_size = GDALGetDataTypeSize(data_type);
  eterm = enif_make_int(env, data_type_size);

  if(strncmp(data_type_name, "Unknown", 7) && data_type_size == 0) {
    enif_free(data_type_name);
    // Raise an exception here?
    return enif_make_tuple2(env, enif_make_atom(env, "error"), eterm);
  }

  enif_free(data_type_name);

  return enif_make_tuple2(env, enif_make_atom(env, "ok"), eterm);
}

/***************************************************************************//**
 *
 * Wrapper for:
 *  int GDALDataTypeIsComplex(GDALDataType)
 *
 * @param env Erlang environment.
 * @param argc Arg count.
 * @param argv The args.
 * @returns An Erlang tuple that contains: {"ok", [true or false]} or
 *  {"error", [relavant value].
 *
 ******************************************************************************/
static ERL_NIF_TERM
data_type_is_complex(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  int is_complex;
  ERL_NIF_TERM eterm;

  unsigned len = is_valid_data_type_name(env, argc, argv[0]);
  if (!len) { return enif_make_badarg(env); }

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

/***************************************************************************//**
 *
 *  GDALDataType GDALDataTypeUnion(GDALDataType, GDALDataType)
 *
 ******************************************************************************/
static ERL_NIF_TERM
data_type_union(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  unsigned len0 = is_valid_data_type_name(env, argc, argv[0]);
  if (!len0) { return enif_make_badarg(env); }

  unsigned len1 = is_valid_data_type_name(env, argc, argv[1]);
  if (!len1) { return enif_make_badarg(env); }

  char *name0 = enif_alloc(sizeof(char) * (len0 + 1));
  char *name1 = enif_alloc(sizeof(char) * (len1 + 1));

  if(!enif_get_atom(env, argv[0], name0, len0+1, ERL_NIF_LATIN1)) {
    return enif_make_badarg(env);
  }

  if(!enif_get_atom(env, argv[1], name1, len1+1, ERL_NIF_LATIN1)) {
    return enif_make_badarg(env);
  }

  GDALDataType data_type0 = GDALGetDataTypeByName(name0);
  GDALDataType data_type1 = GDALGetDataTypeByName(name1);
  enif_free(name0);
  enif_free(name1);
  GDALDataType union_data_type = GDALDataTypeUnion(data_type0, data_type1);
  const char * union_data_type_name = GDALGetDataTypeName(union_data_type);

  return enif_make_tuple2(env, enif_make_atom(env, "ok"), enif_make_atom(env, union_data_type_name));
}

/***************************************************************************//**
 *
 *  Function exports
 *
 ******************************************************************************/
static ErlNifFunc nif_funcs[] =
{
    {"by_name",   1, get_data_type_by_name},
    {"name",      1, get_data_type_name},
    {"size",      1, get_data_type_size},
    {"complex?",  1, data_type_is_complex},
    {"union",     2, data_type_union}
};

ERL_NIF_INIT(Elixir.Egdal.GDAL.DataType, nif_funcs, &load, NULL, NULL, unload);
