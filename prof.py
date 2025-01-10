import pstats

stats = pstats.Stats("out.prof")
stats.strip_dirs().sort_stats("cumulative").print_stats()
