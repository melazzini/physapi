import numpy as np
from pylab import linspace, plot, show, grid, legend, full, xlabel, ylabel, sin, cos, xscale, yscale, title

myArray = np.array([1, 2, 3, 4, 5])

myArray = myArray[myArray > 0]

print(myArray)


data = np.loadtxt("intrinsicFlux")
x = data[:, 0]
y = data[:, 1]

plot(x, y, label='source')

data = np.loadtxt("flux70_dE")
x = data[:, 0]
y = data[:, 1]

plot(x, y, label='clumpy torus')


xscale('log')
yscale('log')
xlabel('energy, eV')
ylabel(r'$ E^{2} \frac{dN}{dE},\; \frac{keV^{2}}{sr*s*keV} $')
title(r'$AGN\; FLUX, \; N_{H}=10^{24}\, cm^{-2} $')
legend()
show()
