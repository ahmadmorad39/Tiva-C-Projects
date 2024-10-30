import tkinter as tk
from tkinter import *
import serial as ser
import math
import numpy as np
import pygame
from tkinter import filedialog as fd
import time




# when the timeout expires the program will continue
# clear buffers

global State, s, enableTX


s = ser.Serial('COM4', baudrate=115200, bytesize=ser.EIGHTBITS, parity=ser.PARITY_NONE,
                              stopbits=ser.STOPBITS_ONE,
                              timeout=0.01)  # timeout of 1 sec so that the read and write operations are blocking,
s.reset_input_buffer()
s.reset_output_buffer()
enableTX = True
angle = '00'
State = 0

def start_draw_board():
    global State,s,enableTX
    time.sleep(0.25)
    pygame.init()
    fpsclock = pygame.time.Clock()  # create the display surface object
    win = pygame.display.set_mode()
    pygame.display.toggle_fullscreen()
    # set the pygame window name
    pygame.display.set_caption("Drawing Board")
    width = win.get_width()
    height = win.get_height()
    # marker current co-ordinates
    x = width // 2
    y = height // 2
    radius = 2
    # velocity / speed of movement
    vel = 1
    # Indicates pygame is running
    run = True

    # mode 0 = draw, 1 move, 2 erase , 3 move
    mode = 0
    c_color = (255, 0, 0)
    win_backup = win.copy()
    enableTX = True

    width_val = 2

    # infinite loop
    while run:
        v_x = 0
        v_y = 0

        while (s.in_waiting > 0):  # while the input buffer isn't empty
            try:
                line = s.readline().decode("ascii")[:-1]
                line_split = line.split('_')
                if line_split[0] == 'XY':
                    X = (int(line_split[1], 16) - 2074)
                    Y = (int(line_split[2], 16) - 2315)

                    v_x = X/9200
                    v_y = Y/9200
                    print(f"coordinates {v_x}, {v_y}")
                elif line_split[0] == 'TB':
                    # restart backup
                    win.blit(win_backup, (0, 0))
                    mode += 1
                    if mode == 3:
                        mode = 0

                    if mode == 0:
                        c_color = (255, 0, 0)
                    elif mode == 1:
                        c_color = (255, 255, 255)
                    elif  mode == 2:
                        c_color = (0, 0, 255)
                if (s.in_waiting == 0):
                    enableTX = True
            except:
                print("Error")

        # iterate over the list of Event objects
        # that was returned by pygame.event.get() method.
        for event in pygame.event.get():
            # if event object type is QUIT
            # then quitting the pygame
            # and program both.
            if event.type == pygame.QUIT:
                # it will make exit the while loop
                run = False
                enableTX = True
                while (s.out_waiting > 0 or enableTX):  # while the output buffer isn't empty
                    bytetxMsg = bytes('s0' + '\n', 'ascii')
                    s.write(bytetxMsg)
                    if s.out_waiting == 0:
                        enableTX = False

        # TX
        while (s.out_waiting > 0 or enableTX):  # while the output buffer isn't empty
            bytetxMsg = bytes('\n', 'ascii')
            s.write(bytetxMsg)
            if s.out_waiting == 0:
                enableTX = False


        if abs(v_x) <= 0.21 and abs(v_y) <= 0.21:
            v_x = 0
            v_y = 0

        # stores keys pressed
        keys = pygame.key.get_pressed()
        if keys[pygame.K_SPACE]:
            #restart backup
            win.blit(win_backup, (0, 0))
            mode += 1
            if mode == 4:
                mode = 0
                c_color = (255, 0, 0)
            if mode == 1 or mode == 3:
                c_color = (0, 0, 255)
            elif mode == 0:
                c_color = (255, 0, 0)
            elif mode == 2:
                c_color = (255, 255, 255)

        if keys[pygame.K_ESCAPE]:
            run = False

        # if left arrow key is pressed
        if keys[pygame.K_UP]:
            # decrement in y co-ordinate
            width_val += 1
            if width_val == 0:
                width_val =1

        # if left arrow key is pressed
        if keys[pygame.K_DOWN]:
            # increment in y co-ordinate
            width_val -= 1


        if mode != 0:
            #restart backup
            win.blit(win_backup, (0, 0))

        if mode == 1:
            pygame.draw.line(win, (0, 0, 0), (x, y), (x+v_x, y+v_y), width=2*width_val)
            #pygame.display.flip()


        #pygame.draw.circle(win, c_color, (x, y), radius)
        if mode == 0:
            pygame.draw.line(win, c_color, (x, y), (x + v_x, y + v_y), width=width_val)
            win_backup = win.copy()
        else:
            win_backup = win.copy()
            pygame.draw.circle(win, c_color, (x + v_x, y + v_y), 4)

        x += v_x
        y += v_y

        # drawing spot on screen which is rectangle here
        if(x < 0):
            x = 0
        if(y< 0):
            y = 0
        if (x > width):
            x = width
        if(y> height):
            y = height

        # it refreshes the window
        pygame.display.flip()
        fpsclock.tick(60)

    # closes the pygame window
    pygame.quit()



