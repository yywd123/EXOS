#!/usr/bin/python

import os
import argparse

argparser = argparse.ArgumentParser(description="EXOS构建脚本")
argparser.add_argument("--force-rebuild", action="store_true",
                       help="忽略记录的时间戳 将所有源文件重新编译", default=False)
argparser.add_argument("--release", action="store_true",
                       help="使用正式模式构建", default=False)
args = argparser.parse_args()

ts_files = []
ts_ts = []
isForceRebuild = args.force_rebuild

isRelease = args.release

print("EXOS构建系统\n")

if isForceRebuild:
    os.system("rm build/obj/*")

with open(".srcts", mode="r", encoding="utf-8") as srcTimestamp:
    print("读取源文件时间戳...")
    timestamps = srcTimestamp.readlines()
    for index in range(0, len(timestamps)):
        ts_files.append(timestamps[index].split(" ")[0])
        ts_ts.append(int(timestamps[index].split(" ")[1]))

with open(".build_src", mode="w", encoding="utf-8") as srclist:
    print("生成构建列表...")
    srcdirs = []
    for path, dirs, src in os.walk("src/"):
        srcdirs.append(path)

    suffixes = [".c", ".cpp", ".S"]
    rules = ["c", "cpp", "as"]
    targetList = ""
    for srcdir in srcdirs:
        for srcName in os.listdir(srcdir):
            nameparts = os.path.splitext(srcName)
            srcPath = srcdir + "/" + srcName
            if (os.path.isfile(srcPath) and nameparts[1] in suffixes):
                if (ts_files.__contains__(srcPath)):
                    index = ts_files.index(srcPath)
                    if (not isForceRebuild and ts_ts[index] == int(os.stat(srcPath).st_mtime)):
                        continue
                    else:
                        ts_ts[index] = int(os.stat(srcPath).st_mtime)
                else:
                    ts_files.append(srcPath)
                    ts_ts.append(int(os.stat(srcPath).st_mtime))

                target = str(srcdir).replace(
                    "/", ".") + "." + srcName + ".o"
                rule = rules[suffixes.index(nameparts[1])]
                srclist.write("build " + target + ": " + rule +
                              " " + srcPath + "\n")
                targetList += " " + target
    srclist.write("\nbuild kernel.so: link" + targetList + "\n")

with open(".toolchain", mode="w", encoding="utf-8") as toolchainInfo:
    print("生成工具链信息...")
    toolchainInfo.write("AS = x86_64-linux-gnu-as\n")
    toolchainInfo.write("CC = x86_64-linux-gnu-gcc\n")
    toolchainInfo.write("CPP = x86_64-linux-gnu-g++\n")
    toolchainInfo.write("LD = x86_64-linux-gnu-ld\n")
    toolchainInfo.write("OBJCOPY = x86_64-linux-gnu-objcopy\n")
    if isRelease:
        toolchainInfo.write("BUILDFLAG = -DRELEASE\n")
if (os.system("ninja") != 0):
    exit(-1)
print("构建完成")

with open(".srcts", mode="w", encoding="utf-8") as srcTimestamp:
    print("保存源文件时间戳...")
    for index in range(0, len(ts_files)):
        srcTimestamp.write(ts_files[index] + " " + str(ts_ts[index]) + "\n")
