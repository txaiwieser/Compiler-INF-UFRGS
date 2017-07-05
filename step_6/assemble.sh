if [ $# -eq 1 ]; then
	sh recompile.sh
	./etapa6 $1 out.s
	gcc out.s
	echo "Execution:"
	./a.out
else
	echo $0 "error: a file path as parameter is needed."
fi