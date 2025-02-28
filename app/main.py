from fastapi import FastAPI
from fastapi.responses import FileResponse
from pydantic import BaseModel
import numpy as np
from typing import List
import matplotlib
import matplotlib.pyplot as plt
import sammon_module
import json

matplotlib.use('Agg')  # Usar backend no interactivo
app = FastAPI()

# Definir el modelo para el vector
class VectorF(BaseModel):
    vector: List[float]
    
@app.post("/sammon-mapping")
def calculo(num_samples: int, num_features: int, iterations: int, alpha: float):
    output_file = 'sammon_mapping.png'

    # Generar datos de ejemplo (Nube de puntos en 3D)
    np.random.seed(0)
    X = np.random.rand(num_samples, num_features) * 10

    # Aplicar Sammon Mapping
    max_iter=iterations
    Y, loss_history = sammon_module.sammon_mapping(X, max_iter, alpha)

    # Graficar los resultados
    plt.figure(figsize=(10, 4))

    # Gráfica de dispersión
    plt.subplot(1, 2, 1)
    plt.scatter(Y[:, 0], Y[:, 1], c='blue', alpha=0.5)
    plt.title("Sammon Mapping - Espacio Reducido")
    plt.xlabel("Dim 1")
    plt.ylabel("Dim 2")

    # Gráfica de la pérdida
    plt.subplot(1, 2, 2)
    plt.plot(loss_history, 'r-')
    plt.title("Evolución de la Pérdida")
    plt.xlabel("Iteraciones")
    plt.ylabel("Pérdida")

    plt.tight_layout()
    #plt.show()

    plt.savefig(output_file)
    plt.close()
    
    j1 = {
        "Grafica": output_file
    }
    jj = json.dumps(str(j1))

    return jj

@app.get("/sammon-mapping-graph")
def getGraph(output_file: str):
    return FileResponse(output_file, media_type="image/png", filename=output_file)