import subprocess
Import("env")

my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}

env.Replace(PROGNAME="%s-%s-%s-%s-%s" % (
    defines.get("PIO_SRC_NAM"),
    str(env["BOARD"]),
    defines.get("PIO_SRC_TAG"),
    defines.get("PIO_SRC_REV"),
    defines.get("PIO_SRC_BRH")))