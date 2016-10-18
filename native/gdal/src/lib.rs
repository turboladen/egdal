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

    let c_str = unsafe {
        let version_ptr = GDALVersionInfo(c_key.as_ptr());
        CStr::from_ptr(version_ptr)
    };

    let meow = str::from_utf8(c_str.to_bytes()).unwrap();

    let mut binary = OwnedNifBinary::alloc(meow.len()).unwrap();
    let _result = binary.as_mut_slice().write_all(meow.as_bytes());

    Ok(binary.release(env).get_term(env))
}
