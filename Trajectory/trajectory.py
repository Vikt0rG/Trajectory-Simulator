import sys
import argparse

import socket

import numpy as np

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.collections import LineCollection

# Function to set up socket connection
def init_socket():
    HOST = '127.0.0.1'  # Localhost
    PORT = 8080
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    return s

# Function to receive data from C++ server
def receive_data(sock):
    data = sock.recv(1024)  # Receive up to 1024 bytes
    decoded_data = data.decode('utf-8')  # Decode bytes to string
    # print(f'data: {data} \n decoded: {decoded_data}')
    # Split the data by newlines and process each line
    lines = decoded_data.strip().split('\n')
    
    # Handle the first line of data
    if len(lines) > 0:
        x_str, y_str = lines[0].split(',')  # Split by the comma for the first line
        return float(x_str), float(y_str)

    return None, None  # If no data is received, return None

# Define the initialization function for the animation
def init():
    global line
    line.set_segments([])  # Clear previous segments
    point.set_offsets([[0, 0]])  # Initialize scatter point
    return line, point

# Define the animate function for updating the plot
def animate(i, sock, xdata, ydata, line, point, complete_trajectory):
    x, y = receive_data(sock)
    max_age = 50
    trail = 25
    alpha_max = 1.0
    alpha_min = 0.0

    if x is not None and y is not None:
        xdata.append(x)
        ydata.append(y)

        len_data = len(xdata)
        if not complete_trajectory and len_data > max_age:
            xdata.pop(0)
            ydata.pop(0)

        # Create segments for LineCollection
        segments = [[(xdata[i], ydata[i]), (xdata[i + 1], ydata[i + 1])] for i in range(len(xdata) - 1)]

        if len(segments) > 0:
            if len(segments) <= trail:
                alpha_gradient = np.linspace(alpha_max, alpha_min, len(segments))
            else:
                alpha_gradient = np.concatenate((
                    np.linspace(alpha_max, alpha_min, trail)[::-1],
                    np.ones(len(segments) - trail)
                ))

            # Create color array for segments
            colors = np.array([[1, 0.647, 0, alpha] for alpha in alpha_gradient])

            # Set the segments and apply the corresponding colors
            line.set_segments(segments)
            line.set_color(colors)

        # Update scatter point (the moving particle)
        point.set_offsets([x, y])

    return line, point

def main(complete_trajectory):
    # Set up the figure and axis
    fig, ax = plt.subplots(figsize=(5, 5), constrained_layout=True)

    ax.set_facecolor('black')
    fig.patch.set_facecolor('black')
    ax.set_xlim(-20, 20)  
    ax.set_ylim(-20, 20)

    # Initialize empty data lists
    xdata, ydata = [], []

    # Create LineCollection for the line
    global line, point
    line = LineCollection([], linewidths=2)
    ax.add_collection(line)

    # Create scatter point
    point = ax.scatter([], [], color='darkorange')

    # Set up animation
    ani = animation.FuncAnimation(
        fig, animate, fargs=(sock, xdata, ydata, line, point, complete_trajectory),
        interval=3,
        init_func=init,
        blit=True
    )
    plt.show()

if __name__ == "__main__":
    # Set up the socket connection
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('localhost', 8080))

    parser = argparse.ArgumentParser(description='Process flags.')
    parser.add_argument('--complete_trajectory', '-ct', action='store_true',
                        help='Whether to plot the entire trajectory (default = False)')

    # Parse the command-line arguments
    args = parser.parse_args()

    main(args.complete_trajectory)

    sock.close()