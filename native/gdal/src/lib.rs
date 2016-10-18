#[macro_use]
extern crate rustler;

use std::ffi::CString;
use std::ffi::CStr;
use std::str;
use std::os::raw::c_char;

use rustler::{NifEnv, NifTerm, NifResult};
use rustler::binary::OwnedNifBinary;

use std::io::Write;

rustler_export_nifs!(
    "Elixir.Egdal.GDALRust",
    [("get_version_info_dawg", 0, get_version_info)],
    Some(on_load)
);

fn on_load(_env: &NifEnv, _load_info: NifTerm) -> bool {
    true
}

#[link(name="gdal")]
extern {
    fn GDALVersionInfo(key: *const c_char) -> *const c_char;
}

fn get_version_info<'a>(env: &'a NifEnv, _args: &Vec<NifTerm>) -> NifResult<NifTerm<'a>> {
    let key = "RELEASE_NAME";
    let c_key = CString::new(key.as_bytes()).unwrap();

    let version_ptr = unsafe { GDALVersionInfo(c_key.as_ptr()) };
    let binary = _c_string_to_binary(version_ptr);

    Ok(binary.release(env).get_term(env))
}

fn _c_string_to_binary(raw_ptr: *const c_char) -> OwnedNifBinary {
    let c_str = unsafe { CStr::from_ptr(raw_ptr) };
    let rust_str = str::from_utf8(c_str.to_bytes()).unwrap();
    let mut binary = OwnedNifBinary::alloc(rust_str.len()).unwrap();
    let _result = binary.as_mut_slice().write_all(rust_str.as_bytes());

    binary
}
