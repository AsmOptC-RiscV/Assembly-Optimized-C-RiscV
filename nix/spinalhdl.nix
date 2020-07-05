{ fetchMavenArtifact }:

let
  commons-io = fetchMavenArtifact {
    groupId = "commons-io";
    artifactId = "commons-io";
    version = "2.4";
    sha256 = "108mw2v8ncig29kjvzh8wi76plr01f4x5l3b1929xk5a7vf42snc";
  };

  snakeyaml = fetchMavenArtifact {
    groupId = "org.yaml";
    artifactId = "snakeyaml";
    version = "1.8";
    sha256 = "1z0ybg8azqanrhqjbr57n6sflm7scfxik99j070f9zk7g6ykcl9g";
  };

  scopt = fetchMavenArtifact {
    groupId = "com.github.scopt";
    artifactId = "scopt_2.11";
    version = "3.4.0";
    sha256 = "0y9av2rpnaj3z9zg4chfpxgjx3xqqx9bzcv0jr3n9h4v5fqbc52r";
  };

  spinalhdl-core = fetchMavenArtifact {
    groupId = "com.github.spinalhdl";
    artifactId = "spinalhdl-core_2.11";
    version = "1.3.8";
    sha256 = "13lywy7xv3naplq556hx6iggrnkb3f4dnsdql2vi0l6640racpb0";
  };

  spinalhdl-lib = fetchMavenArtifact {
    groupId = "com.github.spinalhdl";
    artifactId = "spinalhdl-lib_2.11";
    version = "1.3.8";
    sha256 = "0ckbpr73g24bp0pcyj449cvd21z720yn5zc21k9ci56rw5349gy7";
  };

  spinalhdl-sim = fetchMavenArtifact {
    groupId = "com.github.spinalhdl";
    artifactId = "spinalhdl-sim_2.11";
    version = "1.3.8";
    sha256 = "07zn6rz8lbakg2q431aqpk70xrm40c6in6q3apnshj4ss9kj62vx";
  };

in [ commons-io scopt snakeyaml spinalhdl-core spinalhdl-lib spinalhdl-sim ]
