defmodule Egdal.Mixfile do
  use Mix.Project

  def project do
    [app: :egdal,
     version: "0.0.1",
     elixir: "~> 1.3",
     compilers: [:rustler] ++ Mix.compilers,
     rustler_crates: [
       gdal: [
         path: "/native/gdal",
         mode: (if Mix.env == :prod, do: :release, else: :debug),
       ]
     ],
     build_embedded: Mix.env == :prod,
     start_permanent: Mix.env == :prod,
     deps: deps]
  end

  # Configuration for the OTP application
  #
  # Type "mix help compile.app" for more information
  def application do
    [applications: [:logger]]
  end

  # Dependencies can be Hex packages:
  #
  #   {:mydep, "~> 0.3.0"}
  #
  # Or git/path repositories:
  #
  #   {:mydep, git: "https://github.com/elixir-lang/mydep.git", tag: "0.1.0"}
  #
  # Type "mix help deps" for more examples and options
  defp deps do
    [{:rustler, "~> 0.1"}]
  end
end
