{ stdenv }:

stdenv.cc.libc.overrideAttrs (old: {
  CFLAGS =
    [ "-Os" "-fdata-sections" "-ffunction-sections" "-fshort-wchar" "-g" ];

  configureFlags = [
    "--disable-newlib-fseek-optimization"
    "--disable-newlib-fvwrite-in-streamio"
    "--disable-newlib-supplied-syscalls"
    "--disable-newlib-unbuf-stream-opt"
    "--disable-newlib-wide-orient"
    "--disable-nls"
    "--enable-lite-exit"
    "--enable-newlib-global-atexit"
    "--enable-newlib-nano-formatted-io"
    "--enable-newlib-nano-malloc"
    "--enable-newlib-reent-small"
  ];

  postInstall = ''
    for file in $out/${stdenv.hostPlatform.config}/lib/*.a; do
      ln -s $file ''${file%.a}_nano.a
    done
  '';
})
