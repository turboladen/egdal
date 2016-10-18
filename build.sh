#!/usr/bin/env sh

gcc -undefined dynamic_lookup -dynamiclib c_src/gdal.c -o egdal_gdal_data_type.so -I/usr/local/opt/erlang/lib/erlang/erts-7.3/include -L/usr/local/opt/gdal/include -lgdal
gcc -undefined dynamic_lookup -dynamiclib c_src/gdal_info.c -o egdal_gdal_info.so -I/usr/local/opt/erlang/lib/erlang/erts-7.3/include -L/usr/local/opt/gdal/include -lgdal
