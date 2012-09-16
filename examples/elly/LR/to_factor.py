
m = {}
for l in open('lr_feat_unigram_nf_1.tsv', 'r'):
    (fid, vid, feature) = l.rstrip().split()
    fid = int(fid)
    vid = int(vid)
    feature = int(feature)
    m[vid/1000] = feature

m[0] = 0

fof = open('unigram.tsv', 'w')
fov = open('__vf.tsv', 'w')

ctf = -1
for v in sorted(m.iterkeys()):
    ctf = ctf + 1
    fof.write('%d\t1\t0\t%d\t%d\n' % (ctf, m[v], v))
    fov.write('%d\t23\t1\t0\t%d\n' % (v, ctf))

fof.close()
fov.close()
