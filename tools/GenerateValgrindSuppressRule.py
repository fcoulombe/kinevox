
import os
import re
import sys


if len(sys.argv) < 3:
    print sys.argv[0] + " <cause> <symbol> <valgrindlog> "
    
cause = sys.argv[1]
symbolToLookFor = sys.argv[2]
f = open(sys.argv[3])

class Block(object):
    def __init__(self):
        self.stackFrames = []
blockList = []
currentBlock = None
state = "FindBlock"
for line in f:
    if state == "FindBlock": 
        if re.search("bytes in \d+ blocks are still reachable in loss record", line):
            state = "ParseBlock"
            currentBlock = Block()
    elif state == "ParseBlock":
        
        if re.search("^==\d+==$", line.strip()):
            state = "FindBlock"
            blockList.append(currentBlock)
            continue
        currentBlock.stackFrames.append(line)
        

#print len(blockList)
count = 0
for b in blockList:
    generateRule = False
    for line in b.stackFrames:
        if re.search(symbolToLookFor, line):
            generateRule = True
            count = count+ 1
            break
    if generateRule:
        print "{"
        print "   %s-%d"  % (cause, count)
        print "   Memcheck:Leak"
        for line in b.stackFrames:
            m = re.match("(==\d+==.+)(by 0x.+: )(.+)( \(in .+\))", line)
            if m != None:
                print "   fun:" + m.group(3)
        print "}"
        print ""
            
print "Count: %d Filtered: %d" % (len(blockList), count)
