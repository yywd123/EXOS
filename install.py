#!/usr/bin/python

import os, argparse

supportArchs = [
  "x86_64", 
  #"aarch64"
]

rootdir = ""
selectedArch = ""

rebuildFlag = False;

def mkdir(dirName : str) :
  os.system("mkdir -p " + rootdir + dirName)
  print("创建了文件夹[" + rootdir + dirName + "]")

def cp(src : str, dest : str) :
  ret = os.system("cp -f " + src + " " + rootdir + dest)
  if (ret != 0) :
    print("复制系统文件[" + rootdir + dest + "]失败,返回值[{0}]".format(ret))
    exit();
  print("复制系统文件[" + rootdir + dest + "]完成")

dependcies = [
  "build/kernel.sys", 
  "boot/build/boot.efi",
  ]

dependciesDestinations = [
  "/EXOS/kernel.sys",
  "placeholder__BOOTLOADER",
  ]

def checkDependcies() -> bool :
  if rebuildFlag == True:
    return False
  print("检查依赖中...")
  for depFile in dependcies:
    if (not os.path.isfile(depFile)) :
      return False

  return True

if (len(dependcies) != len(dependciesDestinations)) :
  print("安装脚本依赖列表与目的地列表不对等!请检查依赖列表与目的地列表")

argparser = argparse.ArgumentParser(description="EXOS安装脚本")
argparser.add_argument("path", type=str, help="安装目录")
argparser.add_argument("--rebuild", action="store_true", help="强制重新构建", default=False)
argparser.add_argument("--arch", type=str, help="目标机器架构", default="x86_64")
args = argparser.parse_args()

rebuildFlag = args.rebuild
rootdir = args.path
selectedArch = args.arch

if selectedArch not in supportArchs:
  print("不支持的架构: " + selectedArch)
  exit(-1)

dependciesDestinations[dependciesDestinations.index("placeholder__BOOTLOADER")] = "/efi/boot/boot" + ["x64", "aa64"][supportArchs.index(selectedArch)] + ".efi"

if (not os.access(rootdir, os.W_OK))  :
  os.system("mkdir -p " + rootdir)
  if (not os.access(rootdir, os.W_OK))  :
    print("没有目标文件夹的写入权限!请尝试使用sudo再试一次")
    exit()

if (not checkDependcies()):
  if (os.system("cd boot;python ./build.py --arch " + selectedArch) != 0) :
    print("构建引导出错!!!")
    exit()
  print("引导构建完成")
  if (os.system("python ./build.py --arch " + selectedArch) != 0) :
    print("构建内核出错!!!")
    exit()
  print("内核构建完成")

mkdir("/EXOS")
mkdir("/efi/boot/")
mkdir("/efi/EXOS/")

for i in range(0, len(dependciesDestinations))  :
  cp(dependcies[i], dependciesDestinations[i])

print("安装完成")
