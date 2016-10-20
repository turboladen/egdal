extern crate erlang_nif_sys;

use erlang_nif_sys::*;
use std::ffi::CString;
use std::ffi::CStr;
use std::str;

#[link(name="gdal")]
extern {
    fn GDALVersionInfo(key: *const c_char) -> *const c_char;
}

pub extern "C" fn get_version_info (
    env:  *mut ErlNifEnv,
    argc: c_int,
    _args: *const ERL_NIF_TERM
) -> ERL_NIF_TERM {
    if argc != 0 { return unsafe { enif_make_badarg(env) } }

    let key = "RELEASE_NAME";
    let c_key = CString::new(key.as_bytes()).unwrap();

    let v = unsafe { GDALVersionInfo(c_key.as_ptr()) };
    let c_str = unsafe { CStr::from_ptr(v) };
    let meow = str::from_utf8(c_str.to_bytes()).unwrap();

    unsafe { enif_make_string(env, meow.as_ptr(), ErlNifCharEncoding::ERL_NIF_LATIN1) }
}
