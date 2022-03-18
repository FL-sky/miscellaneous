import time
  
time_start=time.time()

n = 100000000
s = 0
for i in range(n):
    s += (i ^ (i + 10))
time_end=time.time()
print(s)

print('time cost',time_end-time_start,'s')