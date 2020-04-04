Import("env")
from os.path import basename, isdir, join

build_flags = env.ParseFlags(env['BUILD_FLAGS'])
#print build_flags.get("CPPDEFINES")
#flags = {k: v for (k, v) in build_flags.get("CPPDEFINES")}
#print flags
#filename = flags.get("HARDWARE") + "." + flags.get("SOFTWARE_VERSION")

#alternate example
#build_tag = "32v1"
#env.Replace(PROGNAME="mkstft%s" % build_tag)

#build using chosen build enviroment filename
filename = env['PIOENV']
env.Replace(PROGNAME=filename)


# Dump construction environment (for debug purpose)
#print(env)
#print(env.Dump())