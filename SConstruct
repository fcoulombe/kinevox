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

default_env = Environment()
#################################################################
#
#   adding custom commandline argument
#
#################################################################
AddOption('--verbose', action="store_true", 
          dest="verbose",
          default=False,
          help='make the build verbose')

AddOption('--use-valgrind', action="store_true", 
          dest="useValgrind",
          default=False,
          help='run the unit tests with valgrind')

AddOption('--interactive-test', action="store_true", 
          dest="isInteractiveTest",
          default=False,
          help='run the unit tests in interactive mode. this is for GL especially, press space to go to the next test')

AddOption('--print-component-dependencies', action="store_true", 
          dest="print-component-dependencies",
          default=False,
          help='prints the component dependnecies for the programs')


AddOption('--gen-valgrind-suppressions', action="store_true", 
          dest="genValgrindSuppressions",
          default=False,
          help='generate valgrind suppression files')

if default_env['PLATFORM']=='win32':
    AddOption('--compiler', action="store",
              type='string', 
              dest="compiler",
              default='vc',
              help='specify the compiler')
else:
    AddOption('--compiler', action="store",
              type='string', 
              dest="compiler",
              default='gcc',
              help='specify the compiler')

AddOption('--configuration', action="store",
          type='string', 
          dest="configuration",
          default='debug',
          help='specify whether we are compiling in (debug, opt) mode')


########################################################################
#
#   fetching some other packages from github if they don't seem to be there
#   usually on used on very first build
#
########################################################################

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


default_env.SConsignFile(default_env.File("#build/.sconsign.dblite").abspath) 
#detect if we have the build script installed (hopefully its gonna be the right one)
if not os.path.exists(default_env.Dir("#gclbuildscript").abspath):
    #git clone the build script
    print "The buildscript doesn't seem to be installed. I will fetch it for you from github"
    
    gitClone("git@github.com:fcoulombe/gclbuildscript.git", "./gclbuildscript")
    if default_env['PLATFORM'] == "win32":
	    print "making link!"
	    import ctypes
	    kdll = ctypes.windll.LoadLibrary("kernel32.dll")
	    kdll.CreateSymbolicLinkA("gclbuildscript/site_scons", "site_scons", 0)
    else:
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

lflags = [] 
cflags = [ ]
if default_env['PLATFORM']=='darwin':
    cflags.append("-DOS_MACOSX")
elif default_env['PLATFORM']=='win32':
    cflags.append("-DOS_WIN32")
else:
    cflags.append("-DOS_LINUX") 
                     
if platform.architecture()[0] == "32bit":
    cflags.append('-DUSE_64BIT_PLATFORM=0')
elif platform.architecture()[0] == "64bit":
    cflags.append('-DUSE_64BIT_PLATFORM=1')
else:
    print "unsupported architecture: %s" % platform.architecture()[0] 

configuration = GetOption('configuration')
if compiler == 'gcc':
    extracflags = [ "-g", "-Wall",  '-fexceptions', '-ftrapv', '-DFBXSDK_NEW_API'] #, '-fvisibility=hidden']   
    cflags.extend(extracflags)
    cflags.append("-Werror")
    cflags.append("-Wextra")
    default_env['CXX'] = 'g++'
    default_env['CC'] = 'gcc'
    #default_env['CXX'] = 'scan-build g++'
    #default_env['CC'] = 'scan-build gcc'
    cflags.append("-std=c++0x");
    #cflags.append("-pedantic")
    lflags.append("-L/usr/lib/")
    lflags.append("-rdynamic");
    if configuration == 'debug':
        cflags.append("-O0")
    elif configuration == 'opt':
        cflags.append("-O3")
elif compiler == 'clang':
    extracflags = [ "-g", "-Wall",  '-fexceptions', '-ftrapv', '-DFBXSDK_NEW_API'] #, '-fvisibility=hidden']   
    cflags = cflags+extracflags
    
    if default_env['PLATFORM']=='darwin':
        default_env['CXX'] = '/Developer//usr/bin/clang++'
        default_env['CC'] = '/Developer//usr/bin/clang'
    else:
        default_env['CXX'] = 'clang++'
        default_env['CC'] = 'clang'
    cflags.append("-std=c++0x");
    cflags.append("-pedantic")
    cflags.append("-Wextra" )
    #cflags.append("-Werror" )
    lflags.append("-L/usr/lib/")
    if configuration == 'debug':
        cflags.append("-O0")
    elif configuration == 'opt':
        cflags.append("-O3")
elif compiler == 'vc':
    default_env['CXX'] = 'cl.exe'
    default_env['CC'] = 'cl.exe'
    cflags.append("/WX")
    cflags.append("/EHsc")
    cflags.append("/ZI")
    if configuration == 'debug':
        cflags.append("-Od")
    elif configuration == 'opt':
        cflags.append("-O3")
    #lflags.append("/NODEFAULTLIB:library")
    lflags.append("/DEBUG")
    cflags.append("/MDd")
else:
    print "this isn't good"
