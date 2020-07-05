self: super: rec {
  spinalhdl = self.callPackage ./spinalhdl.nix { };
  vexriscv = self.callPackage ./vexriscv.nix { };
  pqvexriscv = self.callPackage ./pqvexriscv.nix { };

  latexrun = super.latexrun.overrideAttrs (old: {
    patches = [
      (self.fetchurl {
        url =
          "https://github.com/aslushnikov/latexrun/commit/566809a5b8feecc16a13033f3036b6a3c84e8ed7.patch";
        sha256 = "00ygj0yddrv54h1m8j2mxp9qlqwyjyc294iwplk65kpqkc01gl72";
      })
    ];
  });

  scala_2_11 = super.scala_2_11.overrideAttrs (old: {
    installPhase = ''
      mkdir -p $out
      rm "bin/"*.bat
      mv * $out

      # put docs in correct subdirectory
      mkdir -p $out/share/doc
      mv $out/doc $out/share/doc/${old.name}
      mv $out/man $out/share/man

      for p in $(ls $out/bin/) ; do
        wrapProgram $out/bin/$p \
          --prefix PATH ":" ${self.coreutils}/bin \
          --prefix PATH ":" ${self.gnugrep}/bin \
          --prefix PATH ":" ${self.jdk}/bin \
          --set JAVA_HOME ${self.jdk}/lib/openjdk
      done
    '';
  });
}