def states_loop():
    global enableTX, s, State, angle
    if (State == 1):
        line = None
        while (s.in_waiting > 0):  # while the input buffer isn't empty
            try:
                line = s.readline().decode("ascii")[:-1]
            except:
                pass
            if (s.in_waiting == 0):
                enableTX = True
        if line:
            line_split = line.split('_')
            if line_split[0] == 'XY':
                X = -1 * (int(line_split[1], 16) - 480)
                Y = (int(line_split[2], 16) - 480)
                R = np.sqrt(X ** 2 + Y ** 2)
                if not(abs(X) <= 250 and abs(Y) <= 250):
                    angle = math.degrees(2 * math.atan2(Y, X + R))
                    if angle < 0:
                        angle = angle + 360
                    print(X, Y, angle)
                    angle = str(hex(int((angle / 360.0) * 510.0)))[2:].upper()
                    print(X, Y, angle)
            if (s.in_waiting == 0):
                enableTX = True
            # TX
        if line:
            msg = 'a' + angle
            while (s.out_waiting > 0 or enableTX):  # while the output buffer isn't empty
                s.write(bytes(msg + '\n', 'ascii'))
                if s.out_waiting == 0:
                    enableTX = False

    if (State == 3) or (State == 4) or (State == 5):
        line = None
        while (s.in_waiting > 0):  # while the input buffer isn't empty
            try:
                line = s.readline().decode("ascii")[:-1]
            except:
                pass
            if (s.in_waiting == 0):
                enableTX = True
        if line:
            line_split = line.split('_')
            if line_split[0] == 'CA':
                steps = (int(line_split[1], 16))
                current_steps.config(text="Current Stepper Motor Steps: " + str(steps))
                c_angle = (steps / 510.0) * 360
                current_angle.config(text="Current Stepper Motor Angle: " + "{:.1f}".format(c_angle))
                print(steps, c_angle)
            if (s.in_waiting == 0):
                enableTX = True
    if State == 6:
        line = None
        while (s.in_waiting > 0):  # while the input buffer isn't empty
            try:
                line = s.readline().decode("ascii")[:-1]
            except:
                pass
            if (s.in_waiting == 0):
                enableTX = True
        if line:
            line_split = line.split('_')
            if line_split[0] == 'CC':
                steps = (int(line_split[1], 16))
                current_cal.config(text="Current Stepper Calibration Counter: " + str(steps))
                c_angle = (1 / steps) * 360
                current_cal_a.config(text="Current Stepper Calibration Angle: {:.1f}".format(c_angle))
                print(steps, c_angle)
            if (s.in_waiting == 0):
                enableTX = True
    root.after(5, states_loop)  # After 5ms, call scanning
    return



def open_text_file(F_id):
    global s,enableTX
    enableTX = True
    filetypes = (
        ('text files', '*.txt'),
        ('All files', '*.*')
    )

    filename = fd.askopenfilename(
        title='Select A script',
        initialdir='/',
        filetypes=filetypes)

    if F_id == 1:
        start_script1["state"] = NORMAL
    elif F_id == 2:
        start_script2["state"] = NORMAL
    else:
        start_script3["state"] = NORMAL

    with open(filename) as f:
        lines = [line.rstrip().upper() for line in f]

    file_str = ''
    for line in lines:
        if len(line) != 0:
            file_str += line
    print(file_str)
    msg = "F" + str(F_id) + file_str
    while s.out_waiting > 0 or enableTX:  # while the output buffer isn't empty
        s.write(bytes(msg + '\n', 'ascii'))
        if s.out_waiting == 0:
            enableTX = False
    return