default_env.Append(CPPPATH=[default_env.Dir("#3rdParty/include")])
#default_env['CXX'] = 'scan-build -k'
#default_env['CC'] = 'scan-build -k'


default_env.AppendUnique(CPPFLAGS=cflags )
default_env.AppendUnique(CFLAGS=cflags)
#default_env.AppendUnique(CXXFLAGS=cflags)
default_env.AppendUnique(LINKFLAGS=lflags)
variant ="%s-%s-%s-%s" % (default_env['CC'], platform.machine(),  platform.architecture()[0], configuration) 
print variant

if not GetOption('verbose'):
    default_env['RANLIBCOMSTR'] = "[ranlib] $TARGET" 
    default_env['ARCOMSTR'] = "[ar] $TARGET"
    default_env['CCCOMSTR'] = "[cc] $SOURCE"
    default_env['CXXCOMSTR'] = "[cxx] $SOURCE"
    default_env['LINKCOMSTR'] = "[link] $TARGET"
    default_env['INSTALLSTR'] = '[install] $TARGET'

default_env['INSTALL_DIR'] = '#/bin/' + variant 
default_env['BUILD_VARIANT'] = variant
    
##############################################
##############################################


default_env.Tool('Profiler', toolpath=['site_scons/site_tools'])
default_env.Tool('Catalog', toolpath=['site_scons/site_tools'])
default_env.Tool('SConsWalk', toolpath=['site_scons/site_tools'])
default_env.Tool('Wrappers', toolpath=['site_scons/site_tools'])
default_env.Tool('RSync', toolpath=['site_scons/site_tools'])
default_env.Tool('Data', toolpath=['site_scons/site_tools'])
default_env.Tool('UnitTest', toolpath=['site_scons/site_tools'])

if default_env['PLATFORM']=='win32':
    #default_env['ENV']['PATH'] += "C:\Program Files/Microsoft Visual Studio 10.0/VC/bin/"
    print "1"
    os.system("\"c:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat\"")
    print "2"
else:
    default_env['ENV']['PATH'] = '/usr/local/bin:/opt/local/bin:/opt/local/sbin:/usr/bin:/bin:/Developer/usr/bin' 
    default_env['ENV']['PKG_CONFIG_PATH'] = '/usr/local/lib/pkgconfig:/usr/local/lib64/pkgconfig'
    print default_env['ENV']['PATH']



sconsFilesList = [
#third party
"./3rdParty/collada/SConscript",
"./3rdParty/fbx/SConscript",
"./3rdParty/freetype/SConscript",
"./3rdParty/il/SConscript",
"./3rdParty/libfreenect/SConscript",
"./3rdParty/zlib/SConscript",
"./3rdParty/libpng/SConscript",
"./3rdParty/opencv/SConscript",
"./3rdParty/opengl/SConscript",
"./3rdParty/openal/SConscript",
"./3rdParty/ogg/SConscript",
"./3rdParty/vorbis/SConscript",
"./3rdParty/glew/SConscript",
"./3rdParty/sdl/SConscript",
"./3rdParty/pthread/SConscript",

#libs
"./lib/gcl/gcl/SConscript",
"./lib/sound/SConscript",
"./lib/windriver/SConscript",
"./lib/input/SConscript",
"./lib/kinetestlib/SConscript",
"./lib/kinect/SConscript",
"./lib/renderer/SConscript",
"./lib/voxel/SConscript",
"./lib/applayer/SConscript",

#tools
"./program/tools/common/SConscript",
"./program/tools/common/unittest/SConscript",

"./program/tools/meshconverter/SConscript",

#tools unittest


#unittest
"./lib/gcl/gcl/unittest/SConscript",
"./lib/sound/unittest/SConscript",
"./lib/windriver/unittest/SConscript",
"./lib/input/unittest/SConscript",
"./lib/kinetestlib/unittest/SConscript",
"./lib/renderer/unittest/SConscript", 
"./lib/kinect/unittest/SConscript",
"./lib/voxel/unittest/SConscript",
"./lib/applayer/unittest/SConscript",

#programs
"./program/example/example_data/SConscript",
"./program/example/basicTextureMapping/SConscript",
"./program/example/spinningCube/SConscript",
"./program/example/spriteExample/SConscript",
"./program/example/textRenderingExample/SConscript",
"./program/example/meshLoading/SConscript",


"./program/sudoku/SConscript",
"./program/arkanoid/SConscript",
"./program/kinevox/SConscript",
]
if GetOption('genValgrindSuppressions'):
    sconsFilesList.append("./tools/valgrindgen/SConscript") #this tool generates some rules for valgrind so that it ignores certain pattern of memory errors that we don't care about

#default_env.StampTime("start SConscript Parse...")
default_env.SConsWalkList(sconsFilesList, './SConscript', variant)
default_env.StampTime("Parsing Time")
if "@aliases" in targetList:
    default_env.displayAliases()
    sys.exit()

def OnExit():
    default_env.StampTime("exit")

atexit.register(OnExit)
