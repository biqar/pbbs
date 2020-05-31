for i in 1 2 4 8 16 24 32 40 80
do
echo "export OMP_NUM_THREADS=$i;./testInputs -x -r 3"
export OMP_NUM_THREADS=$i;./testInputs -x -r 3
done