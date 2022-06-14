{
  inputs = {
    nixpkgs.url = github:nixos/nixpkgs/nixos-unstable;
    flake-utils.url = github:numtide/flake-utils;
  };

  outputs = { self, nixpkgs, flake-utils, ... }: flake-utils.lib.eachDefaultSystem ( system:
    let
      pkgs = import nixpkgs {
        inherit system;
      };

      cfetch = (with pkgs; stdenv.mkDerivation {
        name = "cfetch";
        src = self;
        nativeBuildInputs = [
          gcc
          gnumake
        ];
        installPhase = ''
          mkdir -p $out/bin
          mv cfetch $out/bin/
        '';
      });
    in rec {
      packages.default = cfetch;
      apps.default = flake-utils.lib.mkApp {
        drv = packages.default;
      };
      devShells.default = pkgs.mkShell {
        buildInputs = [
          cfetch
        ];
      };
    }
  );
}
