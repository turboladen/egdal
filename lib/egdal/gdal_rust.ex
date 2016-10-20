defmodule Egdal.GDALRust do
  @on_load :load_nifs

  def load_nifs do
    :ok = :erlang.load_nif('./rust/target/debug/libgdal', 0)
  end

  def get_version_info(), do: err

  defp err, do: throw :nif_not_loaded
end
