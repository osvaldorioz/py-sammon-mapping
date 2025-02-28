## **Resumen del Algoritmo Sammon Mapping**  

Sammon Mapping es un método de reducción de dimensionalidad no lineal que proyecta datos de alta dimensión a un espacio de menor dimensión, preservando la estructura de las distancias entre los puntos.  

### **Funcionamiento del Algoritmo**  
1. **Calcular la matriz de distancias original**  
   - Se obtiene la distancia euclidiana entre cada par de puntos en el espacio de alta dimensión.  

2. **Inicializar aleatoriamente los puntos en el espacio reducido**  
   - Generalmente en 2D o 3D.  

3. **Minimizar la función de costo de Sammon**  
   ![imagen](https://github.com/user-attachments/assets/5a123223-c92f-442b-a2b8-892b730301b2)

4. **Optimizar la proyección iterativamente**  
   - Se actualizan las coordenadas en el espacio reducido mediante descenso de gradiente.  

---

## **Implementación en este Programa**  

### **C++ con Pybind11 y Eigen**
- **Cálculo de la matriz de distancias original**  
  - Se usa **Eigen::MatrixXd** para almacenar las distancias entre los puntos.  
  - Se calcula con la norma euclidiana.  

- **Inicialización aleatoria de la proyección en 2D**  
  - Se genera una matriz \( Y \) de valores aleatorios.  

- **Iteración para minimizar la pérdida de Sammon**  
  - Se actualizan las posiciones con una tasa de aprendizaje \( \alpha \).  
  - Se almacena la pérdida en cada iteración.  

- **Retorno de los datos a Python**  
  - La matriz \( Y \) es convertida a un array de NumPy.  
  - Se devuelve la lista de pérdidas.  

### **Python para Visualización**  
- Genera datos aleatorios en 3D.  
- Aplica **Sammon Mapping** llamando al módulo compilado en C++.  
- Genera dos gráficos:  
  1. **Dispersión de los datos transformados.**  
  2. **Evolución de la pérdida en las iteraciones.**  

Este programa combina la eficiencia de C++ para el cálculo numérico con la facilidad de uso de Python para la visualización. 🚀
