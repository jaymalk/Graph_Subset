# Complete run file, for single go run
_nm=$1
# Running the first set
./main $_nm 1
# Runnning minisat
./minisat "$_nm.satinput" "$_nm.satoutput"
# Running the second set
./main $_nm 2
# Making a results directory and moving the results there
mkdir ".result"
mv "$_nm"* ./".result"/
mv ".result" "$_nm.result"
# Now confirming that the results are correct
python2 ./.resources/check.py ./"$_nm.result"/"$_nm.graphs" ./"$_nm.result"/"$_nm.mapping"