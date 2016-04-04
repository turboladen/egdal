defmodule Egdal.GDAL.DataType do
  @on_load :load_nifs

  def load_nifs do
    :ok = :erlang.load_nif('./egdal_gdal_data_type', 0)
  end

  def by_name(_name) do
    "NIF not loaded"
  end

  def name(_index) do
    "NIF not loaded"
  end

  def size(_name) do
    "NIF not loaded"
  end

  def complex?(_name) do
    "NIF not loaded"
  end
end
