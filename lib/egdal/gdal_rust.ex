defmodule Egdal.GDALRust do
  require Rustler

  @on_load :load_nifs

  def load_nifs do
    Rustler.load_nif("gdal")
  end

  def get_version_info_dawg(), do: err

  defp err, do: throw :nif_not_loaded
end
