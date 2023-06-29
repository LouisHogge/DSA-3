# python show_image.py nom_du_fichier.txt indice_de_limage

import matplotlib.pyplot as plt
import sys

classes = ["tree", "zigzag", "cell2", "cloud", "cell4", "broom", "door", "eiffel", "sun", "key"]

args = sys.argv
f_name = "testset.txt"
if(len(args) >= 2):
    f_name = args[1]

index = 0
if(len(args) >= 3):
    index = int(args[2])

f = open(f_name)
N = int(f.readline())

if index >= N:
    index = 0

while index > 0:
    f.readline()
    l = int(f.readline())
    for i in range(l):
        f.readline()
    index -= 1

c = int(f.readline())
l = int(f.readline())
x = []
y = []
s = []
for i in range(l):
    data = f.readline().split(" ")
    x.append(int(data[0]))
    y.append(-int(data[1]))
    s.append(False if int(data[2])==0 else True)

f.close()

plt.title(classes[c])

x_stroke = [x[0]]
y_stroke = [y[0]]
for i in range(1,l):
    if(s[i] == 1):
        plt.plot(x_stroke, y_stroke, "b")
        x_stroke.clear()
        y_stroke.clear()
    x_stroke.append(x[i])
    y_stroke.append(y[i])

plt.plot(x_stroke,y_stroke, "b")
plt.show()