def start_script(script_num):
    global s,enableTX,State
    enableTX = True
    msg = 'i' + str(script_num)
    while s.out_waiting > 0 or enableTX:  # while the output buffer isn't empty
        s.write(bytes(msg + '\n', 'ascii'))
        if s.out_waiting == 0:
            enableTX = False
    print("Starting script number ",  script_num)
    State = 2 + script_num
    return



def send_state(state_num):
    global State,s,enableTX
    enableTX = True

    if State == 6 and state_num == 6:
        state_num = 0
        Cal_control['text'] = "Start Motor Calibration"

    if state_num == 6:
        Cal_control['text'] = "Stop Motor Calibration"

    State = state_num
    print("starting state " + str(state_num))
    msg = 's' + str(state_num)
    while s.out_waiting > 0 or enableTX:  # while the output buffer isn't empty
        s.write(bytes(msg + '\n', 'ascii'))
        if s.out_waiting == 0:
            enableTX = False

    if state_num == 2:
        start_draw_board()
        time.sleep(0.1)

    root.after(5, states_loop)  # After 5ms, call states_loop
    return



root = tk.Tk()
root.title('DCS EE PROJECT')
canvas = tk.Canvas(root, height=600, width=800, bg="#3F3F3F")
canvas.pack()

frame_1 = tk.Frame(root, bg="#5F5F5F")
frame_1.place(relheight=0.9, relwidth=0.4, relx=0.55, rely=0.05)

run_script1 = tk.Button(frame_1, text="Select Script 1", fg="black", bg="#F3F3F3", command= lambda: open_text_file(1))
run_script1.place(relwidth=0.3, relx=0.15, rely=0.05)


start_script1 = tk.Button(frame_1, text="Start Script 1", fg="black", bg="#F3F3F3", state=DISABLED, command= lambda: start_script(1))
start_script1.place(relwidth=0.3, relx=0.55, rely=0.05)


run_script2 = tk.Button(frame_1, text="Select Script 2", fg="black", bg="#F3F3F3", command= lambda: open_text_file(2))
run_script2.place(relwidth=0.3, relx=0.15, rely=0.15)


start_script2 = tk.Button(frame_1, text="Start Script 2", fg="black", bg="#F3F3F3", state=DISABLED, command= lambda: start_script(2))
start_script2.place(relwidth=0.3, relx=0.55, rely=0.15)


run_script3 = tk.Button(frame_1, text="Select Script 3", fg="black", bg="#F3F3F3", command= lambda: open_text_file(3))
run_script3.place(relwidth=0.3, relx=0.15, rely=0.25)


start_script3 = tk.Button(frame_1, text="Start Script 3", fg="black", bg="#F3F3F3", state=DISABLED, command= lambda: start_script(3))
start_script3.place(relwidth=0.3, relx=0.55, rely=0.25)



Cal_control = tk.Button(frame_1, text="Start Motor Calibration ", fg="black", bg="#F3F3F3", command= lambda: send_state(6))
Cal_control.place(relwidth=0.8, relx=0.1, rely=0.45)



Manual_control = tk.Button(frame_1, text="Start Manual control of motor", fg="black", bg="#F3F3F3", command= lambda: send_state(1))
Manual_control.place(relwidth=0.8, relx=0.1, rely=0.55)


Draw_board = tk.Button(frame_1, text="Start Drawing-Board", fg="black", bg="#F3F3F3", command= lambda: send_state(2))
Draw_board.place(relwidth=0.8, relx=0.1, rely=0.65)

frame_2 = tk.Frame(root, bg="#5F5F5F")
frame_2.place(relheight=0.9, relwidth=0.4, relx=0.05, rely=0.05)

# Draw an Oval in the canvas
# frame_2.create_oval(0,0,100,100)


current_angle = tk.Label(frame_2, text='Current Stepper Motor Angle:')
current_angle.place(relwidth=0.8, relx=0.1, rely=0.7)

current_steps = tk.Label(frame_2, text='Current Stepper Motor Steps:')
current_steps.place(relwidth=0.8, relx=0.1, rely=0.5)


current_cal = tk.Label(frame_2, text='Current Stepper Calibration Counter: 512')
current_cal.place(relwidth=0.8, relx=0.1, rely=0.3)

current_cal_a = tk.Label(frame_2, text='Current Stepper Calibration Angle: 0.7')
current_cal_a.place(relwidth=0.8, relx=0.1, rely=0.1)


#current_angle.pack()
#current_steps.pack()

#Make the window resizable false
root.resizable(False,False)

root.after(5, states_loop)  # After 5ms, call states_loop
root.mainloop()
