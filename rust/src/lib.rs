#[macro_use]
extern crate erlang_nif_sys;
extern crate libc;

use erlang_nif_sys::*;

pub mod data_type;
pub mod version_info;

extern "C" fn load(
    _env:       *mut ErlNifEnv,
    _priv_data: *mut *mut c_void,
    _load_info: ERL_NIF_TERM
) -> c_int {
    unsafe { GDALAllRegister() };
    0
}

extern "C" fn reload(
    _env:       *mut ErlNifEnv,
    _priv_data: *mut *mut c_void,
    _load_info: ERL_NIF_TERM
) -> c_int { 0 }

extern "C" fn upgrade(
    _env:           *mut ErlNifEnv,
    _priv_data:     *mut *mut c_void,
    _old_priv_data: *mut *mut c_void,
    _load_info:     ERL_NIF_TERM
) -> c_int { 0 }

extern "C" fn unload(
    _env:       *mut ErlNifEnv,
    _priv_data: *mut c_void
) {}

nif_init!(
    b"Elixir.Egdal.GDAL\0",
    Some(load), Some(reload), Some(upgrade), Some(unload),
    nif!(b"get_version_info\0", 0, version_info::get_version_info),
    nif!(b"get_data_type_by_name\0", 1, data_type::get_data_type_by_name)
);

#[link(name="gdal")]
extern {
    fn GDALAllRegister() -> c_void;
}
