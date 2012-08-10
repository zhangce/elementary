
import sys

vid2word = {}
for l in open(sys.argv[2], 'r'):
    (vid, word) = l.rstrip().split('\t')
    vid2word[vid] = word

topic2word = {}
for l in open(sys.argv[1], 'r'):
    (vid, topic) = l.rstrip().split('\t')
    
    word = vid2word[vid]
    if topic not in topic2word:
        topic2word[topic] = {}

    if word not in topic2word[topic]:
        topic2word[topic][word] = 0

    topic2word[topic][word] = topic2word[topic][word] + 1

topics = []
for topic in topic2word:
    topics.append(int(topic))
topics.sort()

for ntopic in topics:
    
    topic = '%d' % ntopic
    
    ct = 0
    sys.stdout.write('TOPIC #')
    sys.stdout.write(topic)
    sys.stdout.write('   \n')
    
    for word in sorted(topic2word[topic], key=topic2word[topic].get, reverse=True):
        
        ct = ct + 1
        if ct == 20:
            break
        
        sys.stdout.write('  ')
        sys.stdout.write(word)
        sys.stdout.write('(')
        sys.stdout.write('%d)  ' % topic2word[topic][word])

    sys.stdout.write('\n\n')
