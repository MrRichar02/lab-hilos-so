import subprocess
import time

inicio = time.perf_counter()

with open("fibonacci.txt", "w") as f:
    subprocess.run(
        ["./fibonacci_s", "100000"],
        stdout=f,
        check=True
    )

fin = time.perf_counter()

print(f"Tiempo: {fin - inicio:.6f} segundos")
