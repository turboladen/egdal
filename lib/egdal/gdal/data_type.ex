defmodule Egdal.GDAL.DataType do
  @on_load :load_nifs

  def load_nifs do
    :ok = :erlang.load_nif('./egdal_gdal_data_type', 0)
  end

  @doc """
  Gets the DataType using the shortened `name`.

  * Parameters
    * _name: Atom that represents the type name.
  * Returns a tuple
    * *Atom* `:ok` or `:error`
    * *Integer* equivalent to the GDALDataType enum value.
  """
  def by_name(_name) do
    {:error, "NIF not loaded"}
  end

  @doc """
  Gets the DataType using the shortened `name`.

  * Parameters
    * _index: *Integer* equal to an GDALDataType enum value.
  * Returns a tuple
    * *Atom* `:ok` or `:error`
    * *Atom* equivalent to the GDALDataType name.
  """
  def name(_index) do
    {:error, "NIF not loaded"}
  end

  @doc """
  Gets the size (in bits) of the DataType using the shortened `name`.

  * Parameters
    * _name: Atom that represents the type name.
  * Returns a tuple
    * *Atom* `:ok` or `:error`
    * *Integer* of the GDALDataType size.
  """
  def size(_name) do
    {:error, "NIF not loaded"}
  end

  @doc """
  Gets the size (in bits) of the DataType using the shortened `name`.

  * Parameters
    * _name: Atom that represents the type name.
  * Returns a tuple
    * *Atom* `:ok` or `:error`
    * *Atom*, either `:true` or `:false`.
  """
  def complex?(_name) do
    {:error, "NIF not loaded"}
  end

  end
end
