Tuffy is an open-source Markov logic network inference engine.
The present version is 0.3, released May 1, 2011.
You can learn more about it at http://research.cs.wisc.edu/hazy/tuffy/
User manual can be downloaded at http://research.cs.wisc.edu/hazy/tuffy/doc/tuffy-manual.pdf

You may want to try out some sample MLNs:

SMOKE:
java -jar tuffy.jar -i samples/smoke/prog.mln -e samples/smoke/evidence.db -queryFile samples/smoke/query.db -r out.txt

UW-CSE:
java -jar tuffy.jar -i samples/cse/prog.mln -e samples/cse/evidence.db -queryFile samples/cse/query.db -r out.txt

IE:
java -jar tuffy.jar -i samples/ie/prog.mln -e samples/ie/evidence.db -queryFile samples/ie/query.db -r out.txt

CLASS:
java -jar tuffy.jar -i samples/class/prog.mln -e samples/class/evidence.db -queryFile samples/class/query.db -r out.txt
