
import sys

PRED_FILE = sys.argv[1]
FOLDER = sys.argv[2]

WEIGHT_FILE = FOLDER + "/factor_weight.tsv"
FACTOR_MEANING = FOLDER + "/factor_meaning.tsv"
VARIABLE_MEANING = FOLDER + "/variable_meaning.tsv"

OUTPUT_WEIGHT = FOLDER + ".prog.txt"
OUTPUT_PRED = FOLDER + ".pred.txt"

vm = {}
fm = {}

for l in open(FACTOR_MEANING, 'r'):
    (id, meaning) = l.rstrip().split('\t')
    fm[int(id)] = meaning


for l in open(VARIABLE_MEANING, 'r'):
    (id, meaning) = l.rstrip().split('\t')
    vm[int(id)] = meaning

fo = open(OUTPUT_WEIGHT, 'w')
ct = 0
for l in open(WEIGHT_FILE, 'r'):

    fo.write( l.rstrip() + " " + fm[ct] + "\n" )

    ct = ct + 1
fo.close()

if PRED_FILE != '~':

    fo = open(OUTPUT_PRED, 'w')
    for l in open(PRED_FILE, 'r'):
        ss = l.rstrip().split('\t')
        prob = ""
        if len(ss) == 3:
            prob = ss[2]

        vid = int(ss[0])
        pred = ss[1]

        if pred == "0":
            continue

        if prob != "":
            fo.write( prob + " " + vm[vid] + "\n" )

        else:
            fo.write( vm[vid] + "\n" )
    fo.close()
