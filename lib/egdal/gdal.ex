defmodule Egdal.GDAL do
  @on_load :load_nifs

  def load_nifs do
    :ok = :erlang.load_nif('./egdal_gdal_info', 0)
  end

  def get_version_info() do
    "NIF not loaded"
  end
end
