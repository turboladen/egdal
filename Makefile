CC=gcc
ERLANG_PATH = $(shell erl -eval 'io:format("~s", [lists:concat([code:root_dir(), "/erts-", erlang:system_info(version), "/include"])])' -s init stop -noshell)
GDAL_PATH = /usr/local/opt/gdal/include
# CFLAGS = -undefined dynamic_lookup -dynamiclib -g -O3 -ansi -pedantic -Wall -Wextra
CFLAGS = -I$(ERLANG_PATH) \
				 -L$(GDAL_PATH) \
				 -lgdal

gdal_info:
	$(CC) -undefined dynamic_lookup -dynamiclib \
		-o egdal_gdal_info.so c_src/gdal_info.c \
		$(CFLAGS)
