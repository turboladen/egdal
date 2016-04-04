defmodule Egdal.GDAL.DataType do
  @on_load :load_nifs

  def load_nifs do
    :ok = :erlang.load_nif('./egdal', 0)
  end

  def get_version_info() do
    "NIF not loaded"
  end

  def get_data_type_by_name(_name) do
    "NIF not loaded"
  end

  def get_data_type_name(_index) do
    "NIF not loaded"
  end

  def get_data_type_size(_name) do
    "NIF not loaded"
  end
end
