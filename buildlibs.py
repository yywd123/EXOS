#!/usr/bin/python

import os, argparse

argparser = argparse.ArgumentParser(description="EXOS库文件构建脚本")
argparser.add_argument("--arch", type=str, help="目标机器架构", default="x86_64")
args = argparser.parse_args();
supportArchs = [
  "x86_64", 
  "aarch64"
]

selectedArch = ""

if args.arch in supportArchs:
  selectedArch = args.arch
else:
  print("无效的目标架构: " + args.arch)
  exit(-1)

#gnu-efi
os.system("export CROSS_COMPILE=" + selectedArch + "-linux-gnu-;make -s -C libs/gnu-efi")