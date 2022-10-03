simple2:
	gcc -O3 -march=native -fopt-info-vec-optimized -o simple2 simple2.c

intrinsics:
	gcc -O3 -march=native -o intrinsics simple2_instrinsics.c

intrinsics2:
	gcc -O3 -march=native -o intrinsics2 simple2_instrinsics_part2.c

clean:
	rm -f simple2 intrinsics intrinsics2