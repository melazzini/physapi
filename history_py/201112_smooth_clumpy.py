import numpy as np
from pylab import linspace, plot, show, grid, legend, full, xlabel, ylabel, sin, cos, xscale, yscale, title

myArray = np.array([1, 2, 3, 4, 5])

myArray = myArray[myArray > 0]

print(myArray)


data = np.loadtxt("./results/NH24/intrinsicFlux_dE.txt")
x = data[:, 0]
y = x * data[:, 1] * 10**-6

plot(x, y, label='source')

data = np.loadtxt("./results/NH24/resultingFlux70_dE.txt")
x = data[:, 0]
y = x * data[:, 1] * 10**-6

plot(x, y, label='smooth torus')

# data = np.loadtxt("./toUSB/23_075_Flux_70_N=1.txt")
# x = data[:, 0]
# y = x * x * data[:, 1] * 10**-6

# plot(x, y, label=r'$\phi=0.03,\; N_{aver}=<0.75>, N=1$')

data = np.loadtxt("./toUSB/24_1_Flux_70_N=1.txt")
x = data[:, 0]
y = x * x * data[:, 1] * 10**-6

plot(x, y, label=r'$\phi=0.03,\; N_{aver}=<1>, N=1$')

data = np.loadtxt("./toUSB/24_2_Flux_70_N=2.txt")
x = data[:, 0]
y = x * x * data[:, 1] * 10**-6

plot(x, y, label=r'$\phi=0.03,\; N_{aver}=<2>, N=2$')

data = np.loadtxt("./toUSB/24_3_Flux_70_N=3.txt")
x = data[:, 0]
y = x * x * data[:, 1] * 10**-6

plot(x, y, label=r'$\phi=0.03,\; N_{aver}=<3>, N=3$')

data = np.loadtxt("./toUSB/24_4_Flux_70_N=4.txt")
x = data[:, 0]
y = x * x * data[:, 1] * 10**-6

plot(x, y, label=r'$\phi=0.03,\; N_{aver}=<4>, N=4$')

# data = np.loadtxt("./toUSB/23_5_Flux_70_N=5.txt")
# x = data[:, 0]
# y = x * x * data[:, 1] * 10**-6

# plot(x, y, label=r'$\phi=0.03,\; N_{aver}=<5>, N=5$')

xscale('log')
yscale('log')
xlabel('energy, eV')
ylabel(r'$ E^{2} \frac{dN}{dE},\; \frac{keV^{2}}{sr*s*keV} $')
title(r'$AGN\; FLUX, \; N_{H}=10^{24}\, cm^{-2} $')
legend()
show()
