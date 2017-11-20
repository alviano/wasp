for i in *.asp; do
    printf "$i ";
    cat $i | clasp --enum-mode=cautious | grep Consequences | tail -n1;
    printf "$i ";
    cat $i | ~/tesi/wasp_python/wasp/build/release/wasp --query-algorithm=ict | grep -v {} | grep { | tail -n1 | tr " " "\n" | wc -l
done
