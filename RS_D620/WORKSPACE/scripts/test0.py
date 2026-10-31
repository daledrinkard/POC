#!/usr/bin/env python3
"""
test0.py <port_number> <filename>

Sends command lines from an ASCII file to a COM port at 115200,8,N,1.

<port_number> is the numeric part of the COM port, e.g. "6" for COM6.
<filename> is an ASCII text file read one line at a time. Any line that
begins with 'S' has everything up to and including the first 'P' in that
line sent out the serial port (and echoed to the terminal, followed by a
CRLF), followed by a 10ms pause. Lines not starting with 'S' are ignored.
Any bytes arriving from the COM port are echoed to the terminal as they
arrive. After the last line of the file is processed, the script pauses
for 1 second before closing the port.

Requires pyserial: pip install pyserial
"""
import sys
import time
import threading

try:
    import serial
except ImportError:
    sys.exit("This script requires pyserial. Install it with: pip install pyserial")


def reader_thread(ser, stop_event):
    while not stop_event.is_set():
        data = ser.read(ser.in_waiting or 1)
        if data:
            sys.stdout.write(data.decode('ascii', errors='replace'))
            sys.stdout.flush()


def main():
    if len(sys.argv) != 3:
        sys.exit(f"usage: {sys.argv[0]} <com_port_number> <filename>")

    port_name = f"COM{sys.argv[1]}"
    filename = sys.argv[2]

    ser = serial.Serial(
        port=port_name,
        baudrate=115200,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=0.05,
    )

    stop_event = threading.Event()
    thread = threading.Thread(target=reader_thread, args=(ser, stop_event), daemon=True)
    thread.start()

    try:
        with open(filename, 'r', encoding='ascii', errors='replace') as f:
            for line in f:
                line = line.rstrip('\r\n')
                if not line.startswith('S'):
                    continue
                p_index = line.find('P')
                if p_index == -1:
                    print(f"[test0] no 'P' found in line, skipping: {line}")
                    continue
                payload = line[:p_index + 1]
                ser.write(payload.encode('ascii'))
                sys.stdout.write(payload + '\r\n')
                sys.stdout.flush()
                time.sleep(0.010)
        time.sleep(1)
    finally:
        stop_event.set()
        thread.join()
        ser.close()


if __name__ == "__main__":
    main()
