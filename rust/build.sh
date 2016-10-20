#/bin/bash

cargo rustc --lib -- --codegen link-args='-flat_namespace -undefined suppress'
cp -f "./target/debug/libgdal.dylib" "./target/debug/libgdal.so"
