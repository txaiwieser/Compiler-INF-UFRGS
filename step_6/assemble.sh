if [ $# -eq 1 ];
then
./etapa6 $1 out.s
gcc out.s
echo "Execution result:"
./a.out
fi