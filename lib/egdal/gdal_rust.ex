defmodule Egdal.GDALRust do
  @on_load :load_nifs

  def load_nifs do
    # :ok = :erlang.load_nif('./rust/target/debug/libegdal.dylib', 0)
    :ok = :erlang.load_nif('./rust/target/debug/libegdal', 0)
  end

  def get_version_info_meow(), do: err

  defp err, do: throw :nif_not_loaded
end
