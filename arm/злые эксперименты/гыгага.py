# -*- coding: utf-8 -*-

import pylab
from mpl_toolkits.mplot3d import Axes3D
import numpy

file = open("data.txt", "r")
data = [map(float, line.split("\t")) for line in file]

def makeData():
    (x,y,z) = (data[0::3],data[1::3],data[2::3])
    return x, y, z


if __name__ == '__main__':
    x, y, z = makeData()

    fig = pylab.figure()
    axes = Axes3D(fig)

    axes.plot_trisurf(x, y, z)

    pylab.xlim(x.min(), x.max())
    pylab.ylim(y.min(), y.max())
    pylab.show()
    
