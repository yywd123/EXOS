#!/usr/bin/python

import os, sys, time, subprocess

if os.system("git submodule update --init") != 0:
  print("更新依赖库失败")
  exit(-1)
subprocess.Popen("make -q -C gnu-efi", shell=True).wait()

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

  srclist.write("\nbuild bootx64.so: link" + targetList + "\n")
os.system("rm build/obj/*")
if (os.system("ninja") != 0):
  sys.exit(-1)