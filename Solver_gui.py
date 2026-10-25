from __future__ import annotations
import csv, subprocess
from pathlib import Path
from tkinter import ttk, messagebox
import tkinter as tk
import matplotlib
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure

ROOT = Path(__file__).resolve().parent
BIN = ROOT / "myClass"
CSV_FILE = ROOT / "results" / "result.csv"
CSV_FILE.parent.mkdir(exist_ok=True)

def compile_solver():
    srcs = ["main.cpp", "Problem.cpp", "DifferenceScheme.cpp", "Quadrature.cpp", "Progonka.cpp", "AnalyticSolution.cpp"]
    cmd = ["g++", "-std=c++17", "-O2", "-o", str(BIN)] + srcs
    proc = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    if proc.returncode != 0:
        raise RuntimeError(proc.stderr.strip() or "Ошибка компиляции")

class SolverGui(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Лабораторная №2: Метод Баланса")
        self.geometry("1200x800")
        self._build()

    def _build(self):
        # Верхняя панель управления
        top_frame = ttk.Frame(self, padding=10)
        top_frame.pack(fill="x")

        ttk.Label(top_frame, text="Точность eps:").pack(side="left")
        self.var_eps = tk.StringVar(value="0.5e-6")
        ttk.Entry(top_frame, textvariable=self.var_eps, width=10).pack(side="left", padx=5)

        ttk.Label(top_frame, text=" | Задать n вручную:").pack(side="left")
        self.var_n = tk.StringVar(value="40")
        ttk.Entry(top_frame, textvariable=self.var_n, width=6).pack(side="left", padx=5)

        # Кнопки запуска с автоподбором и без
        btn_frame = ttk.Frame(self, padding=(10, 0))
        btn_frame.pack(fill="x")

        ttk.Button(btn_frame, text="1. Тест (Дирихле) [Авто]",
                   command=lambda: self.run_task("TestDirichlet", auto=True)).pack(side="left", padx=5)
        ttk.Button(btn_frame, text="2. Основа (Дирихле) [Авто]",
                   command=lambda: self.run_task("MainDirichlet", auto=True)).pack(side="left", padx=5)
        ttk.Button(btn_frame, text="3. Тест (Смешанная) [Авто]",
                   command=lambda: self.run_task("TestMixed", auto=True)).pack(side="left", padx=5)
        ttk.Button(btn_frame, text="4. Основа (Смешанная) [Авто]",
                   command=lambda: self.run_task("MainMixed", auto=True)).pack(side="left", padx=5)

        # Ручной запуск (для проверки таблиц и графиков)
        manual_frame = ttk.Frame(self, padding=(10, 5))
        manual_frame.pack(fill="x")
        ttk.Label(manual_frame, text="Ручной запуск (для таблиц):").pack(side="left")
        ttk.Button(manual_frame, text="1", command=lambda: self.run_task("TestDirichlet", auto=False)).pack(side="left")
        ttk.Button(manual_frame, text="2", command=lambda: self.run_task("MainDirichlet", auto=False)).pack(side="left")
        ttk.Button(manual_frame, text="3", command=lambda: self.run_task("TestMixed", auto=False)).pack(side="left")
        ttk.Button(manual_frame, text="4", command=lambda: self.run_task("MainMixed", auto=False)).pack(side="left")

        # Окно лога
        log_frame = ttk.LabelFrame(self, text="Вывод программы (Справка)", padding=5)
        log_frame.pack(fill="x", padx=10, pady=5)
        self.log_text = tk.Text(log_frame, height=6, wrap="word", font=("Courier", 11), bg="#f0f0f0")
        self.log_text.pack(fill="x")

        # Графики и таблица
        mid = ttk.Panedwindow(self, orient="horizontal")
        mid.pack(fill="both", expand=True, padx=10, pady=5)

        left = ttk.Frame(mid)
        right = ttk.Frame(mid)
        mid.add(left, weight=3)
        mid.add(right, weight=2)

        self.figure = Figure(figsize=(6, 4), dpi=100)
        self.canvas = FigureCanvasTkAgg(self.figure, master=left)
        self.canvas.get_tk_widget().pack(fill="both", expand=True)
        NavigationToolbar2Tk(self.canvas, left).update()

        self.table = ttk.Treeview(right, show="headings")
        self.table.pack(fill="both", expand=True)

    def run_task(self, task_name, auto):
        if not BIN.exists():
            try: compile_solver()
            except Exception as e:
                messagebox.showerror("Ошибка сборки", str(e))
                return

        cmd = [str(BIN)]
        if auto:
            cmd.append("-auto")
            cmd.append(task_name)
            cmd.append(self.var_eps.get())
        else:
            try:
                n = int(self.var_n.get())
                cmd.append(str(n))
                cmd.append(task_name)
            except ValueError:
                messagebox.showerror("Ошибка", "n должно быть числом")
                return

        try:
            proc = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, encoding="utf-8")
            if proc.returncode != 0:
                messagebox.showerror("Ошибка C++", proc.stderr)
                return

            self.log_text.delete("1.0", tk.END)
            self.log_text.insert(tk.END, proc.stdout)
            self.load_csv()
        except Exception as e:
            messagebox.showerror("Ошибка запуска", str(e))

    def load_csv(self):
        if not CSV_FILE.exists(): return
        with open(CSV_FILE, encoding="utf-8") as f:
            rows = list(csv.reader(f, delimiter=";"))
        if not rows: return
        headers, data = rows[0], rows[1:]

        self.table.delete(*self.table.get_children())
        self.table["columns"] = headers
        for col in headers:
            self.table.heading(col, text=col)
            self.table.column(col, width=80, anchor="center")
        step = max(1, len(data) // 1000)
        for i in range(0, len(data), step):
            self.table.insert("", "end", values=data[i])

        self.figure.clear()
        self.ax1 = self.figure.add_subplot(121)
        self.ax1.grid(True)
        idx = {h: i for i, h in enumerate(headers)}
        xs = [float(r[1]) for r in data]

        # График решений
        if "u(x)" in idx and "v(x)" in idx:
            self.ax1.plot(xs, [float(r[idx["u(x)"]]) for r in data], 'g--', label="Точное")
            self.ax1.plot(xs, [float(r[idx["v(x)"]]) for r in data], 'r-', label="Численное")
        elif "v(x)" in idx and "v2(x)" in idx:
            self.ax1.plot(xs, [float(r[idx["v(x)"]]) for r in data], 'r-', label="v (сетка n)")
            self.ax1.plot(xs, [float(r[idx["v2(x)"]]) for r in data], 'b--', label="v2 (сетка 2n)")
        self.ax1.set_xlabel("x")
        self.ax1.legend()

        # График погрешности
        self.ax2 = self.figure.add_subplot(122)
        self.ax2.grid(True)
        if "Разность" in idx:
            self.ax2.plot(xs, [float(r[idx["Разность"]]) for r in data], 'm-')
            self.ax2.set_ylabel("Разность |u-v|")
            self.ax2.set_yscale('log')
        self.figure.tight_layout()
        self.canvas.draw()

if __name__ == "__main__":
    app = SolverGui()
    app.mainloop()