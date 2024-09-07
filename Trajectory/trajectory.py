import sys

import socket

import matplotlib.pyplot as plt
import matplotlib.animation as animation

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
    print(f'data: {data} \n decoded: {decoded_data}')
    # Split the data by newlines and process each line
    lines = decoded_data.strip().split('\n')
    
    # Handle the first line of data
    if len(lines) > 0:
        x_str, y_str = lines[0].split(',')  # Split by the comma for the first line
        return float(x_str), float(y_str)

    return None, None  # If no data is received, return None

# Define the initialization function for the animation
def init():
    line.set_data([], [])
    return line,

# Define the animate function for updating the plot
def animate(i, sock, xdata, ydata, line):
    x, y = receive_data(sock)
    if x is not None and y is not None:
        xdata.append(x)
        ydata.append(y)
        line.set_data(xdata, ydata)
    return line,

def main():
    # Set up the figure and axis
    fig, ax = plt.subplots(figsize=(5,5), constrained_layout=True)
    ax.set_xlim(-10, 10)  # Adjust these limits based on your data range
    ax.set_ylim(-10, 10)  # Adjust these limits based on your data range

    # Initialize empty data lists
    xdata, ydata = [], []

    # Create a line object
    global line
    line, = ax.plot([], [], lw=2, color='darkorange')

    # Set up animation
    ani = animation.FuncAnimation(fig, animate, fargs=(sock, xdata, ydata, line), 
                                  interval=10, init_func=init, blit=True)

    plt.show()

if __name__ == "__main__":
    # Set up the socket connection
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('localhost', 8080))

    main()

    sock.close()