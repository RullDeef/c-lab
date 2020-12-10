import tkinter as tk
from tkinter import simpledialog

# actions

from awork import fill_fib

# actual data

ARRAYS_COUNT = 2
arrays = [()] * ARRAYS_COUNT
array_selected = 0

# GUI

root = tk.Tk()
root.title("app")
root.geometry("640x480")

root.rowconfigure(0, weight=0)
root.rowconfigure(1, weight=1)

root.columnconfigure(0, weight=1)

# make radio buttons for each array selection
array_selector = tk.IntVar(root, value=0)
array_strings = []

def selector_trace(*args):
    global array_selected
    array_selected = array_selector.get()

array_selector.trace("w", selector_trace)

def make_region_data():
    frame = tk.Frame(root)
    frame.grid(row=0, column=0, sticky=tk.NSEW, padx=64, pady=16)

    for i in range(ARRAYS_COUNT):
        frame.rowconfigure(i, weight=1)

    frame.columnconfigure(0, weight=1)
    frame.columnconfigure(1, weight=3)
    frame.columnconfigure(2, weight=12)

    def make_region_array(row):
        radio = tk.Radiobutton(frame, variable=array_selector, value=row)
        radio.grid(row=row, column=0)

        label = tk.Label(frame, text=f"массив #{row + 1}")
        label.grid(row=row, column=1, sticky=tk.W)

        strvar = tk.StringVar(root, value="")

        def trace(*args, **kwargs):
            arrays[row] = tuple(map(int, filter(lambda s: s != "-", strvar.get().split())))
            print(arrays)

        strvar.trace("w", trace)

        def validate(action_type, string):
            if action_type == "1":
                try:
                    for s in string.split():
                        if s != "-":
                            int(s)
                except:
                    root.bell()
                    return False
            return True

        entry = tk.Entry(frame, textvariable=strvar, validate="key")
        entry.config(validatecommand=(entry.register(validate), "%d", "%P"))
        entry.grid(row=row, column=2, sticky=tk.EW)

        array_strings.append(strvar)

    for row in range(ARRAYS_COUNT):
        make_region_array(row)

make_region_data()

def make_region_action():
    frame = tk.Frame(root)
    frame.grid(row=1, column=0, sticky=tk.NSEW, padx=64, pady=16)

    for i, w in enumerate((14, 2)):
        frame.columnconfigure(i, weight=w)

    # fill fib
    def fill_fib_action():
        n = simpledialog.askinteger("Внимание", "Укажите размер массива", parent=root, minvalue=1, maxvalue=20)
        if n is not None:
            array = fill_fib(n)
            array_strings[array_selected].set(" ".join(map(str, array)))

    lbl_fill_fib = tk.Label(frame, text="Заполнить выбранный массив числами Фиббоначи")
    lbl_fill_fib.grid(row=0, column=0)
    btn_fill_fib = tk.Button(frame, text="выполнить", command=fill_fib_action)
    btn_fill_fib.grid(row=0, column=1)

make_region_action()

root.mainloop()
