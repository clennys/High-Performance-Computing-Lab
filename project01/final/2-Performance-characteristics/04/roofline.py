import matplotlib.pyplot as plt
import numpy as np

def find_constant_start_index(values):
    # Find the index from which the values become constant
    for i in range(1, len(values)):
        if all(val == values[i] for val in values[i:]):
            return i
    return len(values)

def plot_roofline(Pmax, bmax, Imin, Imax, N=1000, ax=None, **plt_kwargs):
    if ax is None:
        ax = plt.gca()
    I = np.logspace(np.log(Imin), np.log(Imax), N)
    P = bmax * I
    P = np.minimum(P, Pmax)
    peak = find_constant_start_index(P)
    ax.loglog(I, P, **plt_kwargs)
    ax.set_xscale('log', base=2)
    # ax.set_yscale('log', base=2)
    plt.fill_between(I[:peak+1], P[:peak+1], color='blue', alpha=0.3, label='Bandwith bound')
    plt.fill_between(I[peak:], P[peak:], color='green', alpha=0.3, label='Compute bound')

    peak_x = I[peak]
    peak_y = P[peak]
    ax.axvline(x=peak_x, color='red', linestyle='-', label="Ridge point")
    
    # Annotate the peak x value
    ax.text(peak_x, peak_y, f'{peak_x:.2f}', color='red', fontsize=10,
            verticalalignment='bottom', horizontalalignment='right')

    ax.grid(True)
    ax.set_xlim(Imin, Imax)
    ax.set_xlabel(rf"Operational Intensity $I$ [Flops/Byte]")
    ax.set_ylabel(rf"Performance $P$ [GFlops/s]")
    return ax


if __name__ == "__main__":
    fig, ax = plt.subplots()
    # ax = plot_roofline(Pmax=41.6, bmax=18, Imin=1.e-2, Imax=1.e+3, ax=ax,
    #                    label="EPYC 7H12 core")
    # ax = plot_roofline(Pmax=39.2, bmax=25, Imin=1.e-2, Imax=1.e+3, ax=ax,
    #                    label="EPYC 7763 core")
    ax = plot_roofline(Pmax=36.8, bmax=12, Imin=1.e-2, Imax=1.e+3, ax=ax,
                       label="Intel(R) Xeon(R) E5-2650 v3")
    ax.legend()
    plt.savefig("roofline.pdf")
    # plt.show()
