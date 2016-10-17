defmodule Egdal.GDALTest do
  use ExUnit.Case, async: true

  test "get_version_info" do
    assert {:ok, '1.11.5'} = Egdal.GDAL.get_version_info
  end
end
