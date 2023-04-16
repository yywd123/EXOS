#!/usr/bin/python

import os, time, argparse

argparser = argparse.ArgumentParser(description="EXOS构建脚本")
argparser.add_argument("--arch", type=str, help="目标机器架构", default="x86_64")
args = argparser.parse_args();
selectedArch = ""

with open(".build_src", mode="w", encoding="utf-8") as srclist:
  supportArchs = ["x86_64"]
  if (supportArchs.__contains__(args.arch)):
    selectedArch = args.arch;
  else:
    print("不支持的目标架构: " + args.arch)
    exit(-1)
  srcdirs = []
  for srcdir in ["kernel", "driver", "arch/" + selectedArch]:
    for path, dirs, src in os.walk("src/" + str(srcdir)):
      srcdirs.append(path)

  suffixes = [".c", ".cpp", ".S"]
  rules = ["c", "cpp", "as"]
  targetList = ""
  for srcdir in srcdirs:
    for srcName in os.listdir(srcdir):
      nameparts = os.path.splitext(srcName)
      if (os.path.isfile(srcdir + "/" + srcName) and nameparts[1] in suffixes):
        target = nameparts[1].replace(".", "") + "_" + nameparts[0] + "_{" + str(time.time_ns()) + "}.o"
        rule = rules[suffixes.index(nameparts[1])]
        srclist.write("build " + target + ": " + rule + " " + srcdir + "/" + srcName + "\n")
        targetList += " " + target

  srclist.write("\nbuild kernel.sys: link" + targetList + "\n")
  
with open("include/archinfo.h", mode="w", encoding="utf-8") as archinfo:
  archinfo.write("#pragma once\n\n")
  archinfo.write("#define TARGET_ARCH " + selectedArch + "\n")
os.system("rm build/objs/*")
if (os.system("ninja") != 0):
  exit(-1)
