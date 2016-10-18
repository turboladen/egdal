#[macro_use]
extern crate rustler;
extern crate gdal;

use rustler::{NifEnv, NifTerm, NifError, NifDecoder, NifEncoder, NifResult};
use rustler::resource::ResourceCell;
use rustler::atom::{init_atom, get_atom};
use rustler::tuple::make_tuple;
use rustler::binary::OwnedNifBinary;
use gdal::version::version_info;

use std::io::Write;

rustler_export_nifs!(
    "Elixir.Egdal.GDALRust",
    [("get_version_info_dawg", 0, get_version_info)],
    Some(on_load)
);

fn on_load(env: &NifEnv, load_info: NifTerm) -> bool {
    true
}

fn get_version_info<'a>(env: &'a NifEnv, args: &Vec<NifTerm>) -> NifResult<NifTerm<'a>> {
    let psz_request = "RELEASE_NAME";
    let version = gdal::version::version_info("RELEASE_NAME");
    let mut binary = OwnedNifBinary::alloc(version.len()).unwrap();
    binary.as_mut_slice().write_all(version.as_bytes());

    Ok(binary.release(env).get_term(env))
}
