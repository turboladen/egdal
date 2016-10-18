defmodule Egdal.GDAL.DataTypeTest do
  use ExUnit.Case, async: true
  # use ExUnit.Case
  import Egdal.GDAL.DataType

  test ".by_name with valid type" do
    assert {:ok, 8} = by_name(:CInt16)
  end

  test ".by_name with invalid type" do
    assert catch_error(by_name(:CInt1612)) == :badarg
  end

  test ".name with valid type" do
    assert {:ok, :Byte} = name(1)
  end

  test ".name with invalid Integer index" do
    # assert {:error, :Byte} = name(1234567)
    # assert {:error, :Byte} = name('123')
  end

  test ".size with valid type" do
    assert {:ok, 32} = size(:Float32)
  end

  test ".size with invalid type" do
    assert catch_error(size(:Float321)) == :badarg
  end

  test ".complex with valid, complex type" do
    assert {:ok, true} = complex?(:CInt32)
  end

  test ".complex with valid, non-complex type" do
    assert {:ok, false} = complex?(:Int32)
  end

  test ".complex with invalid type" do
    assert catch_error(complex?(:CInt321)) == :badarg
  end

  test ".union with two valid types" do
    assert {:ok, :CFloat64} = union(:CInt32, :Float64)
  end

  test ".union with first type valid, second type invalid" do
    assert catch_error(union(:CInt32, :Float6420)) == :badarg
  end

  test ".union with first type invalid, second type valid" do
    assert catch_error(union(:CInt3210, :Float64)) == :badarg
  end
end
