extern crate erlang_nif_sys;

use erlang_nif_sys::*;
use std::ffi::CString;
use std::mem::uninitialized;

#[repr(C)]
enum GDALDataType {
    Unknown,
    Byte,
    UInt16,
    Int16,
    UInt32,
    Int32,
    Float32,
    Float64,
    CInt16,
    CInt32,
    CFloat32,
    CFloat64,
    TypeCount
}

#[link(name="gdal")]
extern {
    fn GDALGetDataTypeByName(name: *const c_char) -> GDALDataType;
}

pub extern "C" fn get_data_type_by_name(
    env:  *mut ErlNifEnv,
    argc: c_int,
    argv: *const ERL_NIF_TERM
) -> ERL_NIF_TERM {
    if argc != 1 { return unsafe { enif_make_badarg(env) } };

    unsafe {
        let string_length = {
            let mut string_length: u32 = uninitialized();
            enif_get_atom_length(env, *argv, &mut string_length, ErlNifCharEncoding::ERL_NIF_LATIN1);

            string_length
        };

        let mut c_name: [i8] = uninitialized();
        let name = enif_get_string(env, *argv, &mut c_name, string_length, ErlNifCharEncoding::ERL_NIF_LATIN1);
        // let c_name = CString::new(name.as_bytes()).unwrap();
        let data_type = GDALGetDataTypeByName(c_name.as_ptr()) as i32;
        enif_make_int(env, data_type)
    }
}
