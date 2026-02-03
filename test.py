from stat_engine import StatArray
import random
import numpy as np

# data = [random.random() * 100 for _ in range(1_000_000)]

# arr = StatArray(data)

# print(arr.mean)
# print(arr.std)
# print(arr.variance)

# nums = []
# new_arr = StatArray(nums)
# print(new_arr.mean)
# print(new_arr.std)
# print(new_arr.variance)

# with StatArray([1,2,3,4,5]) as stats:
#     print(stats.mean)
#     print(stats.std)
#     print(stats.variance)


# data = [6,3,7,9,1,4, 7.5, 8.79996]
# arr = StatArray(data)
# print(arr.mean())
# print(arr.variance(ddof=0))
# print(arr.std(ddof=0))
# print(arr.median())

with StatArray([6,3,7,9,1,4, 7.5, 8.79996]) as stats:
    print(stats.mean())
    print(stats.variance())
    print(stats.std())
    print(stats.median())

arr1 = np.array([6,3,7,9,1,4, 7.5, 8.79996])
print(arr1.mean())
print(arr1.var())
print(arr1.std())
print(np.median(arr1))