{ latexrun, texlive, nix-gitignore, python3, stdenvNoCC }:

let
  texdeps = texlive.combine {
    inherit (texlive)
      scheme-basic algorithms blindtext booktabs cleveref doublestroke etoolbox
      float hyphenat l3packages lineno listings mdframed metafont multirow
      needspace pgf pstricks soul type1cm xcolor xkeyval zref;
  };

in stdenvNoCC.mkDerivation {
  name = "paper.pdf";
  src = nix-gitignore.gitignoreSource [ ] ../paper;
  buildInputs = [ python3 latexrun texdeps ];
  buildPhase = ''
    export HOME=$PWD/.home
    python gen.py paper.tex
    latexrun paper.tex
  '';

  installPhase = ''
    mv paper.pdf $out
  '';
}
