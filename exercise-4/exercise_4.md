## Smooth Shading

Consider the simple 4 polygon set. Assuming we wish to use smooth shading for the surface it represents.

![hadin](assets/shading.png)

3. What is the vertex normal at the point **a** shown?

   * $$
     n = \frac{n_1 + n_2 + n_3 + n_4}{|n_1| + |n_2| + |n_3| + |n_4|} \\
     n = 
     \frac{
         \begin{bmatrix} 0.11 \\ 0.11 \\ 0.99 \end{bmatrix} +
         \begin{bmatrix} 0.35 \\ 0.0 \\ 0.85 \end{bmatrix} + 
         \begin{bmatrix} 0.34 \\ -0.22 \\ 0.91 \end{bmatrix} + 
         \begin{bmatrix} -0.50 \\ 0.0 \\ 0.87 \end{bmatrix}
     }
     { 1.0021 + 0.9192 + 0.9960 + 1.0034 } \\

     n = \frac{\begin{bmatrix} 0.3 \\ -0.11 \\ 3.62 \end{bmatrix}}{3.9209} \\
     n = \begin{bmatrix} 0.0765 \\ -0.0280 \\ 0.9233 \end{bmatrix}
     $$

4. ​