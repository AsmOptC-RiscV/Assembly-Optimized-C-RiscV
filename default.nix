let
  nixpkgs = import <nixpkgs>;

  pkgs = nixpkgs { overlays = [ (import ./nix) ]; };

  inherit (pkgs.nix-gitignore) gitignoreSource;

  crossSystem =
    pkgs.lib.recursiveUpdate pkgs.lib.systems.examples.riscv32-embedded {
      platform.gcc.arch = "rv32imac";
    };

  riscvPkgs = nixpkgs { inherit crossSystem; };

in with pkgs;

rec {
  inherit pkgs riscvPkgs;

  shell = riscvPkgs.mkShell {
    nativeBuildInputs = [ riscvPkgs.buildPackages.gdb openocd verilator ];
    hardeningDisable = [ "all" ];
    PREFIX = riscvPkgs.hostPlatform.config;
  };

  paper = callPackage nix/paper.nix { };
  newlib-nano = riscvPkgs.callPackage nix/newlib-nano.nix { };

  schemes =
    [ "AES-CTR-Bitsliced" "AES-LUT" "Gimli" "Keccak" "Keccak-v2" "saturnin" ];
  platforms = [ "sifive" "murax" ];

  gccMurax = scheme:
    riscvPkgs.stdenv.mkDerivation {
      name = "${scheme}-gcc-murax.elf";
      src = gitignoreSource [ "*.nix" ] ./.;
      nativeBuildInputs = [ python3 ];
      PREFIX = riscvPkgs.hostPlatform.config;
      ECHO = "echo -e";
      configurePhase = ''
        patchShebangs .
        ./configure.py ${scheme} gcc murax
      '';

      postBuild = ''
        echo -en '\033[0m'
      '';

      installPhase = ''
        mv main $out
      '';
    };

  gccSifive = scheme:
    riscvPkgs.stdenv.mkDerivation {
      name = "${scheme}-gcc-sifive.elf";
      src = gitignoreSource [ "*.nix" ] ./.;
      nativeBuildInputs = [ python3 ];
      PREFIX = riscvPkgs.hostPlatform.config;
      ECHO = "echo -e";
      configurePhase = ''
        patchShebangs .
        ./configure.py ${scheme} gcc sifive
      '';

      postBuild = ''
        echo -en '\033[0m'
      '';

      installPhase = ''
        mv main.elf $out
      '';
    };

  riscv-ovpsim-bin = runCommand "riscv-ovpsim-bin" {
    src = fetchFromGitHub {
      owner = "riscv";
      repo = "riscv-ovpsim";
      rev = "5eed1ab3fbaaeed2e105d4b13e4afd56c75570fc";
      sha256 = "1fx0p1c72svd1n0d0ncakmksfa7kl39yv61wgdl8y085naqaf6k3";
    };
  } ''
    cp -r $src $out
    mkdir -p $out/bin
    chmod -R u+w $out
    patchelf --set-interpreter ${glibc}/lib/ld-linux-x86-64.so.2 $out/bin/Linux64/riscvOVPsim.exe
  '';

  gccBuild = platform: target: scheme:
    riscvPkgs.stdenv.mkDerivation {
      name = "${scheme}-gcc-${platform}.elf";
      src = gitignoreSource [ "*.nix" "riscvOVPsim" ] ./.;
      nativeBuildInputs = [ python3 ];
      PREFIX = riscvPkgs.hostPlatform.config;
      ECHO = "echo -e";
      configurePhase = ''
        patchShebangs .
        ./configure.py ${scheme} gcc ${platform}
      '';

      postBuild = ''
        echo -en '\033[0m'
      '';

      installPhase = ''
        mv ${target} $out
      '';
    };

  gcc-ovpsim = gccBuild "riscvOVPsim" "mainOVP.elf";
  run-gcc-ovpsim = scheme:
    runCommand "${scheme}-gcc-ovpsim.out" { } ''
      cd ${riscv-ovpsim-bin}/bin/Linux64
      ./riscvOVPsim.exe --program ${gcc-ovpsim scheme} --variant RV32IMC | tee $out
    '';

  ovpsim.gcc.Ascon-128 = run-gcc-ovpsim "Ascon-128";
  ovpsim.gcc.Ascon-128a = run-gcc-ovpsim "Ascon-128a";
  ovpsim.gcc.Gimli = run-gcc-ovpsim "Gimli";
  ovpsim.gcc.Keccak = run-gcc-ovpsim "Keccak";
  ovpsim.gcc.Keccak-200 = run-gcc-ovpsim "Keccak-200";
  ovpsim.gcc.Keccak-v2 = run-gcc-ovpsim "Keccak-v2";
  ovpsim.gcc.AES-LUT = run-gcc-ovpsim "AES-LUT";
  ovpsim.gcc.AES-CTR-Bitsliced = run-gcc-ovpsim "AES-CTR-Bitsliced";
}
