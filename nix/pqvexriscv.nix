{ stdenvNoCC, openjdk, fetchFromGitHub, spinalhdl, vexriscv, scala_2_11, makeWrapper }:

stdenvNoCC.mkDerivation {
  name = "pqvexriscv";
  src = fetchFromGitHub {
    rev = "bbc4e1af4c912d2d1a7f63dffe760772cb2ebb57";
    owner = "mupq";
    repo = "pqriscv-vexriscv";
    sha256 = "15171zhh549llsyj8wlhj5d6q46zg5savnbd1xg16n6zaz3pcqz5";
  };

  buildInputs = spinalhdl ++ [ vexriscv ];
  nativeBuildInputs = [ makeWrapper scala_2_11 ];
  buildPhase = ''
    scalac -d pqvexriscv.jar $(find src/main/scala -type f)
  '';

  installPhase = ''
    mkdir -p $out/share/java $out/bin
    mv pqvexriscv.jar $out/share/java
    makeWrapper ${scala_2_11}/bin/scala $out/bin/pqvexriscvsim \
      --add-flags "-cp $CLASSPATH:$out/share/java/pqvexriscv.jar mupq.PQVexRiscvSim"
  '';
}
