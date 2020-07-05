{ lib, stdenvNoCC, fetchurl, runCommand, fetchFromGitHub, makeWrapper
, spinalhdl, scala_2_11, fetchMavenArtifact }:

stdenvNoCC.mkDerivation {
  name = "vexriscv";
  src = fetchFromGitHub {
    rev = "ee36c36fddf84e81a48957e8b4963c2764b27b62";
    owner = "SpinalHDL";
    repo = "VexRiscv";
    sha256 = "02cnvz3xrdairll2pfpscdnprw9ya3a17z8bky9d37kk4axl7bmc";
  };

  buildInputs = spinalhdl;
  nativeBuildInputs = [ scala_2_11 ];
  buildPhase = ''
    scalac -d vexriscv.jar $(find src/main/scala -type f)
  '';

  installPhase = ''
    mkdir -p $out/share/java
    mv vexriscv.jar $out/share/java
  '';
}
