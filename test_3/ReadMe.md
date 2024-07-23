Like cassioneri with Google Benchmark and cmake.
But with random inputs like mimicry.


Run with
```
cmake --preset=clang.release.make
cmake --build build/clang.release.make
./build/clang.release.make/bin/bench
./build/clang.release.make/bin/bench > times/3_sorting_network.txt

./build/clang.release.make/bin/bench_embed 200   401  > times/embed_3_200_c401_merge.txt
./build/clang.release.make/bin/bench_embed 20000 401  > times/embed_3_20000_c401_merge.txt
./build/clang.release.make/bin/bench_embed 200   4001 > times/embed_3_200_c4001_merge.txt
./build/clang.release.make/bin/bench_embed 20000 4001 > times/embed_3_20000_c4001_merge.txt

# switch to quicksort
./build/clang.release.make/bin/bench_embed 200   401  > times/embed_3_200_c401_quick.txt
./build/clang.release.make/bin/bench_embed 20000 401  > times/embed_3_20000_c401_quick.txt
./build/clang.release.make/bin/bench_embed 200   4001 > times/embed_3_200_c4001_quick.txt
./build/clang.release.make/bin/bench_embed 20000 4001 > times/embed_3_20000_c4001_quick.txt
```