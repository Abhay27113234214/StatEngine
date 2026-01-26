from stat_engine import StatArray
import random

data = [random.random() * 100 for _ in range(1_000_000)]

arr = StatArray(data)

print(arr.mean())
print(arr.std())