
import os
import os.path
import re

GCLLicense = """/*
 * Copyright (C) 2011 by Francois Coulombe
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */"""

MD5License = """/*
  Copyright (C) 1999, 2000, 2002 Aladdin Enterprises.  All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  L. Peter Deutsch
  ghost@aladdin.com

 */"""

MD5HeaderLicense = """/*
	  Copyright (C) 1999, 2002 Aladdin Enterprises.  All rights reserved.

	  This software is provided 'as-is', without any express or implied
	  warranty.  In no event will the authors be held liable for any damages
	  arising from the use of this software.

	  Permission is granted to anyone to use this software for any purpose,
	  including commercial applications, and to alter it and redistribute it
	  freely, subject to the following restrictions:

	  1. The origin of this software must not be misrepresented; you must not
		 claim that you wrote the original software. If you use this software
		 in a product, an acknowledgment in the product documentation would be
		 appreciated but is not required.
	  2. Altered source versions must be plainly marked as such, and must not be
		 misrepresented as being the original software.
	  3. This notice may not be removed or altered from any source distribution.

	  L. Peter Deutsch
	  ghost@aladdin.com

 */"""

PythonLicense = """#Copyright (C) 2011 by Francois Coulombe

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
# """

excludeRules = ["[\.]git"]
boilerPlateExtensionList = ["[\.]cpp", "[\.]h" , "SConscript", "[\.]py"] 
def dirWalkCallBack(arg, dirname, fnames):
    for rule in excludeRules:
        if re.search(rule, dirname):
            return
    #print fnames
    for name in fnames:
        fullName = dirname + "/" + name
        for ext in boilerPlateExtensionList:
            if re.search(ext, fullName):
                #print fullName
                f = open(fullName, "r")
                if not f:
                    print "FAIL!"
                lineList = f.readlines()
                fileStr = ""
                for line in lineList:
                    fileStr = fileStr + line
                licenseList = [ fileStr.find(GCLLicense), 
                                fileStr.find("Jeffrey"), 
                                fileStr.find(MD5License),
                                fileStr.find(MD5HeaderLicense),
                                fileStr.find(PythonLicense)]
                hasFoundALicense = False
                for license in licenseList:
                    if license != -1:
                        hasFoundALicense = True
                                
                if  not hasFoundALicense:
                    print fullName
                    f.close()
                    f = open(fullName, "w")
                    if re.search("[\.]cpp$", fullName) or re.search("[\.]h$", fullName):
                        f.write(GCLLicense+fileStr)
                    elif re.search("SConscript", fullName) or re.search("[\.]py"):
                        print fullName
                        print fileStr
                        f.write(PythonLicense+"\n\n"+fileStr)
                f.close()
                    

    #print GCLLicense
    #if os.path.isfile(dirname): 
    #    print dirname
    

out = os.path.walk("lib", dirWalkCallBack, None)
