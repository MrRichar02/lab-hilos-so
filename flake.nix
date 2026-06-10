{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {

      devShells.x86_64-linux.default = pkgs.mkShell {
        packages = with pkgs.python313Packages; [
          pandas
          numpy
          ipykernel
          matplotlib
          pkgs.gcc
          pkgs.ccls
          pkgs.gmp
        ];
      };

    };
}
