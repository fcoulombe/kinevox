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


AddOption('--verbose', action="store_true", 
          dest="verbose",
          default=False,
          help='make the build verbose')

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

cflags = ["-O0", "-g", "-Wall", "-Werror", "-Wextra", '-fexceptions', '-ftrapv', '-fvisibility=hidden']            

if compiler == 'g++':
    default_env['CXX'] = 'g++'
    default_env['CC'] = 'gcc'
    #default_env['CXX'] = 'scan-build g++'
    #default_env['CC'] = 'scan-build gcc'
elif compiler == 'clang':
    default_env['CXX'] = 'clang++'
    default_env['CC'] = 'clang'
    cflags.append("-std=c++0x");
    cflags.append("-pedantic")

#default_env['CXX'] = 'scan-build -k'
#default_env['CC'] = 'scan-build -k'
                        
default_env.AppendUnique(CPPFLAGS=cflags )
default_env.AppendUnique(CFLAGS=cflags)
default_env.AppendUnique(CXXFLAGS=cflags)
variant ="%s-%s-%s" % (default_env['CC'], platform.machine(),  platform.architecture()[0]) 
print variant

if not GetOption('verbose'):
    default_env['RANLIBCOMSTR'] = "[ranlib] $TARGET" 
    default_env['ARCOMSTR'] = "[ar] $TARGET"
    default_env['CCCOMSTR'] = "[cc] $SOURCE"
    default_env['CXXCOMSTR'] = "[cxx] $SOURCE"
    default_env['LINKCOMSTR'] = "[link] $TARGET"
    default_env['INSTALLSTR'] = '[install] $TARGET'

default_env['INSTALL_DIR'] = '#/bin/' + variant 

    
##############################################
##############################################


default_env.Tool('Profiler', toolpath=['site_scons/site_tools'])
default_env.Tool('Catalog', toolpath=['site_scons/site_tools'])
default_env.Tool('SConsWalk', toolpath=['site_scons/site_tools'])
default_env.Tool('Wrappers', toolpath=['site_scons/site_tools'])
default_env.Tool('RSync', toolpath=['site_scons/site_tools'])
default_env.Tool('UnitTest', toolpath=['site_scons/site_tools'])


default_env['ENV']['PATH'] = '/usr/local/bin:/opt/local/bin:/opt/local/sbin:/usr/bin:/bin:/Developer/usr/bin' 
default_env['ENV']['PKG_CONFIG_PATH'] = '/usr/local/lib/pkgconfig'
print default_env['ENV']['PATH']



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
"./lib/Kinect/SConscript",
"./lib/Kinect/unittest/SConscript",
"./lib/Renderer/SConscript",
"./lib/Renderer/unittest/SConscript",
"./lib/Input/unittest/SConscript", #depends on renderer
"./lib/Voxel/SConscript",
"./lib/Voxel/unittest/SConscript",
"./lib/AppLayer/SConscript",
"./lib/AppLayer/unittest/SConscript",
"./Program/Example/BasicTextureMapping/SConscript",
"./Program/Kinevox/SConscript",
]
if GetOption('genValgrindSuppressions'):
    sconsFileList.append("./tools/valgrindgen/SConscript") #this tool generates some rules for valgrind so that it ignores certain pattern of memory errors that we don't care about

#default_env.StampTime("start SConscript Parse...")
default_env.SConsWalkList(sconsFilesList, './SConscript', variant)
default_env.StampTime("Parsing Time")
if "@aliases" in targetList:
    default_env.displayAliases()
    sys.exit()

def OnExit():
    default_env.StampTime("exit")

atexit.register(OnExit)
