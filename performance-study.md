
# Performance Testing

The following table shows the performance of the program across different versions and configurations. There are multiple tables, one for each machine used for testing.

### NYCK-DESKTOP
|  Version | 1000 64 0.4 0 | 200 128 .5 1000 | 10 512 .4 0 | 3 1024 .4 100 |
|----------|---------------|-----------------|-------------|---------------|
| Serial   |               |                 |             |               |
| OMP      |               |                 |             |               |
| MPI      |               |                 |             |               |

### NYCK-LAPTOP
|  Version | 1000 64 0.4 0 | 200 128 .5 1000 | 10 512 .4 0 | 3 1024 .4 100 |
|----------|---------------|-----------------|-------------|---------------|
| Serial   |               |                 |             |               |
| OMP      |     25.5s     |       38.3s     |    132.6s   |     337.4s    |
| MPI      |               |                 |             |               |


# Optimization Study

We will be studying how the implementations can be optimized, looking into Big O complexity, number of iterations and actual assembly instructions; and then comparing the performance of the different implementations to prove the hypotheses.

## Serial
This is the basis of OMP implementation too, so figuring out the optimizations for this will help in the OMP implementation too.

### Three grids strategy (3GS)

Without 3GS: 30.5s
With 3GS: ?

# Sample Instances

```
$ life3d 1000 64 0.4 0
1 81443 62
2 24563 24
3 20080 1
4 19016 1
5 17576 1
6 16905 1
7 15793 1
8 15174 1
9 14807 1
```

```
$ life3d 200 128 .5 1000
1 585667 87
2 198117 20
3 123360 6
4 117152 0
5 116181 0
6 116832 0
7 116421 0
8 116559 0
9 116344 0
```

```
$ life3d 10 512 .4 0
1 19157196 9
2 11835190 9
3 10389985 1
4 9659849 1
5 9049681 1
6 8563492 1
7 8146694 1
8 7824521 1
9 7580102 1
```

```
$ life3d 3 1024 .4 100
1 99923786 1
2 90413714 1
3 83137654 1
4 77287897 1
5 72448825 1
6 68444736 1
7 65198270 1
8 62633412 1
9 60611199 1
```