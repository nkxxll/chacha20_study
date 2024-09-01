{
  description = "C project with Clang, Clangd, Clang-Format, and Make";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "nixpkgs/nixos-unstable";
  };


  outputs = inputs @ {flake-parts, ...}:
    flake-parts.lib.mkFlake {inherit inputs;} {
      imports = [];
      systems = ["x86_64-linux" "aarch64-linux" "aarch64-darwin" "x86_64-darwin"];
      perSystem = {
        config,
        self',
        inputs',
        pkgs,
        system,
        ...
      }:
      let
      llvm = pkgs.llvmPackages_latest;
      in{
        devShells = {
          default = pkgs.mkShell {
            buildInputs = with pkgs; [
              bear
              clang-tools         # This includes clangd, clang-format, etc.
              gnumake             # The build tool
              llvm.libstdcxxClang # LSP and compiler
            ];
          };
        };
      };
    };
}
