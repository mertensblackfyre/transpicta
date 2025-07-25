
# shell.nix
let
  pkgs = import <nixpkgs> {};
in
  pkgs.mkShell {
    packages = [
    pkgs.gdb
    pkgs.cmake
    pkgs.pkg-config
    pkgs.meson
    pkgs.ninja
    pkgs.libpng
    pkgs.libwebp
    pkgs.libjpeg
    pkgs.spdlog
];
}
