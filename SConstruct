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

AddOption('--print-component-dependencies', action="store_true", 
          dest="print-component-dependencies",
          default=False,
          help='prints the component dependnecies for the programs')


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
if not os.path.exists(default_env.Dir("#gclbuildscript").abspath):
    #git clone the build script
    print "The buildscript doesn't seem to be installed. I will fetch it for you from github"
    
    gitClone("git@github.com:fcoulombe/gclbuildscript.git", "./gclbuildscript")
    os.symlink("gclbuildscript/site_scons", "site_scons")
    
if not os.path.exists(default_env.Dir("#lib/gcl").abspath):
    #git clone the build script
    print "The cgl lib doesn't seem to be installed. I will fetch it for you from github"
    
    gitClone("https://fcoulombe@github.com/fcoulombe/gcl.git", "./lib/gcl")



##############################################
#
#    SELECT COMPILER
#
##############################################
compiler = GetOption('compiler')

cflags = ["-O0", "-g", "-Wall", "-Werror", "-Wextra", '-fexceptions', '-ftrapv', '-fvisibility=hidden']   
if default_env['PLATFORM']=='darwin':
    cflags.append("-DOS_MACOSX")
else:
    cflags.append("-DOS_LINUX")                  

if compiler == 'g++':
    default_env['CXX'] = 'g++'
    default_env['CC'] = 'gcc'
    #default_env['CXX'] = 'scan-build g++'
    #default_env['CC'] = 'scan-build gcc'
elif compiler == 'clang':
    default_env['CXX'] = '/Developer//usr/bin/clang++'
    default_env['CC'] = '/Developer//usr/bin/clang'
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
"./3rdParty/il/SConscript",
"./3rdParty/libfreenect/SConscript",
"./3rdParty/libpng/SConscript",
"./3rdParty/opencv/SConscript",
"./3rdParty/opengl/SConscript",
"./3rdParty/glew/SConscript",
"./3rdParty/sdl/SConscript",

"./lib/gcl/gcl/SConscript",
"./lib/gcl/gcl/unittest/SConscript",
"./lib/input/SConscript",
"./lib/kinect/SConscript",
"./lib/kinect/unittest/SConscript",
"./lib/renderer/SConscript",
"./lib/renderer/unittest/SConscript",
"./lib/input/unittest/SConscript", #depends on renderer
"./lib/voxel/SConscript",
"./lib/voxel/unittest/SConscript",
"./lib/applayer/SConscript",
"./lib/applayer/unittest/SConscript",
"./program/example/example_data/SConscript",
"./program/example/basicTextureMapping/SConscript",
"./program/kinevox/SConscript",
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
