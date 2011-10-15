#Copyright (C) 2011 by Francois Coulombe

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 

import os
import os.path
import atexit
import sys
import platform
import subprocess
print platform.machine()
print platform.architecture()


AddOption('--use-valgrind', action="store_true", 
          dest="useValgrind",
          default=False,
          help='run the unit tests with valgrind')

AddOption('--gen-valgrind-suppressions', action="store_true", 
          dest="genValgrindSuppressions",
          default=False,
          help='generate valgrind suppression files')

AddOption('--compiler', action="store",
          type='string', 
          dest="compiler",
          default='g++',
          help='specify the compiler')




def gitAdd(fileName, repoDir):
    cmd = 'git add ' + fileName
    pipe = subprocess.Popen(cmd, shell=True, cwd=repoDir)
    pipe.wait()
    return


def gitClone(repo, dest="."):
    cmd = 'git clone ' + repo + " " + dest
    pipe = subprocess.Popen(cmd, shell=True)
    pipe.wait()
    return
targetList = COMMAND_LINE_TARGETS

default_env = Environment()
default_env.SConsignFile(default_env.File("#build/.sconsign.dblite").abspath) 
#detect if we have the build script installed (hopefully its gonna be the right one)
if not os.path.exists(default_env.Dir("#GCLBuildScript").abspath):
    #git clone the build script
    print "The buildscript doesn't seem to be installed. I will fetch it for you from github"
    
    gitClone("git@github.com:fcoulombe/GCLBuildScript.git", "./GCLBuildScript")
    os.symlink("GCLBuildScript/site_scons", "site_scons")
    
if not os.path.exists(default_env.Dir("#lib/GCL").abspath):
    #git clone the build script
    print "The cgl lib doesn't seem to be installed. I will fetch it for you from github"
    
    gitClone("https://fcoulombe@github.com/fcoulombe/GCL.git", "./lib/GCL")



##############################################
#
#    SELECT COMPILER
#
##############################################
compiler = GetOption('compiler')
"""if compiler == 'g++':
    default_env['CXX'] = 'g++'
    default_env['CC'] = 'gcc'
elif compiler == 'clang':"""
print compiler
default_env['CXX'] = 'clang++'
default_env['CC'] = 'clang'
default_env['CXX'] = 'scan-build -k'
default_env['CC'] = 'scan-build -k'
cflags = ["-pedantic", "-std=c++0x", '-fexceptions', '-ftrapv', '-fvisibility=hidden']
default_env.AppendUnique(CFLAGS=cflags)
default_env.AppendUnique(CXXFLAGS=cflags)
print default_env['CC']
print default_env['CXX']  
    
    
##############################################
##############################################


default_env.Tool('Profiler', toolpath=['site_scons/site_tools'])
default_env.Tool('Catalog', toolpath=['site_scons/site_tools'])
default_env.Tool('SConsWalk', toolpath=['site_scons/site_tools'])
default_env.Tool('Wrappers', toolpath=['site_scons/site_tools'])

default_env['ENV']['PATH'] = '/Developer/usr/bin:' + default_env['ENV']['PATH'] + ':/opt/local/bin:/opt/local/sbin' 
default_env['ENV']['PKG_CONFIG_PATH'] = '/usr/local/lib/pkgconfig'
print default_env['ENV']['PATH']


cflags = ["-O0", "-fPIC", "-g", "-Wall", "-Werror", "-Wextra"]                                    
default_env.Append(CPPFLAGS=cflags )

sconsFilesList = [
"./3rdParty/IL/SConscript",
"./3rdParty/libfreenect/SConscript",
"./3rdParty/libpng/SConscript",
"./3rdParty/OpenCV/SConscript",
"./3rdParty/OpenGL/SConscript",
"./3rdParty/SDL/SConscript",

"./lib/GCL/GCL/SConscript",
"./lib/GCL/GCL/unittest/SConscript",
"./lib/Input/SConscript",
"./lib/Input/unittest/SConscript",
"./lib/Kinect/SConscript",
"./lib/Kinect/unittest/SConscript",
"./lib/Renderer/SConscript",
"./lib/Renderer/unittest/SConscript",
"./lib/Voxel/SConscript",
"./lib/Voxel/unittest/SConscript",
"./lib/AppLayer/SConscript",
"./lib/AppLayer/unittest/SConscript",
"./Program/Example/BasicTextureMapping/SConscript",
"./Program/Kinevox/SConscript",
]
if GetOption('genValgrindSuppressions'):
    sconsFileList.append("./tools/valgrindgen/SConscript") #this tool generates some rules for valgrind so that it ignores certain pattern of memory errors that we don't care about

default_env.StampTime("start SConscript Parse...")
default_env.SConsWalkList(sconsFilesList, './SConscript')
default_env.StampTime("end sconscript parse...")
if "@aliases" in targetList:
    default_env.displayAliases()
    sys.exit()

def OnExit():
    default_env.StampTime("exit")

atexit.register(OnExit)
