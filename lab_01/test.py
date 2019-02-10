input_file = open("in_2.txt", 'r')
size = int(input_file.readline())
print(size)
mtr = [[0] * size] * 2

line_1 = input_file.readline()
mtr[0] = list(map(float, line_1.split()))
line_2 = input_file.readline()
mtr[1] = list(map(float, line_2.split()))

##for i in range(size):
##    total = input_file.readline()
##    print(total)
##    mtr[i] = list(map(float, total.split()))

print(mtr)
    
