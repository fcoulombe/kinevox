import os
import sys
import uuid
filterList = set()

rootdir = sys.argv[1]
print "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
print "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">"
 
print "\t<ItemGroup>"
for root, subFolders, files in os.walk(rootdir):
    root = root.replace(".\\", "")
    if root != "."  and root.find(".git") == -1 and root.find("\\build\\") == -1 :
        filterList.add(root)
    for file in files:
        if file.find(".sln") == -1 and file.find(".suo") == -1 and file.find("vcxproj") == -1:
            fullPath = os.path.join(root,file).replace(".\\", "")
            if root != ".":
                print "\t\t<None Include=\""+fullPath+"\" >"
                print "\t\t\t<Filter>"+root+"</Filter>"
                print "\t\t</None>"
            else:
                print "\t\t<None Include=\""+fullPath+"\" />"
                
            
print "\t</ItemGroup>"

print "\t<ItemGroup>"
for filter in filterList:
    print "\t\t<Filter Include=\""+filter+"\">"
    print "\t\t\t<UniqueIdentifier>{"+str(uuid.uuid4())+"}</UniqueIdentifier>"
    print "\t\t</Filter>"

print "\t</ItemGroup>"
print "</Project>"