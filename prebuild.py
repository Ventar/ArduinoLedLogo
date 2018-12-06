Import("env")

build_tag = "v0.0.1"

env.Replace(PROGNAME="ledlogo_%s" % build_tag)