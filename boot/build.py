#!/usr/bin/python

import os, argparse, time

argparser = argparse.ArgumentParser(description="EXOS引导构建脚本")
argparser.add_argument("--arch", type=str, help="目标机器架构", default="x86_64")
args = argparser.parse_args();
supportArchs = [
  "x86_64", 
  "aarch64"
]

selectedArch = ""
MULTIARCH_CCFLAGS = [
  "-mno-red-zone -maccumulate-outgoing-args", #x86_64
  "", #aarch64
]
MULTIARCH_LDFLAGS = [
  "", #x86_64
  "", #aarch64
]

if (supportArchs.__contains__(args.arch)):
    selectedArch = args.arch;
else:
  print("不支持的目标架构: " + args.arch)
  exit(-1)

# if os.system("git submodule update --init") != 0:
#   print("更新依赖库失败")
#   exit(-1)
os.system("export CROSS_COMPILE=" + selectedArch + "-linux-gnu-;make -s -C ../libs/gnu-efi")

#生成构建脚本
with open(".build_src", mode="w", encoding="utf-8") as srclist:
  srcdirs = ["."]

  suffixes = [".c"]
  rules = ["c"]
  targetList = ""
  for srcdir in srcdirs:
    for srcName in os.listdir(srcdir):
      nameparts = os.path.splitext(srcName)
      if (os.path.isfile(srcdir + "/" + srcName) and nameparts[1] in suffixes):
        target = nameparts[1].replace(".", "") + "_" + nameparts[0] + "_{" + str(time.time_ns()) + "}.o"
        rule = rules[suffixes.index(nameparts[1])]
        srclist.write("build " + target + ": " + rule + " " + srcdir + "/" + srcName + "\n")
        targetList += " " + target

  srclist.write("\nbuild boot.so: link" + targetList + "\n")
with open(".toolchain", mode="w", encoding="utf-8") as toolchainInfo:
  toolchainInfo.write("ARCHNAME = " + selectedArch + "\n")
  toolchainInfo.write("MULTIARCH_CCFLAG = " + MULTIARCH_CCFLAGS[supportArchs.index(selectedArch)] + "\n")
  toolchainInfo.write("MULTIARCH_LDFLAG = " + MULTIARCH_LDFLAGS[supportArchs.index(selectedArch)] + "\n")
  toolchainInfo.write("CC = " + selectedArch + "-linux-gnu-gcc\n")
  toolchainInfo.write("CPP = " + selectedArch + "-linux-gnu-g++\n")
  toolchainInfo.write("LD = " + selectedArch + "-linux-gnu-ld\n")
  toolchainInfo.write("OBJCOPY = " + selectedArch + "-linux-gnu-objcopy\n")
os.system("rm build/obj/*")
if (os.system("ninja") != 0):
  exit(-1)