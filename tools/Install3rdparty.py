
import os
import sys
import shutil

def RemoveDir(top):
	for root, dirs, files in os.walk(top, topdown=False):
		for name in files:
			os.remove(os.path.join(root, name))
		for name in dirs:
			os.rmdir(os.path.join(root, name))
	os.rmdir(top)

def CreateRepo(root):
	if os.path.exists(root+"tmp"):
		RemoveDir(root+"tmp")
	os.mkdir(root+"tmp")
	os.system("git clone -l C:\\src\\3rdparty "+root+"tmp")


def OverwriteIfExists(src, dst, component):
	srcComp = src+component
	dstComp = dst+component
	if not os.path.exists(srcComp):
		return
	if os.path.exists(dstComp):
		RemoveDir(dstComp)
	shutil.copytree(srcComp, dstComp)

def Copy3rdParty(root):
	for d in os.listdir(root+"tmp"):
		if d == ".git":
			continue
		src = root+"tmp/"+d+"/"
		dst = root+"3rdparty/"+d+"/"
		OverwriteIfExists(src, dst, "include")
		OverwriteIfExists(src, dst, "lib")
		OverwriteIfExists(src, dst, "bin")



CreateRepo("kinevox/")
Copy3rdParty("kinevox/")
RemoveDir("kinevox/tmp")
