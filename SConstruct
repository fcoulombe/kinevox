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
import sys
import platform
import subprocess
print platform.machine()
print platform.architecture()

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


default_env.Tool('Catalog', toolpath=['site_scons/site_tools'])
default_env.Tool('SConsWalk', toolpath=['site_scons/site_tools'])
default_env.Tool('Wrappers', toolpath=['site_scons/site_tools'])

default_env['ENV']['PATH'] = '/Developer/usr/bin:' + default_env['ENV']['PATH'] + ':/opt/local/bin:/opt/local/sbin' 
default_env['ENV']['PKG_CONFIG_PATH'] = '/usr/local/lib/pkgconfig'
print default_env['ENV']['PATH']


cflags = ["-O0", "-fPIC", "-g", "-Wall"]                                    
default_env.Append(CPPFLAGS=cflags )


default_env.SConsWalk(".", './SConscript')
if "@aliases" in targetList:
    default_env.displayAliases()
    sys.exit()

