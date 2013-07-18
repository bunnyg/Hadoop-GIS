#! /usr/bin/python


import sys
import math
import random


def genfields():
    fields = []
    for i in range(0,10):
	fields.append(str(random.random()))
    
    c = random.randint(0,3)
    for i in range(0,c):
	fields.append("")

    return random.sample(fields,7)

def main():
    if len(sys.argv)< 2:
	print >> std.err, "Missing argument: [dataset id]"
	sys.exit(0)
    did = sys.argv[1].strip()

    oid = 1 
    for line in sys.stdin:
	sp =line.strip().split("\t")
	tile_id = sp[0].split("_")
	del tile_id[-1]
	tile_id = "_".join(tile_id)

	print "\t".join((tile_id,did, str(oid),"\t".join(genfields()),sp[-1]))
	oid = oid +1

if __name__ == '__main__':
    main()
